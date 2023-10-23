


#ifndef RC_CAR_PRIVATE_H_
#define RC_CAR_PRIVATE_H_

#define THERE_OBSTACLE		LOW
#define THERE_NO_OBSTACLE	HIGH



/* Operate all car motors in CW direction */
static void MoveForward(void);
/* Operate all car motors in CCW direction */
static void MoveBackword(void);
/* Operate two right car motors in CW direction 
 * Operate two left car motors in CCW direction */
static void MoveRight(void);
/* Operate two right car motors in CCW direction 
 * Operate two left car motors in CW direction */
static void MoveLeft(void);
/* Stop all car motors */
static void CarStop(void);

/* Control RC car from order received from Bluetooth */
static void Bluetooth_Control(void);
/* Car run free mode and try to avoid obstacles user can stop and start car operation only */
static void FreeRun(void);
/* Car follow line the ground */
static void LineFollower(void);
/* Car find its way outside the maze */
static void MazeDrive(void);
/* Data received from diagnostic service */
static void Diagnostic_Data(void);






#endif /* RC_CAR_PRIVATE_H_ */