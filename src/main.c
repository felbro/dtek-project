/* main.c
   Written 2016 by Felix Brober and Julia Duong

   For copyright and licensing, see file COPYING
 */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */

#define ITERATIONS 256
#define SW4_ON (PORTD >> 11 & 1)
#define SW3_ON (PORTD >> 10 & 1)
#define BTN4_ON (PORTD >> 7 & 0x1)
#define BTN3_ON (PORTD >> 6 & 0x1)
#define CONVERSION_DONE ((IFS(1) >> 1) & 1)

int re[ITERATIONS];
int im[ITERATIONS];
int wave[ITERATIONS/2];


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

void operationWave() {
        uint8_t i,j;
        int val;
        for (i = 0; i < ITERATIONS/2; i++) {
                val = 0;
                for (j = 0; j < 2; j++) {
                        IFS(1) &= ~0x0002;

                        AD1CON1SET = 0x0004;
                        while(!CONVERSION_DONE) ;
                        AD1CON1CLR = 0x0004;
                        val += ADC1BUF0;
                }
                wave[i] = val / 2;
        }
        graphFreqorWave(wave,0);
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
        graphFreqorWave(re,1);
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

                if(SW3_ON) {
                        operationAmpl();
                }
                else if (SW4_ON) {
                        operationFreq();
                }
                else {
                        operationWave();
                }

        }

        return 0;

}


/*
   TODO

   ändra till signed integer.

 */
