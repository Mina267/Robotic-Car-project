


#ifndef SPI_SERVICE_CFG_H_
#define SPI_SERVICE_CFG_H_



/*******************************************************************************************************/
/****************************** Receive and Send Frames using SPI Interrupt ****************************/
/*************************************** Frame Send Receive Cfg ****************************************/
/*******************************************************************************************************/
#define Q_FRAME_REC_SIZE			30
#define SPI_FRAME_SIZE				1



/*******************************************************************************************************/
/********************************** Send and Receive Frame Synchronous *********************************/
/*******************************************************************************************************/
/* Send Data Synchronous used by master MCU *
 * 20 us for 4 frames */

#define SPI_SEND_REC_FRAME	4

/*******************************************************************************************************/
/******************* SPI Queue For data Received uing External interrupt *******************************/
/*******************************************************************************************************/
#define Q_REC_SIZE					4
#define SPI_STR_REC_MAX_SIZE		30




/*******************************************************************************************************/
/********************************* Send String Synchronous *********************************************/
/*******************************************************************************************************/
#define SPI_1ST_STR_SEND_END		'\n'
#define SPI_2ST_STR_SEND_END		0



#define SPI_1ST_STR_REC_END			'\n'
#define SPI_2ST_STR_REC_END			0







#endif /* SPI_SERVICE_CFG_H_ */