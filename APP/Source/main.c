#include "StdTypes.h"
#include "MemMap.h"
#include "Utils.h"

#include "RC_Car.h"





int main(void)
{
	
	DIO_Init();
	LCD_Init();

	sei();
	

	RC_Car_Init();
	while (1)
	{
		RC_Car_Runnable();
		
	}
	return 0;
}
























