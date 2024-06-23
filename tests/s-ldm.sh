#!/usr/bin/env bash

export SLDM_INTERNAL_AREA="46.174740:11.108462-46.339863:11.312573"
export SLDM_EXTERNAL_AREA_LAT_FACTOR=0.0002
export SLDM_EXTERNAL_AREA_LON_FACTOR=0.0002
# Default broker URL and port
export BROKER_URL="127.0.0.1:5672"
# Default broker topic
export AMQP_TOPIC="topic://5gcarmen.examples"
export MS_REST_ADDRESS="http://localhost"
export MS_REST_PORT=8000
export VEHVIZ_UDP_ADDRESS="127.0.0.1"
export VEHVIZ_UDP_PORT=48110
export VEHVIZ_WEB_PORT=8080
export LOG_FILE="stdout"

/workspaces/S-LDM/build/Debug/sldm/s-ldm -A ${SLDM_INTERNAL_AREA} --ext-area-lat-factor ${SLDM_EXTERNAL_AREA_LAT_FACTOR} --ext-area-lon-factor ${SLDM_EXTERNAL_AREA_LON_FACTOR} --broker-url ${BROKER_URL} --broker-queue ${AMQP_TOPIC} --ms-rest-address ${MS_REST_ADDRESS} --ms-rest-port ${MS_REST_PORT} --vehviz-nodejs-address ${VEHVIZ_UDP_ADDRESS} --vehviz-nodejs-port ${VEHVIZ_UDP_PORT} --vehviz-web-port ${VEHVIZ_WEB_PORT} -L ${LOG_FILE} ${SLDM_OTHER_OPTIONS}
