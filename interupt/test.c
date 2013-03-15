#include <pololu/orangutan.h>  

volatile int redReleased = 0;
int redOn = 0;
long msTicks = 0;

ISR(TIMER0_COMPA_vect)
{
	++msTicks;
	if((msTicks % 1000) == 0)
	{
		redReleased = 1;
	}
}

int main()
{
	// Set mode to CTC
	// WGM2 = 0
	// WGM1 = 1
	// WGM0 = 0

	TCCR0A &= ~(0 << WGM00);
	TCCR0A |= (1 << WGM01);
	TCCR0B &= ~(0 << WGM02);

	//Set the scaler to 256
	// CS00 = 0
	// CS01 = 0
	// CS02 = 1

	TCCR0B &= ~(0 << CS00);
	TCCR0B &= ~(0 << CS01);
	TCCR0B |= (1 << CS02);

	//Set the top
	OCR0A = 78;

	// Turn on global interupts
	sei();

	// Turn on interupt for 
	TIMSK0 |= (1 << OCIE0A);
	clear();

	while(1)
	{
		//lcd_goto_xy(0,0);
		//print_long(msTicks);
		if(redReleased)
		{
			redOn ^= 1;
			red_led(redOn);
			green_led(!redOn);
			redReleased = 0;
		}
		
		
	}

	return 0;
}