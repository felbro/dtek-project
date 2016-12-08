/* visuals.c
 *   Written 2016 by Felix Brober and Julia Duong
 *
 *   For copyright and licensing, see file COPYING
 *
 * Visual part of the program that takes values from main.c
 * and displays the correct representation on the IO-shield.
 * Each function will iterate through the colums on the
 * IO-shield and set the pixels (bits) that are to be lit to 0.
 * All other pixels (bits) will be set to 1.
 */


#include <stdint.h>   /* Declarations of uint_32 and the like */

/**
 * Array representing a quarter of the screen. All elements
 * set to 255 (black) in the beginning.
 **/
uint8_t square[] = {
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
};

/**
 * Called on by graphFreqorWave. If it is to display the amplitude-frequency
 * relation, the function will light all pixels in the given colum up to a given height.
 * If the function is to display the amplitude-time relation, a single dot at a given height
 * is illuminated.
 *
 * @param col     Which colum to display on.
 * @param height  The distance from the bottom to the top point.
 * @param freq    1 if amplitude-frequency relation. 0 if amplitude-time relation.
 **/
void drawFreqOrWave(uint8_t col, short height, const char freq) {
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

/**
 * Called on by graphAmp. Displays two dots at a provided column at
 * a distance "height" from the y-axis center of the IO-shield.
 *
 * @param col     Which colum to display on.
 * @param height  The distance from the middle to the top point.
 *                If set to <0, the function will display dots at
 *                the middle of the y-axis.
 **/
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

/**
 * Iterates through the given array and calls on drawFreqOrWave with the stored values.
 * If the function is to show the amplitude-frequency relation, the "freq"
 * parameter is set to 1. If the "freq" parameter is set to 0, the amplitude-time
 * relation is shown.
 *
 * @param *arr  Pointer to an array with the stored values.
 * @param freq  1 if amplitude-frequency relation. 0 if amplitude-time relation.
 * @param size  Array size.
 **/
void graphFreqorWave(const int *arr, const char freq, const uint8_t size) {
        uint8_t i;
        for (i = freq; i < size; i++) {
                if (i % 32 == 0)
                        display_image(i-32,square);
                drawFreqOrWave(i,arr[i],freq);
        }
        display_image(96,square);
}

/**
 * Calculates y-axis values for each column based on
 * the maximum sampled amplitude and a provided function.
 * The provided function is set to y = sqrt(r^2-(x/3)^2) (e.g. an oval).
 * Calls on drawDots to display the two dots that form when the calculated
 * y-value is reflected through the x-axis.
 *
 * @param val The maximum sampled amplitude.
 **/
void graphAmp(const int val){
        int r2 = (val / 32) * (val / 32);
        int amp;
        uint8_t i;
        for (i = 0; i < 128; i++) {
                if(i % 32 == 0 && i != 0) {
                        display_image(i-32, square);
                }
                amp = r2 - (i-64)*(i-64)/9;
                if (amp > 0) {
                        drawDots(i, isqrt32(amp));
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
