/* visuals.c
   Written 2016 by Felix Brober and Julia Duong

   For copyright and licensing, see file COPYING
 */


#include <stdint.h>   /* Declarations of uint_32 and the like */

uint8_t square[] = {
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
};

void drawFreqOrWave(uint8_t col, short height, char freq) {
        char i;
        col %= 32;
        if (freq) {
                height /= 6;
                if(height >= 32) height = 31;
        }
        else {
                height /= 32;
        }
        for(i = 3; i >= 0; i--) {
                if(freq)
                        square[i*32+col] = 0b11111111 >> height;
                else
                        square[i*32+col] = 0b11111111 & ((~(0x100 >> height)) & 0xff);
                if(height >= 8) height -= 8;
                else height = 0;
        }

}

void drawDots(uint8_t col, char height) {
        col %= 32;
        if (height < 0) {
                square[32+col] = 0b01111111;
                square[64+col] = 0b11111110;
                return;
        }
        char i;
        for (i = 1; i >= 0; i--) {
                square[i*32+col] = 0xff & (~(0x100 >> height) & 0xff);
                square[(96-i*32)+col] = 0xff & (~((0x01 << height) >> 1) & 0xff);
                if(height >= 8) height -= 8;
                else height = 0;
        }
}

// Plot freq if freq = 1
void graphFreqorWave(int *arr, char freq) {

        uint8_t i;
        for (i = 1; i < 128; i++) {
                drawFreqOrWave(i,arr[i],freq);
                if (i % 32 == 0)
                        display_image(i-32,square);
        }
        display_image(96,square);
}


void graphAmp(int val){
        //vi får in värde mellan -512 - 511
        int r2 = (val / 32) * (val / 32);
        int amp;
        uint8_t i;
        for (i = 0; i < 128; i++) {
                if(i % 32 == 0 && i != 0) {
                        display_image(i-32, square);
                }
                amp = r2 - (i-64)*(i-64)/9;
                if (amp > 0) {
                        drawDots(i, isqrt(amp));
                }
                else {
                        drawDots(i, 0);
                }
                if (amp == 0) {
                        drawDots(i,-1);
                }
        }
        display_image(96, square);
}
