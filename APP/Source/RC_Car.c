#include "StdTypes.h"

#include "Diagnostic_Frame.h"


#include "MOTOR_interface.h"
#include "LCD_interface.h"
#include "InfraredSensor.h"
#include "ULTRASONIC.h"

#include "RC_Car.h"
#include "RC_Car_Private.h"
#include "RC_Car_Cfg.h"
#include "DIO_interface.h"
#include "Kernel_Interface.h"

# define F_CPU 8000000
#include <util/delay.h>


static RecFrameStatus_t FrameStatus = NO_REQ;
static CarControl_Mode Car_Mode;



void RC_Car_Init(CarControl_Mode Mode)
{
	Car_Mode = Mode;
	
	Frame_Diag_Init();
	ULTRASONICInt_Init();
	
	/*RTOS_Error_typeCreateTask(5, RC_Car_Runnable, 40, 7);
	RTOS_Error_typeCreateTask(1, Dia_frame, 40, 1);
	
	RTOS_voidStart();
	*/
}


void RC_Car_Runnable(void)
{	
	/* Take data order receive to executed */ 
	Diagnostic_Data();
	
	/* Operate according to machine state */
	switch (Car_Mode)
	{
		case BLUETOOTH_CONTROL_MODE:
		Bluetooth_Control();
		break;
		
		case FREE_RUN_MODE:
		FreeRun();
		break;
		
		case LINE_FOLLOWER_MODE:
		LineFollower();
		break;
		
		case MAZE_MODE:
		MazeDrive();
		break;
	}
}



/* Control RC car from order received from Bluetooth */
static void Bluetooth_Control(void)
{	
	/* Continuous move will receive frame from BCM neither that stop 
	 * Check for request receive for direction of car will move */
	if (FrameStatus  == BLU_FOWARD_DIR_REQ)
	{
		/* Operate car motor to move in forward direction */
		MoveForward();
	}
	else if (FrameStatus  == BLU_BACKWARD_DIR_REQ)
	{
		/* Operate car motor to move in backward direction */
		MoveBackword();
	}
	else if (FrameStatus  == BLU_RIGHT_DIR_REQ)
	{
		/* Operate car motor to move in right direction */
		MoveRight();
	}
	else if (FrameStatus  == BLU_LEFT_DIR_REQ)
	{
		/* Operate car motor to move in left direction */
		MoveLeft();
	}
	else if (FrameStatus  == BLU_STOP_REQ)
	{
		CarStop();
	}
	/* If there is no frame received stop car */
	else
	{
		//CarStop();
		
	}
	
	
}


/* Car run free mode and try to avoid obstacles user can stop and start car operation only */
static void FreeRun(void)
{
	static bool_t Start_flag = FALSE;
	/* Read status of IR sensor read */
	DIO_PinVoltage_type Left_IR_Status = DIO_ReadPin(LEFT_IR_SENSOR);
	DIO_PinVoltage_type Right_IR_Status = DIO_ReadPin(RIGHT_IR_SENSOR);
	
	/* Read  Distance from Ultrasonic sensor */
	u16 distance;
	ULTRASONIC_StartRead(US1);
	ULTRASONIC_Distance_t Status = ULTRASONIC_DistanceGetterCheck(&distance);
	
	
	/* If new read receive from Ultrasonic process */
	if (Status == ULTRA_NEW_DISTANCE)
	{
		/* there is no OBSTACLE in front of the car */
		if (distance > MIN_DIS_TO_STOP)
		{
			MoveForward();
			Start_flag = TRUE;
			_delay_ms(10);
		}
		/* there is OBSTACLE in front of the car */
		else if (Start_flag)
		{
			
			CarStop();
			_delay_ms(1500);
			/* see the left IR if no obstacle move backward and return left */
			if (Left_IR_Status == THERE_NO_OBSTACLE)
			{
				MoveBackword();
				_delay_ms(500);
				MoveLeft();
				_delay_ms(500);
			}
			/* see the right IR if no obstacle move backward and return right */
			else if (Right_IR_Status == THERE_NO_OBSTACLE)
			{
				MoveBackword();
				_delay_ms(500);
				MoveLeft();
				_delay_ms(500);
			}
			/* Obstacle in both ways return 180 degree */
			else
			{
				MoveLeft();
				_delay_ms(1000);
			}
		}
	}
	
}




/* Car follow line the ground */
static void LineFollower(void)
{
	static bool_t StartLine_flag = FALSE;
	/* Read status of IR sensor read */
	
	
	DIO_PinVoltage_type Left_IR_Status = DIO_ReadPin(LEFT_IR_SENSOR);
	DIO_PinVoltage_type Right_IR_Status = DIO_ReadPin(RIGHT_IR_SENSOR);
	if (StartLine_flag)
	{
		/* Detect End of trace*/
		if (Left_IR_Status == THERE_NO_OBSTACLE && Right_IR_Status == THERE_NO_OBSTACLE)
		{
			CarStop();
		}
		
		/* Car move in correct path move forward */
		else if (Left_IR_Status == THERE_OBSTACLE && Right_IR_Status == THERE_OBSTACLE)
		{
			MoveForward();
		}
		
		/* Car deviate to right adjust to correct path by moving left */
		else if (Left_IR_Status == THERE_NO_OBSTACLE)
		{
			MoveLeft();
		}
		/* Car deviate to left adjust to correct path by moving right */
		else if (Right_IR_Status == THERE_NO_OBSTACLE)
		{
			MoveRight();
		}
		
		else
		{
			CarStop();
		}
	}
	
	
	/* Control Start and stop of car in line follower. */
	if (FrameStatus == LINE_FOLLOWER_START_REQ)
	{
		StartLine_flag = TRUE;
	}
	else if (FrameStatus == LINE_FOLLOWER_STOP_REQ)
	{
		StartLine_flag = FALSE;
		CarStop();
	}
}

/* Car find its way outside the maze */
static void MazeDrive(void)
{
	
}



/* Operate all car motors in CW direction */
static void MoveForward(void)
{
	MOTOR_CW(RIGHT_FORWARD_MOTOR);
	MOTOR_CW(LEFT_FORWARD_MOTOR);

}

/* Operate all car motors in CCW direction */
static void MoveBackword(void)
{
	MOTOR_CCW(RIGHT_FORWARD_MOTOR);
	MOTOR_CCW(LEFT_FORWARD_MOTOR);

}

/* Operate two right car motors in CW direction 
 * Operate two left car motors in CCW direction */
static void MoveRight(void)
{
	MOTOR_CCW(RIGHT_FORWARD_MOTOR);
	MOTOR_CW(LEFT_FORWARD_MOTOR);

}

/* Operate two right car motors in CCW direction 
 * Operate two left car motors in CW direction */
static void MoveLeft(void)
{
	MOTOR_CW(RIGHT_FORWARD_MOTOR);
	MOTOR_CCW(LEFT_FORWARD_MOTOR);

}

/* Stop all car motors */
static void CarStop(void)
{
	MOTOR_Stop(RIGHT_FORWARD_MOTOR);
	MOTOR_Stop(LEFT_FORWARD_MOTOR);
}






static void Diagnostic_Data(void)
{
	Frame_Diag(&FrameStatus, &Car_Mode);
}
