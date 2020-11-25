/***************************************************************************************
 * Filename    : cellularUnsolicRespons.h
 * Created on  : Feb 11, 2020
 * Author      : Gaurav Choubey
 ****************************************************************************************/
#ifndef MAIN_CELLULARMODULE_CORECELLULARMODULE_CELLULARUNSOLICRESPONS_H_
#define MAIN_CELLULARMODULE_CORECELLULARMODULE_CELLULARUNSOLICRESPONS_H_

/****************************************************************************************
 * HEADER FILE
 ***************************************************************************************/

/****************************************************************************************
 * VARIABLE
 ***************************************************************************************/
extern bool processingUnsloictMsg;

/****************************************************************************************
* FUNCTION
***************************************************************************************/
/**************************************************************************************
 * Description:
 * ============
 *  Whatever response receive from server,its will process here that means
 *  extraction of http code from http response,MQTT packet parsing from MQTT
 *  response.
 *
 * INPUT:
 * ======
 * > None
 * RETURN:
 * =======
 * > None
 *
 *
 *************************************************************************************/
uint16_t unsolicCellularResponse();


#endif /* MAIN_CELLULARMODULE_CORECELLULARMODULE_CELLULARUNSOLICRESPONS_H_ */
