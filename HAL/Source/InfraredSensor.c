#include "StdTypes.h"
#include "EX_Interrupt.h"
#include "DIO_interface.h"

#include "InfraredSensor.h"
#include "InfraredSensor_Private.h"

/* IR sensor status array */
static InfraredSensor_Status_t InfraStatus_Array[SENSOR_ARRAY_SIZE] =
{
	NO_OBSTACLE,
	NO_OBSTACLE
	};


/* Initialization Infra red sensor with external interrupt
 * Enable external interrupt zero & one
 *`Set call back function of external interrupt
 * Initialize interrupt on falling edge.
 */
void InfraredSensorEXI_Init(void)
{
	EXI_Init();	
	EXI_Enable(EX_INT0);
	EXI_Enable(EX_INT2);
	
	EXI_TriggerEdge(EX_INT0, FALLING_EDGE);
	EXI_TriggerEdge(EX_INT2, FALLING_EDGE);
	EXI_SetCallBack(EX_INT0, Infra_sensor_EX_INT0_CallBack);
	EXI_SetCallBack(EX_INT2, Infra_sensor_EX_INT2_CallBack);
}


void InfraredSensor_Init(void)
{
	DIO_Init();
}



/* Running of infra red sensor */
void InfraredSensor_Runnable(void)
{
	DIO_PinVoltage_type _1_IR_Status = DIO_ReadPin(_1_IR_PIN);
	DIO_PinVoltage_type _2_IR_Status = DIO_ReadPin(_2_IR_PIN);
	
	/* If detect high on DIO pin there is no obstacle */
	if (_1_IR_Status == HIGH)
	{
		InfraStatus_Array[_1_INFRA_SENSOR] = NO_OBSTACLE;
	}
	/* If detect Low on DIO pin there is obstacle */
	else if (_1_IR_Status == LOW)
	{
		InfraStatus_Array[_1_INFRA_SENSOR] = DETECTED_OBSTACLE;
	}
	
	/* If detect high on DIO pin there is no obstacle */
	if (_2_IR_Status == HIGH)
	{
		InfraStatus_Array[_2_INFRA_SENSOR] = NO_OBSTACLE;
	}
	/* If detect Low on DIO pin there is obstacle */
	else if (_2_IR_Status == LOW)
	{
		InfraStatus_Array[_2_INFRA_SENSOR] = DETECTED_OBSTACLE;
	}
	
}


/* Get status of IR Sensor read */
InfraredSensor_Status_t InfraredSensor_Getter(InfraredSensor_t Sensor)
{
	return InfraStatus_Array[Sensor];
}

#include "LCD_interface.h"
/* Call back function for external interrupt IR sensor one */
static void Infra_sensor_EX_INT0_CallBack(void)
{
	static bool_t State_flag = TRUE;
	/* Detect there is obstacle in falling edge and adjust for rising edge */
	if (State_flag)
	{
		InfraStatus_Array[_1_INFRA_SENSOR] = DETECTED_OBSTACLE;
		EXI_TriggerEdge(EX_INT0, RISING_EDGE);
		
		State_flag = FALSE;
	}
	/* Detect raising edge as obstacle remove */
	else
	{
		InfraStatus_Array[_1_INFRA_SENSOR] = NO_OBSTACLE;
		EXI_TriggerEdge(EX_INT0, FALLING_EDGE);
		State_flag = TRUE;
	}
}

/* Call back function for external interrupt IR sensor two */
static void Infra_sensor_EX_INT2_CallBack(void)
{
	
	static bool_t State_flag = TRUE;
	/* Detect there is obstacle in falling edge and adjust for rising edge */
	if (State_flag)
	{
		InfraStatus_Array[_2_INFRA_SENSOR] = DETECTED_OBSTACLE;
		EXI_TriggerEdge(EX_INT2, RISING_EDGE);
		
		State_flag = FALSE;
	}
	/* Detect raising edge as obstacle remove */
	else
	{
		InfraStatus_Array[_2_INFRA_SENSOR] = NO_OBSTACLE;
		EXI_TriggerEdge(EX_INT2, FALLING_EDGE);
		State_flag = TRUE;
	}
}