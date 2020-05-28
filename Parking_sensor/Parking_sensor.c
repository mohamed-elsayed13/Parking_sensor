/*
 * Parking_sensor.c
 *
 * Created: 19/05/2020 12:42:56 AM
 *  Author: M
 */ 

#include "LCD.h"
#include "Timer0.h"
void timer_on();
void timer_off();
void sensor_trig();
void get_display_distance();
uint16_t  total_time=0 , distance=0 ;
uint8_t timer_over_flow=0; 
uint8_t grd=0;
int main(void)
{
	CLRBIT(MCUCSR,ISC2); // falling edge 
	sei();				// global interrupt
	LCD_init();
	timer_normal_init();
	SETBIT(DDRC,2);
    SETBIT(DDRC,5);
    while(1)
    {
		timer_on();
		sensor_trig();
			
		if(distance<=400&&distance>10){
			CLRBIT(PORTC,5);
				} 
		if(distance<=10&&distance>=0){
			SETBIT(PORTC,5);	
				}
		
		 _delay_ms(250);    
		     }
}
ISR(INT2_vect){
	CLRBIT(GICR,INT2);
	total_time=TCNT0*1024.0 /16.0;
	total_time=total_time+timer_over_flow*(16230);   // 16230us = 255*1024/16 refers to the ovf_value_for_timer0
	timer_off();
	timer_over_flow=0;
	get_display_distance();
		}
ISR(TIMER0_OVF_vect){
	timer_over_flow++;
	}
void timer_on(){
	TCNT0=0;
	SETBIT(TCCR0,CS02);
	SETBIT(TCCR0,CS00);
	SETBIT(GICR,INT2);
}
void timer_off(){
	CLRBIT(TCCR0,CS02);
	CLRBIT(TCCR0,CS00);	
}
void get_display_distance(){
	distance=0.01715*total_time;		// 2x=34300*t & to get time in seconds we multiplied by 10^-6
	distance=distance-10;
	LCD_write_command(0x1);
	LCD_write_string("distance=");
	LCD_write_num(distance);
	LCD_write_string("cm");
	if (distance<=10){
	LCD_write_command(0xc0);
	LCD_write_string("Danger!!!");
	}
	}
void sensor_trig(){
		SETBIT(PORTC,2);
		_delay_us(10); // ultrasonic data sheet said 10 us pulse will be good for trigger the sensor
		CLRBIT(PORTC,2);
		/*
		c.c = (1/16Mhz)
		time t represent time of starting wave of ultrasonic sensor 
		TCNT0 multiplied by 1023 the brescaler and c.c to know the real time
		*/
}