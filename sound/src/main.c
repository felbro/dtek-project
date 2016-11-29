

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */

#define ITERATIONS 256
#define SW4_ON (PORTD >> 11 & 1)
#define BTN4_ON (PORTD >> 7 & 0x1)
#define BTN3_ON (PORTD >> 6 & 0x1)
#define CONVERSION_DONE ((IFS(1) >> 1) & 1)

char str[9];

int re[ITERATIONS];
int im[ITERATIONS];


void user_isr(){
}


void operationAmpl(){
        //RESET_LED;
        short maxVal = 0;
        uint8_t i;
        for (i = 0; i < ITERATIONS-1; i++) {
                IFS(1) &= ~0x0002;

                AD1CON1SET = 0x0004;
                while(!CONVERSION_DONE) ;
                AD1CON1CLR = 0x0004;

                if(ADC1BUF0 > maxVal) maxVal = ADC1BUF0;
        }

        graphAmp(maxVal-512);
}


void operationFreq() {
        unsigned int i;
        for (i = 0; i < ITERATIONS; i++) {
                IFS(1) &= ~0x0002;

                AD1CON1SET = 0x0004;
                while(!CONVERSION_DONE) ;
                AD1CON1CLR = 0x0004;

                re[i] = ADC1BUF0 - 512;
                im[i] = 0;

        }

        fix_fft(re,im,8);


        for (i = 0; i < ITERATIONS/2; i++) {
                re[i] = isqrt(re[i]*re[i]+im[i]*im[i]);
        }
        graphFreq(re);
}


int main(void) {
        setUp();
        while( 1 )
        {
                if (BTN4_ON) {
                        AD1CON3 = 0x019a;
                }
                else if (BTN3_ON) {
                        AD1CON3 = 0x04fa;
                }

                if (SW4_ON) {
                        operationFreq();
                }
                else {
                        operationAmpl();
                }

        }

        return 0;

}


/*
   TODO

   ändra till signed integer.

 */
