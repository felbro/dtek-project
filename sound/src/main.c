

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */

//int ADCValue;
//int ADCValue2;
//int ADCAvg;
char str[9];
int averages[8];


int re[64];
int im[64];


void convertit(int value) {
        if(value < 0) value *= -1;
        int i;
        int other = 1;
        for (i = 7; i >= 0; i--) {
                str[i] = (value / other) % 10 + 48;
                other *= 10;
        }
}

int absolute(int val) {
        if (val < 0) return (-1)*val;
        else return val;
}


char findmaxAvg(){
        char temp = 0;
        int maxval = averages[0];
        char curr;
        for (curr = 1; curr < 8; curr++) {
                if (averages[curr] > maxval) temp = curr;
        }
        return temp;
}

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

        averages[0] = (re[1] + re[2] + re[3]) / 3;
        int i;
        for(i = 1; i < 8; i++) {
                averages[i] = (re[i*4] + re[i*4 + 1] +re[i*4 + 2] +re[i*4 + 3]) / 4;
        }
        PORTE &= ~0xff;
        PORTE |= 1 << findmaxAvg();

}


void user_isr(){
        //TESTA (GENOM ATT SKRIVA UT PÅ SKÄRMEN) OCH SE IFALL DENNA KALLAS AV VECTORS.S VID INTERRUPTS
}

unsigned int isqrt(unsigned int a) {
        unsigned int rem = 0;
        int root = 0;
        int i;

        for (i = 0; i < 16; i++) {
                root <<= 1;
                rem <<= 2;
                rem += a >> 30;
                a <<= 2;

                if (root < rem) {
                        root++;
                        rem -= root;
                        root++;
                }
        }

        return (unsigned int) (root >> 1);
}

/*
   TODO
   ÄNDRA FRÅN AUTOMATICALLY START SAMPLING AFTER CONVERSION TILL MANUELLT. ELLER KANSKE ÄNNU BÄTTRE:
   PAUSA EFTER 64 ST SAMPLES HAR SAMPLATS OCH KONVERTERATS.

 */


int main(void) {
        setUp();
        while( 1 )
        {

                unsigned int i;
                for (i = 0; i < 64; i++) {
                        //                ADCValue = 0;
                        //                ADCValue2 = 0;

                        IFS(1) &= ~0x0002;

                        AD1CON1SET = 0x0004;
                        while(((IFS(1) >> 1) & 1) != 1) ;
                        AD1CON1CLR = 0x0004;

                        re[i] = ADC1BUF0 - 512;
                        //re[i] = ADCAvg - 512;
                        im[i] = 0;

                }

                //HÄR SKA VI KANSKE PAUSA SAMPLING

                fix_fft(re,im,6);

                for (i = 0; i < 32; i++) {
                        re[i] = isqrt(re[i]*re[i]+im[i]*im[i]);
                }
                updateValues();         //For the display


                //delay(100);
        }
        return 0;

}
