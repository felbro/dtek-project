

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
//#include "visuals.c"

#define ITERATIONS 256
#define SW4_ON (PORTD >> 11 & 1)
#define RESET_LED (PORTE &= ~0xff)
#define CONVERSION_DONE ((IFS(1) >> 1) & 1)

char str[9];
int averages[8];

int re[ITERATIONS];
int im[ITERATIONS];

char nm[2] = "a";

void user_isr(){
}


void operationAmpl(){
        RESET_LED;
        IFS(1) &= ~0x0002;

        AD1CON1SET = 0x0004;
        while(!CONVERSION_DONE) ;
        AD1CON1CLR = 0x0004;


        convertit(ADC1BUF0 - 512,str);
        display_string(1,str);
        display_update();
}


void operationFreq() {
        unsigned int i;
        for (i = 0; i < ITERATIONS; i++) {
                //                ADCValue = 0;
                //                ADCValue2 = 0;

                IFS(1) &= ~0x0002;

                AD1CON1SET = 0x0004;
                while(!CONVERSION_DONE) ;
                AD1CON1CLR = 0x0004;

                re[i] = ADC1BUF0 - 512;
                im[i] = 0;

        }

        //HÄR SKA VI KANSKE PAUSA SAMPLING

        fix_fft(re,im,8);

        for (i = 0; i < ITERATIONS/2; i++) {
                re[i] = isqrt(re[i]*re[i]+im[i]*im[i]);
        }
        //updateValues();   //For the display


        //DISPLAY
        //calcAverages(16,averages,re); // for entire 5khz
        /*calcAverages(6,averages,re); // for 0-1.3khz

           RESET_LED;
           char yeh = findmaxAvg(averages);
           //if(re[yeh] > 9)
           PORTE |= 1 << (7-yeh);

           convertit(re[yeh],str);
           display_string(1,str);
           display_update();*/
        graph(re);

}


int main(void) {
        setUp();
        while( 1 )
        {
                if (SW4_ON) {
                        operationFreq();
                }
                else {
                        operationAmpl();
                }

                //delay(100);
        }



        return 0;

}


/*
   TODO

   ändra till signed integer.

 */
