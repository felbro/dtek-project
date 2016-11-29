#include <stdint.h>   /* Declarations of uint_32 and the like */


uint8_t square[] = {
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
};

void drawCol(uint8_t col, uint8_t height) {
        char i;
        height /= 6;
        if(height >= 32) height = 31;
        col %= 32;

        for(i = 3; i >= 0; i--) {
                square[i*32+col] = 0b11111111 >> height;
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

void graphFreq(int *arr) {

        uint8_t i;
        for (i = 1; i < 128; i++) {
                if (i % 32 == 0)
                        display_image(i-32,square);
                drawCol(i,arr[i]);
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
