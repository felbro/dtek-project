

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */

//int ADCValue;
//int ADCValue2;
//int ADCAvg;

#define ITERATIONS 256

char str[9];
int averages[8];

int re[ITERATIONS];
int im[ITERATIONS];

char nm[2] = "a";

void user_isr(){
}


void operationAmpl(){
        PORTE &= ~0xff;
        IFS(1) &= ~0x0002;

        AD1CON1SET = 0x0004;
        while(((IFS(1) >> 1) & 1) != 1) ;
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
                while(((IFS(1) >> 1) & 1) != 1) ;
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
        calcAverages(16,averages,re);
        //calcAverages(3,averages,re);

        PORTE &= ~0xff;
        char yeh = findmaxAvg(averages);
        //if(re[yeh] > 9)
        PORTE |= 1 << (7-yeh);

        convertit(re[yeh],str);
        display_string(1,str);
        display_update();

}


int main(void) {
        setUp();
        while( 1 )
        {
                if ((PORTD >> 11 & 1) == 1) {
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
