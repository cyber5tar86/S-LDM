# S-LDM (Server Local Dynamic Map)

![](./docs/pics/CNIT_logo.png)

![](./docs/pics/PoliTO_logo.png)

![](./docs/pics/5G-CARMEN-logo.png)

The **Server Local Dynamic Map** (in short, **S-LDM**) is a Linux-based Local Dynamic Map (LDM), developed by [CNIT](https://www.cnit.it/) as part of the *5G-CARMEN European Project* and based on the LDM concept [defined by ETSI](https://www.etsi.org/deliver/etsi_en/302800_302899/302895/01.01.01_60/en_302895v010101p.pdf).

The S-LDM is designed to be run on any Multi-Access Edge Computing (MEC) platform based on Linux, and is aimed at collecting data from vehicles thanks to the reception of standard-compliant Cooperative Awareness Messages (CAMs [version 2](https://www.etsi.org/deliver/etsi_en/302600_302699/30263702/01.04.01_60/en_30263702v010401p.pdf)). When it receives data from vehicles, it pre-processes it and stores it in a highly-efficient C++ custom in-memory database, with the aim of providing the needed subset of pre-processed data (i.e., a "map of the road") to other MEC services (e.g., centralized automated maneuvering services).

It can then detect when certain "triggering conditions" occur, and use them as a trigger to start periodically providing the data, through a REST interface, to other MEC services. In particular, the S-LDM will act as REST client, and expect the other services to expose a REST server.
Currently, the S-LDM considers as triggering condition **when a vehicle turns on either its right or its left turn indicator** (unless the check is disabled through dedicated options - i.e., `--indicator-trgman-disable` - or the related information is not encapsulated inside CAMs), as it is focused on an automated lane merge maneuver scenario. It will then start periodically sending towards a REST server the data of objects within a certain radius (default: 150 m) around the vehicle(s) which has turned on its indicator, until the server replies with a JSON with a field named `rsp_type` set to the string `STOP`. See also the sample Python 3 REST server located in `./tester/Python-REST-server` for more details on how to receive data from the S-LDM.

The S-LDM can receive CAM messages through one or more **AMQP 1.0** brokers (we tested it on the [Apache ActiveMQ "Classic"](https://activemq.apache.org/components/classic/) broker) to which the S-LDM should be subscribed. The S-LDM can receive both *full ITS messages* (i.e., CAMs encapsulated inside the ETSI GeoNetworking and BTP, in turn encapsulated inside AMQP 1.0) and *pure CAMs* (i.e., CAMs directly encapsulated into AMQP 1.0, without any ETSI Transport and Networking layer). The detection of the message type is performed automatically by the S-LDM and it is thus transparent to the end-user. It is also planned to implement the support to the oncoming [Cooperative Perception Messages (CPMs)](https://www.etsi.org/deliver/etsi_tr/103500_103599/103562/02.01.01_60/tr_103562v020101p.pdf), which are currently being standardized by ETSI.

In order to ensure compativility with the S-LDM, vehicles should send their messages (encapsulating the binary data of CAMs into the AMQP 1.0 protocol) to a well defined topic or queue of an active AMQP 1.0 broker. The S-LDM should be then set to subscribed to the proper topic to receive these messages.

To tackle scalability, each S-LDM instance should be started by specifying a **coverage area**, which can be specified through the `--area`/`-A` option. It will then store data on vehicles and other objects sending CAMs **only when they are located inside the coverage area**. Currently, only rectangular areas based on latitude and longitude values (in degrees) are supported.
This area is called **internal area**. It can then be extended with the `--ext-area-lat-factor`/`-E` and `-ext-area-lon-factor`/`-F` options (which define an additive factor, in degrees, to be added to the latitude and longitude values specified with `-A`). If specified, the `-E` and `-F` options define the so-called **external area**. The S-LDM will store data about vehicles located inside the **internal and external areas**, but it will trigger the data transmission only when a vehicle is switching on its turn indicator inside the **internal area**.

Furthermore, to facilitate filtering of messages outside the coverage area, the S-LDM expects each message to include an **AMQP 1.0 property** named **`quadkeys`**, which contains the [Quadkey](https://docs.microsoft.com/en-us/bingmaps/articles/bing-maps-tile-system) of the current position of the vehicle. This Quadkey should have a **level of detail of at least 16**. If Quadkeys cannot be generated by the vehicle OBUs, you must disable the Quadkey check with the `--disable-quadkey-filter` option.

When the Quadkeys filtering is enabled, the S-LDM will compute a set of Quadkeys corresponding to the internal and external area, to match them afterwards with the ones received from vehicles. As this operation can be computationally expensive (depending on the actual size of the area), the S-LDM will attempt to perform it only once, and then save the result on a cache file, named `cachefile.sldmc`, in case the S-LDM is restarted with the same internal and external area settings. This file is rewritten every time there is a change in the area settings with which the S-LDM is started. If you want to clear this cached data for any reason, you can simply delete the `cachefile.sldmc` file.

Finally, if **pure CAMs** are sent, with no GeoNetworking layer, it is recommended that the messages sent by vehicles contain an AMQP 1.0 property named **`gn_ts`** containing a timestamp calculated in the same way as the GeoNetworking timestamp. The GeoNetworking timestamp is calculated as (see also [this standard](https://www.etsi.org/deliver/etsi_en/302600_302699/3026360401/01.04.01_60/en_3026360401v010401p.pdf) on page 22, looking for the **TST** field): `Timestamp = <TAI Timestamp> mod 2^(32)`. Not including this property when sending pure CAMs will **not** affect the ability of the S-LDM to receive and store data. However, it will disable a useful check which makes sure that it always stores up-to-date data even in case of out-of-order CAM reception.

The S-LDM has been succesfully tested on Ubuntu 18 LTS and Ubuntu 20 LTS (and inside Docker containers based on these two Ubuntu versions).


# Compiling the S-LDM

The S-LDM can be easily compiled with `make`.

Before compiling it, you need to install the following **pre-requisites**: `libcpprest-dev`, `libgeographic-dev`. Under Ubuntu, you can install them with:
```
sudo apt install libcpprest-dev libgeographic-dev
```

The S-LDM also needs the **Qpid Proton** AMQP 1.0 client library to be installed.

You can install it by downloading the latest version of the library (we tested the S-LDM with Qpid Proton up to version 0.35.0, but it should work also with newer versions) from [here](https://qpid.apache.org/releases/qpid-proton-0.35.0/).
You can then extract the contents of the .tar.gz file and follow the instruction reported inside `INSTALL.md`.

For your convenience, we report here the commands we typically use to install Qpid Proton on Ubuntu (with reference to version 0.35.0 and for usage with the S-LDM):
```
cd <Qpid Proton directory>
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DSYSINSTALL_BINDINGS=ON -DBUILD_PYTHON=OFF -DBUILD_RUBY=OFF -DBUILD_GO=OFF
make all
sudo make install
```


Finally, the S-LDM web-based graphical interface (see below) requires Node.js to be installed. It can be installed on Ubuntu with:
```
sudo apt install nodejs
```


After installing the pre-requisites, you can clone this repository:
```
git clone https://github.com/francescoraves483/S-LDM
cd S-LDM
```
And build the S-LDM:
```
make
```

The compiled executable is called `SLDM`.


# S-LDM help and available options

After compiling the S-LDM service, you can view all the available options with:
```
./SLDM --help
```

In the options description, the destination MEC service, when a triggering condition occurs, is generically referred to as `Maneuvering Service`, with reference to our baseline automated maneuver scenario. However, any service can actually be implemented to receive data from the S-LDM, provided that it implements a proper REST server. A sample Python 3 REST server is available inside `./tester/Python-REST-server`.


# Containerizing the S-LDM

The S-LDM can be easily deployed as a Docker container, thanks to the Dockerfile included in `./docker`. The same folder contains also some brief instructions on how to build a new Docker container with the S-LDM.


# Example: running the S-LDM

A sample command for running the S-LDM is provided below:
```
./SLDM -A 42.072561:12.071470-42.392071:12.815384 --ext-area-lat-factor 0.002 --ext-area-lon-factor 0.002 --broker-url 127.0.0.1:5672 --broker-queue topic://5gcarmen.samples -L stdout --disable-quadkey-filter
```

This command will make the S-LDM cover a rectangular area between (lat, lon) `42.072561, 12.071470` and (lat, lon) `42.392071, 12.815384`. It also defines an external area by extending all the rectangle sides by 0.002 degrees. It sets then the S-LDM to subscribe to an AMQP 1.0 broker located at `127.0.0.1:5672`, using a queue/topic named `topic://5gcarmen.samples`. It also enables verbose output on standard output with the `-L stdout` option. Finally, it disables the Quadkey check (with `--disable-quadkey-filter`) to receive all CAM messages, even if they do not contain the `quadkeys` AMQP 1.0 property.
After starting the S-LDM with this command, it will expect to receive CAMs from vehicles on the `topic://5gcarmen.samples` queue of an AMQP 1.0 broker available at `127.0.0.1:5672`.

Thanks to the `-L` option, verbose information about each received message will be printed to standard output.


# The S-LDM web-based interface ("vehicle visualizer")

By default, the S-LDM also launches a web-based user interface, on **port 8080**, displaying the content of the database (i.e., the stored vehicles and road objects on a map), updated every 500 ms.
The behaviour of the web-based interface can be customized with `--vehviz-update-interval` (to change how often the GUI is updated) and `--vehviz-web-port` (to change the port at which the web GUI is available).


# Retrieving data from the S-LDM on demand

It is also possible to retrieve data from the S-LDM database on demand, thanks to a lightweight JSON-over-TCP interface, without the necessity of relying on the triggering mechanisms.
This interface is enabled when the `--enable-on-demand-requests` option is specified.

When enabled, the S-LDM will expose a TCP server, using the default 49000 port (unless a different port is specified with `--set-on-demand-json-port` or its short alternative `-o`).

The server expects to receive requests in the form of TCP packets containing a JSON payload like the following:
```
{
  "lat": <latitude of the center of the area>,
  "lon": <longitude of the center of the area>,
  "range": <radius in meters of the area of interest>
}
```
The S-LDM, upon reception of the "JSON request", will send back a TCP packet with a JSON payload, which contains the list of vehicles and other objects located inside the requested area.

A sample Python JSON-over-TCP client to retrieve data from the S-LDM is available inside `./tester/Sample-JSON-client`. This sample client can be used as a base to develop your own, more complex, service retrieving data on demand from the S-LDM.


# Included directories

The repository contains the following folders:
- `decoder-module`, containing the source code of the ETSI ITS-G5 message decoder (includes the decoding stack for BTP and GeoNetworking)
- `docker`, for the Dockerfile to build the S-LDM Docker image
- `docs`, for images and documentation
- `examples`, for any example useful to the development of the S-LDM - *currently unused*
- `include`, for all the header files
- `js`, containing the web-based vehicle visualized javascript files (the vehicle visualizer is licensed under **GPLv2** and it is derived directly from [ms-van3t](https://github.com/marcomali/ms-van3t))
- `options`, containg the source files of a C, highly-efficient and customizable options management module (the options module is licensed under **GPLv2** and it is derived directly from [LaTe](https://github.com/francescoraves483/LaMP_LaTe))
- `src`, for all the .c and .cpp/.cc source files
- `tester`, for a tester module, to be used in conjunction with [ms-van3t](https://github.com/marcomali/ms-van3t), with the aim of testing the S-LDM in the lab with emulated vehicles. This folder also includes a sample Python 3 REST server to mock any MEC service receiving data from the S-LDM REST client.
- `vehicle-visualizer`, containing the C++ code for the communication between the main S-LDM process and the javascript server managing the vehicle visualizer (vehicle-visualizer is licensed under **GPLv2** and it is derived directly from [ms-van3t](https://github.com/marcomali/ms-van3t))



# Acknowledgments

We currently have an accepted paper at IEEE VTC2022-Spring (19-22 June 2022), titled "*S-LDM: Server Local Dynamic Map for Vehicular Enhanced Collective Perception*".

To acknowledge us in your publications, please refer to the following citation:
```
F. Raviglione, C. M. R. Carletti, C. Casetti, F. Stoffella, G. M. Yilma and F. Visintainer, "S-LDM: Server Local Dynamic Map for Vehicular Enhanced Collective Perception," 2022 IEEE 95th Vehicular Technology Conference: (VTC2022-Spring), Helsinki, Finland, 2022, pp. 1-5, doi: 10.1109/VTC2022-Spring54318.2022.9860701.
```

BibTeX format:
```
@INPROCEEDINGS{S-LDM_paper_2022,
  author={Raviglione, Francesco and Carletti, Carlos Mateo Risma and Casetti, Claudio and Stoffella, Filippo and Yilma, Girma M. and Visintainer, Filippo},
  booktitle={2022 IEEE 95th Vehicular Technology Conference: (VTC2022-Spring)}, 
  title={S-LDM: Server Local Dynamic Map for Vehicular Enhanced Collective Perception}, 
  year={2022},
  volume={},
  number={},
  pages={1-5},
  doi={10.1109/VTC2022-Spring54318.2022.9860701}}
```


![](./docs/pics/EU_flag.jpg)

**Please have a look also at disclaimer.txt, as this work is included in the European Union Horizon 2020 project 5G-CARMEN co-funded by the EU.**


This project includes the **json11** C++ library by Dropbox: [github.com/dropbox/json11](https://github.com/dropbox/json11)



# Contact and Licensing

This project, since it has been released to the public, is licensed under the GPL-2.0 license. Please see also the *LICENSE* file.

For any question or further detail, feel free to contact me at _francescorav.es483@gmail.com_.