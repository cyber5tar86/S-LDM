//
// Created by Lorenzo De Persiis, 2021
//

#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>

#include <proton/connection.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/tracker.hpp>
#include <proton/connection_options.hpp>

#include <sys/types.h>
#include <sys/socket.h>

// TCLAP headers
#include "tclap/CmdLine.h"


extern "C" {
	#include "rawsock.h"
	#include "rawsock_lamp.h"
	#include "ipcsum_alth.h"
	#include "minirighi_udp_checksum.h"
}


#include "camrelayeramqp.h"
#include "sample_quad_final.h"

// Global atomic flag to terminate the whole program in case of errors
std::atomic<bool> terminatorFlag;

// Thread callback function
void *CAMrelayer_callback(void *arg) {
	CAMrelayerAMQP *cr_AMQP_class_ptr=static_cast<CAMrelayerAMQP *>(arg);

	// Checking this just as a matter of additional safety
	if(cr_AMQP_class_ptr!=NULL) {
		try {
			// Create a new Qpid Proton container and run it to start the AMQP 1.0 event loop
			proton::container(*cr_AMQP_class_ptr).run();

			pthread_exit(NULL);
		} catch (const std::exception& e) {
			std::cerr << "Qpid Proton library error while running CAMrelayerAMQP. Please find more details below." << std::endl;
			std::cerr << e.what() << std::endl;
			terminatorFlag = true;
		}
	} else {
		std::cerr << "Error. NULL CAMrelayerAMQP object. Cannot start the AMQP client." << std::endl;
		terminatorFlag = true;
	}

	// Even if this thread will run forever (then, we can think also about adding, in the future, some way to gracefully terminated it)
	// it is good pratice to terminate it with pthread_exit()
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	// Create thread structure to pass the needed arguments to the thread callback
	pthread_camrelayer_args_t cam_args;
	int comm_port = 20000;

	// Parse the command line options with the TCLAP library
	try {
		TCLAP::CmdLine cmd("S-LDM Testing Facilities - UDP->AMQP 1.0 relayer", ' ', "1.0");

		// Arguments: short option, long option, description, is it mandatory?, default value, type indication (just a string to help the user)
		TCLAP::ValueArg<std::string> urlArg("U","url","Broker URL (with port)",false,"127.0.0.1:5672","string");
		cmd.add(urlArg);

		TCLAP::ValueArg<std::string> queueArg("Q","queue","Broker queue or topic",false,"topic://5gcarmen.examples","string");
		cmd.add(queueArg);

		TCLAP::ValueArg<int> portArg("P","comm-port","Port for the UDP communication with ms-van3t",false,20000,"int");
		cmd.add(portArg);

		cmd.parse(argc,argv);

		cam_args.m_broker_address=urlArg.getValue();
		cam_args.m_queue_name=queueArg.getValue();
		comm_port=portArg.getValue();

		std::cout << "The relayer will connect to " + cam_args.m_broker_address + "/" + cam_args.m_queue_name << std::endl;
	} catch (TCLAP::ArgException &tclape) { 
		std::cerr << "TCLAP error: " << tclape.error() << " for argument " << tclape.argId() << std::endl;
	}

	// CAM relayer object
	CAMrelayerAMQP CAM_relayer_obj;

	// Creation of the thread
	// CAM Relayer Thread attributes
	pthread_attr_t tattr;
	// CAM Relayer Thread ID
	pthread_t curr_tid;

	// Set the terminator flag to false
	terminatorFlag = false;

	// Set the arguments/parameters of the CAMrelayerAMQP object
	CAM_relayer_obj.set_args(cam_args);

	// pthread_attr_init()/pthread_attr_setdetachstate()/pthread_attr_destroy() may probably be removed in the future
	// If removed, the second argument of pthread_create() should be NULL instead of &tattr
	pthread_attr_init(&tattr);
	pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);

	// Passing as argument, to the thread, a pointer to the CAM_relayer_obj CAMrelayerAMQP object
	// pthread_create() actually creates a new (parallel) thread, running the content of the function "CAMrelayer_callback" (which must be a void *(void *) function)
	pthread_create(&curr_tid,&tattr,CAMrelayer_callback,(void *) &(CAM_relayer_obj));
	pthread_attr_destroy(&tattr);

	// Wait for the sender to be open before moving on (as required and as described inside camrelayeramqp.h)
	bool sender_ready_status;

	std::cout << "Waiting for the AMQP sender to be ready..." << std::endl;

	sender_ready_status=CAM_relayer_obj.wait_sender_ready();

	std::cout << "Sender should be ready. Status (0 = error, 1 = ok): " << sender_ready_status << std::endl;

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	//address.sin_addr.s_addr = INADDR_ANY;
	if(inet_pton(AF_INET,"10.10.7.254",&address.sin_addr)<1) {
		std::cerr << "Error: cannot set an IP address to bind to." << std::endl;
		exit(EXIT_FAILURE);
	}
	address.sin_port = htons(comm_port);
	socklen_t addrlen = sizeof(struct sockaddr_in);

	uint8_t buffer[1024];
	int buf_length = sizeof(buffer);

	int soc = socket(AF_INET,SOCK_DGRAM,0);
	std::cout << "Bind IP address: " << inet_ntoa(address.sin_addr) << std::endl;

	if(bind(soc, (struct sockaddr*) &address, addrlen)<0) {
		std::cerr << "Error: cannot bind()." << std::endl;
		exit(EXIT_FAILURE);
	}
	int recv_bytes;
	GNmetadata_t gnmetadata;


	//int listen(int soc,int backlog);
	while(terminatorFlag==false) {
		//int listen(int soc,int backlog);
		recv_bytes = recvfrom(soc, buffer, buf_length, 0, NULL, NULL);

		memcpy(&gnmetadata,(void *) buffer, sizeof(GNmetadata_t));

		uint64_t stationID = ntoh64(gnmetadata.stationID);
		double lat = (double) ntohl(gnmetadata.lat)/1e7;
		double lon = (double) ntohl(gnmetadata.lon)/1e7;
		std::cout<<"Check:"<<sizeof(GNmetadata_t)<<"Station ID: "<<stationID<<" Coordinates: "<<lat<<" "<<lon<<std::endl;

		for(int i=0;i<recv_bytes;i++) {
			printf("%02X",buffer[i]);
		}
		// Just to add a newline at the end...
		std::cout << std::endl;

		CAM_relayer_obj.sendCAM_AMQP(buffer+sizeof(GNmetadata_t),recv_bytes-sizeof(GNmetadata_t),lat,lon,16);
	}

	return 0;

}
