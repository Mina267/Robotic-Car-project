

#ifndef RC_CAR_H_
#define RC_CAR_H_


/* Initialization of robot car system
 * Take default system mode you want to start 
 * initialize IR sensor & frame diagnostic */  
void RC_Car_Init(CarControl_Mode Mode);
/* Running of machine state */
void RC_Car_Runnable(void);



#endif /* RC_CAR_H_ */