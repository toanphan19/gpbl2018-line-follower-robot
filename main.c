//
// Notes
// 1. Left & right motor run time ratio for straight: 4 : 5 
// (left motor is stronger))
// 

#include <xc.h>
#include <p18f2553.h>

wait00(short k)
{ 	 
/*Wait time about (~ k * 0.01 sec.) */ 		 
    short i; 		 
    short j; /* Declaration of 16 bit variables 	*/ 	 
    for(j=0;j<k;j++){ /*(~ k * 3000) times iteration */ 	 
		for(i=0;i<3000;i++){ 	 
		}	 
	} 		 
} 	

int led_sens(void)
{
PORTAbits.RA0 = PORTBbits.RB0;          
PORTAbits.RA1 = PORTBbits.RB1;   
PORTAbits.RA2 = PORTBbits.RB2; 
PORTAbits.RA3 = PORTBbits.RB3;   
PORTAbits.RA4 = PORTBbits.RB4;      
}

//
// States
//
enum road_state_codes {
    on_track, off_track,
    straight_line, // for accel
    off_right_little, off_right,
    off_left_little, off_left
};


enum road_state_codes getRoadState(short a, short b, short c, short d, short e) {
    if (    (a == 1 && b == 1 && c == 1
          && d == 1 && e == 1)
         || (a == 0 && b == 0 && c == 0
          && d == 0 && e == 0)) {
        return off_track;
    }
    
     if (  (c == 0
         && a == 1 && b == 1
         && d == 1 && e == 1)
         || (c == 1
         && a == 0 && b == 0
         && d == 0 && e == 0)) {
         return straight_line;
     } 
     
     // White Background
     if (a == 1 && b == 0 // && c == 1
             && d == 1 && e == 1) {
         return off_right_little; 
     } 
     if (a == 1 && b == 1 // && c == 1
             && d == 0 && e == 1) {
         return off_left_little;
     } 
     if (a == 0 && c == 1 && e == 1) {
        return off_right;
     } 
     if (a == 1 && c == 1  && e == 0) {
         return off_left;
     }
     
     // Black Background
     if (a == 0 && b == 1 // && c == 0
             && d == 0 && e == 0) {
         return off_right_little; 
     } 
     if (a == 0 && b == 0 // && c == 0
             && d == 1 && e == 0) {
         return off_left_little;
     } 
     if (a == 1 && c == 0 && e == 0) {
        return off_right;
     } 
     if (a == 0 && c == 0 && e == 1) {
         return off_left;
     }
     
     return on_track;
}

//
// Turning
// - Note: the car used has better left motor, so right motor has to run more
// 

int turnLeft(void) {
    PORTC=0x02; /* right motor on */
    wait00(7);
    PORTC = 0x00;
    wait00(5);
}

int turnRight(void) {
    PORTC=0x01;/* left motor on */
    wait00(6);
    PORTC = 0x00;
    wait00(5);
}


int turnLeftWide(void) {
    
    PORTC=0x03; // both motor on
    wait00(1);
    PORTC=0x02; // right motor on
    wait00(10);
    PORTC = 0x00;
    wait00(5);
    /*
    PORTC=0x02; // right motor on
    wait00(6);
    PORTC = 0x00;
    wait00(10);
    */
}

int turnRightWide(void) {
    
    PORTC=0x03; // both motor on
    wait00(1);
    PORTC=0x01; // left motor on
    wait00(8.5);
    PORTC = 0x00;
    wait00(5);
    /*
    PORTC=0x01; // left motor on
    wait00(5);
    PORTC = 0x00;
    wait00(10);
    */
}


int straight(void){
    PORTC=0x03; /* both motor on */
    wait00(5); 
    PORTC=0x00; /* both motor off */
    wait00(8); 
}

int accelerate(void) {
    PORTC=0x03; /* both motor on */
    wait00(12);
    PORTC=0x02; /* right motor on */
    wait00(1);
    PORTC=0x00; /* both motor off */
    wait00(2); 
}


main(void)
{
    //setting digital or analog pin
    ADCON1 = 0x0F;

    //setting input or output
    TRISA = 0x20;            //  portA   RA0,1,2,3,4:output, RA5:input   10 0000
    TRISB = 0xFF;            //  portB   RB all input   1111 1111
    TRISC = 0x00;            //  portC   RC all output
    
    //reset to port
    PORTA = 0x00;           //  portA   reset output
    PORTB = 0x00;           //  portB    reset output
    PORTC = 0x00;           //  portC    reset output
    
    //Start SW
    while(PORTAbits.RA5==1){
        led_sens();
    }
    
    enum road_state_codes road_state = on_track;
    enum road_state_codes previous_road_state;
    
    //Push Start SW to start
    while (1) {
        led_sens();
        
        previous_road_state = road_state;
        road_state = getRoadState(
            PORTBbits.RB0, PORTBbits.RB1, PORTBbits.RB2,
            PORTBbits.RB3, PORTBbits.RB4
        );
        
        if (road_state == off_track)
            road_state = previous_road_state;
        
        switch (road_state) {
            case straight_line:
                accelerate();
                break;
            case off_left_little:
                turnRightWide();
                break;
            case off_right_little:
                turnLeftWide();
                break;
            case off_left:
                turnRight();
                break;
            case off_right:
                turnLeft();
                break;
            case on_track:
                straight();
                break;
        }
        
    }
            
}
