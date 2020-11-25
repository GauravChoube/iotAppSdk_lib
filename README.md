Introduction :
==============================================================
This is IOT application which include function of two call mainly,configure as well as Data call,selection 
of module(internet module) and getting network,registered,assigned IP address and attached internet, also deal
with sleep functionality.This is complete internet of things application with portable feautre and designed in
module wise manner whihc can be replace without affecting other code.

Feature :
===============================================================
> Connectivity Module Selection.
> Configuration Call.
> Heartbeat Call.
> Power Call.
> Application Data Updation.
> Sleep Mode handling.
> Automatic Device Recovery.
> Extrnal Flash memory handling.
> 10,000 Packet stored.

Plateform:
==============================================================
> Microcontroller 		: MSP430FR5969
> Connectivity Module 	: SIM800/868
> Language        		: Embedded C
> Internet Protocols 	: HTTP/HTTPS,MQTT/MQTTS,TCP/SSL.
> Size            		: 40KB flash ,1.5KB RAM.
> Requirement     		: 1-UART 0(connected with modem)
                          2-TIMER 1,2(used in IOT APP SDK)
                          1-SPI FLASH.(optional for data storage)



IOTAPPSDK:
========================================================================
This is complete bunch of code which has everything from hardware low level to Application.
This SDK is designed in such way that can be portable just changing minor code as echa divided based
on their function.
Generally,Its categories as three Section as follow.

 1.Application
 --------------
    That project application like data capture from sensor,event handling or triggering,etc.
    Application sholud be here and call accordingly which no need to wory about communication
    part which completely taken care by IOTAPP.
    
     1. Application section is responsible for forming actual packet and stored into extenral flash 
        memory.External flash memory is implemented in Queue maner as FIFO style.
     2. Application section is responsible for handling config call response(setting).
     3. Its handle sleeping event and take necessary action if sleep feature enabled.
     4. Its deal with internal flash/storage if need
     
    
    
 2.controllerLL
 --------------
    Micrcontroller low level interface which has driver for peripheral in project.
    Depend of microcontroller/soc replace this file respective soc driver but remember
    api name should be same in order work.
    
 3.IOTAPP       
 ---------
 	 Main body that actually control whole device,configured it,and data uploading,sleeping decison.
 	 IOTAPP has connectivity module and communication application. Depend on Connectivty module,IOT
 	 app used the respective function in order to communicate with any server.This mean to designed to 
 	 handle two connectivity module,cellular or wifi. Cellular module is ready here,wifi will come soon.
 	 IOTAPP support all internet protocol but maily developed for HTTP/HTTPS,MQTT/MQTTS,TCP/SSL.
 	 
 	 Communication application:
 	 --------------------------
 	 It has two main call namely config call and data call.This part code always
 	 common to project no need chnaged in case of porting. In case of porting to another cellular module,
 	 just replce cellular module with respective another cellular module.
 	
	 	 Main funciton or communication part is
	 	 a.Selection of connectivity module(cellular or wifi).
	 	 b.Bring the internet connectivity.
	 	 c.config call 
	 	 d.data call
	 	 
	 Connectivity module:
	 -------------------
	 Its library or bunch of api based on protocol going to used such HTTP/HTTPS,MQTT/MQTTS,TCP/SSL
	 as well as api to control connectivity module.This deal with all low level interface and execute
	 respective protocol action.
	 
	 
 	 This IOT APP designed in such data can have control over device and This can be acheived with
 	 below architecture of two call.
 	 
 	 Config call:
 	 -----------
 	 1.It is HTTP/HTTPS call which used to setup device,its request for setting to provision cloud.
 	   Once device ready at cloud side,device get setting which has information about data cloud(url,
 	   username,passwod,protocol,etc) where all applicaton payload or packet to be send with other 
 	   parameter such aliase name,RTC,etc.
 	 2.Setting has some parameter fixed like data cloud credential,but also has optional that means
 	   need parameter can be add and process seperately if need directly in application section of sdk.
 	 3.Its also update the device status bases on heart beat time to provision cloud.
 	 4.Format of config call is different, change the format in iotComman.c.
 	 5.Config call can be disable,data cloud must be hardcode otherwise device restart.
 	 6.Config call execute on power on,every heartbeat time(configurable through setting),any failure of
 	   system and in case of setting corruption.
 	 
 	 DATA call:
 	 -----------
 	 1.Its MQTT/MQTTS/HTTPS/HTTP call which used to send payload or packet of application to
 	   data cloud.
 	 2.In Data call,its read data/packet from external flash and validate.If evrything is fine,
 	   send packet to cloud that received on config call setting.
 	 3.In case of Validation fail,code discard message go with next packet.
 	 4.Data call execute continuously till data/packet present in external flash.
 	 5.Once all data send,device wait for time(configurable)and enter to sleep
 	 
 
HOW TO USE IT.
============================================================
	Config.h and cellularModuleConfig.h file need to setup before uploading.
	These decide working sequence of IOT APP SDK,setup carefully.

	cellularModuleConfig.h:
	-----------------------
	1.Set each MACRO in cellularModuleConfig.h which set hardware interface with cellular modem.
	  Properly mentioned peripheral number not a address.
	2.Set APN of SIM in cellularModuleConfig.h.
	3.Select Type of network mode(2G/3G/4G/NBIOT - depend cellular modem).
	4.Set size of cellularBuffer(must be more than 700 byte).

        
	Config.h
	---------
	1.Hardcoded the project firmware.

	2.Enabling the debug message by defining Debug_Enable(setup debug uart).

	3.Define connectivity module to be used in project
		  Two option are there
		  # CELLULAR_MODULE
		  # WIFI_MODULE(Coming soon).

	4.Set HARDCODE VALUE depend on project.
		 # HARDCODED_DEV_ID
		 # HARDCODED_MQTT_CREDENTIAL
		 # HARDCODED_MQTT_DEV_ID
		 # OW_MQTT_CLOUD
		 # THINGSBOARD
		 # HIVEMQ
		 # CONFIG_CALL_ENABLE
		 # CONFIG_TIME_HARDCODED
		 # SLEEP_MODE_ENABLE

	5.Select the Connection Call Type that enable respective stack eventually cause memory.
		 #DATA_CALL_TYPE
	     #CONFIG_CALL_TYPE

	6.Select the Mode of protocol Stack.There are three option avail for all protocols
	  as over AT command,over PPP protocols,over TCP/SSL protocols.
	   1.FOR HTTP STACK
	   # HTTP_OVER_AT
	   # HTTP_OVER_TCP
	   # HTTP_OVER_PPP

	   2.FOR HTTPS STACK
	   # HTTPS_OVER_AT
	   # HTTPS_OVER_SSL
	   # HTTPS_OVER_PPP

	   3.FOR MQTT STACK
	   # MQTT_OVER_AT
	   # MQTT_OVER_TCP
	   # MQTT_OVER_PPP

	   4.FOR MQTTS STACK
	   # MQTTS_OVER_AT
	   # MQTTS_OVER_SSL
	   # MQTTS_OVER_PPP

	 7. Setup config call credential which must be fill if enable  
		# CONFIG_URL                   	
		# CONFIG_URL_PATH                
		# CONFIG_USERNAME                 
		# CONFIG_PASSWORD                 
		# CONFIG_AUTH_KEY                           

	 8. Setup hardcode id if needed.

	Once both file fill with proper information, device start connecting to provision cloud,get
	setting and start uploading data.


SAMPLE
========================================================================
	iotAPPSdk come with default sample code where its connecting thigsboard cloud.
	iotAPPSdk send config call request and get setting. Depend on setting parameter,
	sdk form packet sample and send to same cloud at given internal in setting.

	Please visit below link to configure setting for config call as well as monitor
	sample data.

	cloud Plateforma
	-----------------
	link: https://cloud.gciot.tk:/dashboard/daa2ed00-11dc-11eb-af3c-696883eb4ab8?publicId=1cbc82d0-00e1-11eb-af3c-696883eb4ab8

	Device id: Device 1
	username : Device 1
	password : (empty)
	Protocols: HTTPS
=============================================================================================================
