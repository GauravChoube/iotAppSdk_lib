/***************************************************************************************
 * Filename    : cellularModuleMQTT.h
 * Created on  : Feb 11, 2020
 * Author      : Gaurav Choubey
 ****************************************************************************************/

#ifndef MAIN_CELLULARMODULE_CORECELLULARMODULE_CELLULARMODULEMQTT_H_
#define MAIN_CELLULARMODULE_CORECELLULARMODULE_CELLULARMODULEMQTT_H_

/****************************************************************************************
 * HEADER FILE
 ***************************************************************************************/

/****************************************************************************************
 * VARIABLE
 ***************************************************************************************/
typedef enum
{
    CONNECT_PACKET,
    PUBLISH_PACKET,
    SUBSCRIBE_PACKET,



}MQTTPACKET;

typedef enum
{
    MQTT_SUCCESS= 0xB000,
    MQTT_INIT_SUCCESS,
    MQTT_SEND_PACKET_SUCCESS,
    MQTT_PUBLISH_SUCCESS,
    MQTT_SUBSCRIBE_SUCCESS,
    MQTT_CONNECT_SUCCESS,

    MQTT_ACK_RECV ,
    MQTT_ACK_FAILE  ,
    MQTT_ACK_TIMEOUT,
    MQTT_ACK_ID_ERR,
    MQTT_ACK_CRED_ERR,

    MQTT_NOT_RESPOND,
    MQTT_CONNECTION_NOT_OPEN,

    CONNACK_RESPONSE     ,
    CONNACK_RESPONSE_FAILED   ,
    CONANCK_MQTT_ID_ERR,
    CONANCK_MQTT_CRED_ERR,
    SUBACK_RESPONSE     ,
    SUBACK_RESPONSE_FAILED    ,
    PUBACK_RESPONSE     ,
    PUBACK_RESPONSE_FAILED    ,



}MQTTRESPONSE;

#define cellMMQTTBuffSize 1024

volatile int16_t cellMMQTTpcktIDSend;
uint16_t        cellMMQttPacketresponse;

/****************************************************************************************
* FUNCTION
***************************************************************************************/
/**************************************************************************************
 * Description:
 * ============
 *  Deal with connect packet of MQTT protocols and send to MQTT server.
 *  Please established the socket connection with mqtt server before calling
 *  this function.
 *
 * INPUT:
 * ======
 * > No argument
 * RETURN:
 * =======
 * > MQTT_CONNECT_SUCCESS - connect packet send Successfully.
 * > MQTT_NOT_RESPOND     - MQTT response did not received within time
 * > CONNECTION_CLOSED    - Connection of socket is close.Please check the
 *                          network and internet.
 * > MQTT_ACK_CRED_ERR    - MQTT username and password,other connect packet parameter
 *                          are wrong.Please check it and retry again.
 * > MQTT_ACK_ID_ERR      -  MQTT id did not accep by MQTT server.Please check it.
 *
 *************************************************************************************/
uint16_t cellMMqttConnect();

/**************************************************************************************
 * Description:
 * ============
 *  Deal with publish packet of MQTT protocols and send Data on given publish topic
 *  to MQTT server.Please established the socket connection and MQTT connection with
 *  connect packet  before calling Publish Call.
 *
 * INPUT:
 * ======
 * > Pub_Topic  -  Topic on which data to be send.
 * > Buffer     -  Payload to be send.
 * > Buffer_len -  Payload length of Payload.
 * > Qos        -  Qaulity of service for publishing.
 *                 0 - only once, 1 - atleast once,2 - definitely one.
 *
 * RETURN:
 * =======
 * > MQTT_PUBLISH_SUCCESS - Data published successfully
 * > MQTT_NOT_RESPOND     - MQTT response did not received within time.
 * > CONNECTION_CLOSED    - Connection of socket is close.Please check the
 *                          network and Internet.

 *************************************************************************************/
uint16_t cellMMqttPublish(int8_t *Pub_Topic,int8_t * Buffer,uint16_t Buffer_len,int8_t Qos);

/**************************************************************************************
 * Description:
 * ============
 *  Deal with subscribe packet of MQTT protocols and subscribe on given sub topic
 *  to MQTT broker.Please established the socket connection and MQTT connection with
 *  connect packet before calling Publish Call.
 *
 * INPUT:
 * ======
 * > Sub_Topic  -  Topic on which data to be send.
 * > Qos        -  Qaulity of service for publishing.
 *                 0 - only once, 1 - atleast once,2 - definitely one

 * RETURN:
 * =======
 * > MQTT_SUBSCRIBE_SUCCESS - Data subscribe successfully
 * > MQTT_NOT_RESPOND     - MQTT response did not received within time.
 * > CONNECTION_CLOSED    - Connection of socket is close.Please check the
 *                          network and Internet.
 *************************************************************************************/
uint16_t cellMMqttSubscribe(int8_t * Sub_Topic,int16_t Qos);

/**************************************************************************************
 * Description:
 * ============
 *  Send the Data on given topic based on QOS type.This function handle everything.
 *  Its established socket connection with broker and then send connect packet and
 *  subscribe packet to MQTT broker,once everything is fine,start publishing a data
 *  to MQTT server. Please call MQttsetup function before calling this function
 *  which setup credential of MQTT broker
 *
 * INPUT:
 * ======
 * > pubTopic    -  Topic on which data to be send.
 * > payload     -  Payload to be send.
 * > payloadLen  -  Payload length of Payload.
 * > Qos         -  Quality of service for publishing.
 *                  0 - only once, 1 - atleast once,2 - definitely one.
 *
 * RETURN:
 * =======
 * > MQTT_SUCCESS      - Data published successfully
 * > MQTT_NOT_RESPOND  - MQTT response did not received within time.
 * > MQTT_CONNECTION_NOT_OPEN - Unable to Open connection with MQTT broker
 * > CONNECTION_CLOSED - Connection of socket is close.Please check the
 *                       network and Internet.
 *
 *
 *************************************************************************************/
uint16_t cellMqtt(int8_t *payload,uint16_t payloadLen,int8_t *pubTopic,uint8_t pubQos);

/**************************************************************************************
 * Description:
 * ============
 *  Configure the MQTT credential and store into cellular Module library for MQTT
 *  connection.
 *
 * INPUT:
 * ======
 * > url  - MQTT url
 * > port - MQTT port
 * > usr  - MQTT username
 * > pass - MQTT password
 * > id   - MQTT device id
 * > ptpc - MQTT publish topic
 * > stpc - MQTT Subscribe topic
 *
 * RETURN:
 * =======
 * > None
 *
 *
 *************************************************************************************/
void cellMQttSetup(int8_t *url,int8_t *port,int8_t *usr,int8_t *pass,int8_t *id,int8_t *ptpc,int8_t *stpc);

/**************************************************************************************
 * Description:
 * ============
 *  Validate the return value and depend on given rval, state variable is changed
 *  which is increment or decrement state variable.
 *
 * INPUT:
 * ======
 ** > state_variable - Address of State variable of particular function.
 * > rval           - Address of return variable of particular function.
 *
 * RETURN:
 * =======
 * Depend on fuction called.
 *
 *************************************************************************************/
void mqttReturnValue(uint8_t *state_variable,uint16_t *rval);




#endif /* MAIN_CELLULARMODULE_CORECELLULARMODULE_CELLULARMODULEMQTT_H_ */
