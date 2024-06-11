/*
 ============================================================================
 Name        : StopWatch.c
 Author      : Ahmed Mahmoud Hafny
 Version     : 1.0
 Description : StopWatch Project with the requirments in this file :
               https://drive.google.com/file/d/1b0lCdG910Qn6zHk4XqWH9-uJD_EuPKo-/view?usp=sharing
 ============================================================================
 */
/********************************      Header Files              *****************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* #define DP_PIN PB0 */     /*  uncomment if you want to display the dot */


/********************************    Global Variables              *****************************************/
/*  store time and state */
volatile uint16_t milliseconds = 0;
volatile uint8_t seconds = 0, minutes = 0, hours = 0;
volatile uint8_t running = 1;

/********************************      Functions Prototypes Section     *****************************************/
void timer1_init();              /* Function to initialize Timer1 */
void ext_interrupts_init();      /* Function to initialize external interrupts */
void ports_init();               /* Function to initialize ports*/
void display_time();             /* Function to display the time on 7-segment displays*/



/********************************      Main Section     *****************************************/
int main(void) {

	/* Initialize timer, external interrupts, and ports*/
	timer1_init();
	ext_interrupts_init();
	ports_init();

	/* Enable global interrupts*/
	sei();

	while (1) {
		display_time();   /* Always display the time on 7-segment displays */
	    }

	return 0;
}


/********************************      Functions Section     *****************************************/
/******* Function to initialize Timer1 ***************/
void timer1_init() {
	/* Set CTC mode with OCR1A as top & Set prescaler to 64 and start the timer*/
	TCCR1B |= (1 << WGM12)| (1 << CS11) | (1 << CS10);

	/* Set OCR1A value for ## 1ms interrupt ##   (1MHz (Fcpu) / 64 (prescaler) = 15625 , 15625 (for 1 sec) / 1000 = 15.625) */
	OCR1A = 15;

	/* Enable Output Compare A Match Interrupt (Timer Interrupt Enable) */
	TIMSK |= (1 << OCIE1A);
}

/********** Function to initialize external interrupts **********/
void ext_interrupts_init() {
	/* INT0 - falling edge trigger*/
	MCUCR |= (1 << ISC01);
	MCUCR &= ~(1 << ISC00);
	/* INT1 - rising edge trigger*/
	MCUCR |= (1 << ISC10) | (1 << ISC11);
	/* INT2 - falling edge trigger */
	MCUCSR &= ~(1 << ISC2);
	/* Enable external interrupts INT0, INT1, INT2*/
	GICR |= (1 << INT0) | (1 << INT1) | (1 << INT2);
}

/********** Function to initialize ports **********/
void ports_init() {
	/* Set PORTC (PC0-PC3) as output for 7447 decoder inputs*/
	DDRC |= 0x0F;
	/* Set PORTA (PA0-PA7) as output for enabling/disabling 7-segment displays*/
	DDRA |= 0xFF;
	/* Enable pull-ups for PORTD pins (INT0, INT1, INT2)*/
	PORTD |= (1 << PD2) | (1 << PD3);
	PORTB |= (1 << PB2);

	/*  uncomment if you want to display the dot */
   /* DDRB |= (1 << DP_PIN); */

}

/********** Timer1 compare match interrupt service routine **********/
ISR(TIMER1_COMPA_vect) {
	if (running) {

		milliseconds++;
		if (milliseconds >= 1000) {
			milliseconds = 0;
			seconds++;
			if (seconds >= 60) {
				seconds = 0;
				minutes++;
				if (minutes >= 60) {
					minutes = 0;
					hours++;
					if (hours >= 24) {
						hours = 0;
					}
				}
			}
		}
	}
 }

/********** External interrupt 0 service routine (Reset_Count) **********/
ISR(INT0_vect) {

	milliseconds = 0;
	seconds = 0;
	minutes = 0;
	hours = 0;
	running = 0;   /*stop the clock */

}

/********** External interrupt 1 service routine (Pause) **********/
ISR(INT1_vect) {
	running = 0;    /*stop the clock */
}

/********** External interrupt 2 service routine (Resume) **********/
ISR(INT2_vect) {
	running = 1;   /*start the clock */
}

/********** Function to display the time on 7-segment displays **********/
void display_time() {

	/* Array to store digits*/
	uint8_t digits[8];
	digits[0] = milliseconds % 100 / 10;
	digits[1] = milliseconds / 100;
	digits[2] = seconds % 10;
	digits[3] = seconds / 10;
	digits[4] = minutes % 10;
	digits[5] = minutes / 10;
	digits[6] = hours % 10;
	digits[7] = hours / 10;

	/* Display digits on 7-segment displays*/
	uint8_t i ;
	for (i = 0; i < 8; i++) {
		/* Set PORTA to enable the current 7-segment display*/
		PORTA = (1 << i);
		/* Set PORTC to display the current digit*/
		PORTC = (PORTC & 0xF0) | (digits[i] & 0x0F);

               /*  uncomment the follwing if you want to display the dot */
		/*
		   // Control the DP segment
		        if (i == 7 || i == 5|| i == 3|| i == 1|| i == 0) {
		            PORTB |= (1 << DP_PIN);
		        } else {
		            PORTB &= ~(1 << DP_PIN);
		        }
		         */
		/* Small delay for multiplexing (1ms) */
		_delay_ms(0.1);
	}
}
