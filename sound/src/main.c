

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

void updateValues() {

        // DISPLAY AVERAGES ETC. DOES NOT WORK WITHOUT OTHER CODE ADDED
        //ADCValue = ADC1BUF0;
        //ADCValue2 = ADC1BUF1;

        //ADCAvg = (ADCValue + ADCValue2) / 2;
        //ADCAvg = ADCValue;

        /*convertit(ADCAvg);
           display_string( 3, str);
           display_update();
           PORTE &= ~0xff;
           if(absolute(ADCAvg - 512) > 300) PORTE |= 0x00ff;
           else if(absolute(ADCAvg - 512) > 265) PORTE |= 0x007f;
           else if(absolute(ADCAvg - 512) > 230) PORTE |= 0x003f;
           else if(absolute(ADCAvg - 512) > 190) PORTE |= 0x001f;
           else if(absolute(ADCAvg - 512) > 160) PORTE |= 0x000f;
           else if(absolute(ADCAvg - 512) > 130) PORTE |= 0x0007;
           else if(absolute(ADCAvg - 512) > 100) PORTE |= 0x0003;
           else if(absolute(ADCAvg - 512) > 70) PORTE |= 0x0001;*/

//DISPLAY SOME OF THE RE VALUES
        /*convertit(re[5]);

           display_string(1,str);
           convertit(re[13]);
           display_string(2,str);
           convertit(re[25]);
           display_string(3,str);
           display_update();*/

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


void user_isr(){
        /*if((IFS(0) >> 19 & 1) == 1) {
                IFS(0) &= ~0x80000;
                display_string(3,nm);
                display_update();
                nm[0]++;
           }*/
}

/*
   TODO
   onädigt: ÄNDRA FRÅN AUTOMATICALLY START SAMPLING AFTER CONVERSION TILL MANUELLT. ELLER KANSKE ÄNNU BÄTTRE:
   PAUSA EFTER 64 ST SAMPLES HAR SAMPLATS OCH KONVERTERATS.

   ändra till signed integer.

   GÖR 2 STATES. IFALL SWITCH IS ON, VISA FFT THINGY. ANNARS amplitude.

 */
void checkSwitch() {
        if ((PORTD >> 11 & 1) == 1) {
                display_string(3,"On");
        }
        else display_string(3,"Off");
        display_update();
}


int main(void) {
        setUp();
        while( 1 )
        {
                checkSwitch();
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
                updateValues();         //For the display


                //delay(100);
        }
        return 0;

}
