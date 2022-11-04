set(
    MQTT_SRC
    "./mqtt/mqtt.c"
)

set(
    MQTT_INCLUDE
    "./mqtt"
)

idf_component_register(SRCS ${MQTT_SRC}
                    INCLUDE_DIRS ${MQTT_INCLUDE}
                    REQUIRES mqtt)
