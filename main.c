

#include <xc.h>
#include <p18f2553.h>

float kp = 20;
float ki = 0;
float kd = 0;

float P = 0,
        I = 0,
        D = 0;

int error = 0,
        previous_error = 0;
float pid = 0;


wait00(float k)
{ 	 
/*Wait time about (~ k * 1 ms.) */ 		
    float i; 		 
    float j; 	 
    for(j=0;j < k;j++){ /*(~ k * 300) times iteration */ 	 
		for(i=0;i<300;i++){ 	 
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


float abs(float a) {
    return (a > 0) ? a : -a;
}


float getPID() {
    P = error;
    I = I + error;
    D = error - previous_error;
    previous_error = error;
    
    return ((kp*P) + (ki*I) + (kd*D));
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
    
    //Push Start SW to start
    while (1) {
        led_sens();
        
        error = 0;
        if(PORTBbits.RB0==0  &&  PORTBbits.RB1==1 &&  PORTBbits.RB2==1 && PORTBbits.RB3==1 && PORTBbits.RB4==1){ 
           /*White White White White black*/
            error = -3;
        }
        else if (PORTBbits.RB0==0  &&  PORTBbits.RB1==0 &&  PORTBbits.RB2==1 && PORTBbits.RB3==1 && PORTBbits.RB4==1){
            error = -2;
        }
        else if (PORTBbits.RB0==1  &&  PORTBbits.RB1==0 &&  PORTBbits.RB2==1 && PORTBbits.RB3==1 && PORTBbits.RB4==1){
            error = -1;
        }
        
        //right
        else if (PORTBbits.RB0==1  &&  PORTBbits.RB1==1 &&  PORTBbits.RB2==1 && PORTBbits.RB3==1 && PORTBbits.RB4==0){
            error = 3;
        }
        else if (PORTBbits.RB0==1  &&  PORTBbits.RB1==1 &&  PORTBbits.RB2==1 && PORTBbits.RB3==0 && PORTBbits.RB4==0){
            error = 2;
        }
        else if (PORTBbits.RB0==1  &&  PORTBbits.RB1==1 &&  PORTBbits.RB2==1 && PORTBbits.RB3==0 && PORTBbits.RB4==1){
            error = 1;
        }
        
        // white line
        //left
        else if (PORTBbits.RB0==1  &&  PORTBbits.RB1==0 &&  PORTBbits.RB2==0 && PORTBbits.RB3==0 && PORTBbits.RB4==0){
            error = -3;
        }
        else if (PORTBbits.RB0==1  &&  PORTBbits.RB1==1 &&  PORTBbits.RB2==0 && PORTBbits.RB3==0 && PORTBbits.RB4==0){
            error = -2;
        }
        else if (PORTBbits.RB0==0  &&  PORTBbits.RB1==1 &&  PORTBbits.RB2==0 && PORTBbits.RB3==0 && PORTBbits.RB4==0){
            error = -1;
        }
        
        //right
        else if (PORTBbits.RB0==0  &&  PORTBbits.RB1==0 &&  PORTBbits.RB2==0 && PORTBbits.RB3==0 && PORTBbits.RB4==1){
            error = 3;
        }
        else if (PORTBbits.RB0==0  &&  PORTBbits.RB1==0 &&  PORTBbits.RB2==0 && PORTBbits.RB3==1 && PORTBbits.RB4==1){
            error = 2;
        }
        else if (PORTBbits.RB0==0  &&  PORTBbits.RB1==0 &&  PORTBbits.RB2==0 && PORTBbits.RB3==1 && PORTBbits.RB4==0){
            error = 1;
        }
        
        
        pid = getPID();
        
        PORTC=0x03; /* both motor on */
        wait00(30);
        PORTC=0x01; /* left motor on */
        wait00(0 + pid);
        PORTC=0x02; /* right motor on */
        wait00(0 );
        PORTC=0x00; /* both motor off */
        wait00(20); 
    }
    
    return 0;
}
