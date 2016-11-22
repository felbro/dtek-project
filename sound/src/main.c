

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */

//int ADCValue;
//int ADCValue2;
//int ADCAvg;
char str[9];

int re[64];
int im[64];


void convertit(int value) {
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
/*
   void updateValues() {
        ADCValue = ADC1BUF0;
        //ADCValue2 = ADC1BUF1;

        //ADCAvg = (ADCValue + ADCValue2) / 2;
        ADCAvg = ADCValue;

        convertit(ADCAvg);
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
        else if(absolute(ADCAvg - 512) > 70) PORTE |= 0x0001;
   }
 */

unsigned short isqrt(unsigned long a) {
        unsigned long rem = 0;
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

        return (unsigned short) (root >> 1);
}

int main(void) {
        setUp();
        while( 1 )
        {

                int i;
                for (i = 0; i < 64; i++) {
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
                fix_fft(re,im,6);

                for (i = 0; i < 32; i++) {
                        re[i] = isqrt(re[i]*re[i]+im[i]*im[i]);
                }
//                updateValues(); //For the display


        }
        return 0;

}
