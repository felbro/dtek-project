/* main.c
 *   Written 2016 by Felix Brober and Julia Duong
 *
 *   For copyright and licensing, see file COPYING
 *
 * Main part of the program that samples values and calls
 * on visual parts to display on the IO-shield. The sampling
 * frequency is, during setup, set to 10 kHz (which means that
 * the amplitude-frequency plot can show 5kHz) but can be changed
 * to 20kHz by pressing button 4. Changing switches will affect
 * what's displayed on the IO-shield with the options:
 * amplitude-frequency, amplitude-time and a visual representation
 * of the amplitude.
 */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */

#define ITERATIONS 256 //Number of samples
#define SW4_ON (PORTD >> 11 & 1)
#define SW3_ON (PORTD >> 10 & 1)
#define BTN4_ON (PORTD >> 7 & 0x1)
#define BTN3_ON (PORTD >> 6 & 0x1)
#define CONVERSION_DONE ((IFS(1) >> 1) & 1) // Checks conversion flag. 1 if done.

int re[ITERATIONS]; // Sampled values
int im[ITERATIONS]; // Imaginary part of the sampled values

/**
 * user interupt service routine. Not currently used.
 **/
void user_isr(){
}

/**
 * Turns the sampling on and waits until done. Turns it off
 * once it's done.
 **/
void sample() {
        IFS(1) &= ~0x0002;
        AD1CON1SET = 0x0004;
        while(!CONVERSION_DONE) ;
        AD1CON1CLR = 0x0004;
}

/**
 * Samples 255 times at the preset frequency and calls
 * on graphAmp with the maximum sampled amplitude to display
 * a visual representation of the amplitude of the audio.
 **/
void operationAmpl(){
        short maxVal = 0;
        short tmp;
        uint8_t i;
        for (i = 0; i < ITERATIONS-1; i++) {
                sample();
                tmp = absolute(ADC1BUF0 - 512);
                if(tmp > maxVal) maxVal = tmp;
        }
        graphAmp(maxVal);
}

/**
 * Samples 256 times at the preset frequency. To be able
 * to display it properly on the 128 column IO-display, the
 * average of two adjacent values are calucated. The function
 * calls on graphFreqorWave to display the amplitude-time relation
 * on the IO-shield.
 **/
void operationWave() {
        uint8_t i,j;
        int val;
        for (i = 0; i < ITERATIONS/2; i++) {
                val = 0;
                for (j = 0; j < 2; j++) {
                        sample();
                        val += ADC1BUF0;
                }
                re[i] = val / 2;
        }
        graphFreqorWave(re,0,ITERATIONS/2);
}

/**
 * Samples 256 times at the pre set frequency. Calls on
 * a FFT (fast fourier transform) program to get the
 * amplitude-frequency relation. Calls on graphFreqorWave
 * to display the 128 results on the IO-Shield.
 **/
void operationFreq() {
        unsigned int i;
        for (i = 0; i < ITERATIONS; i++) {
                sample();
                re[i] = ADC1BUF0 - 512;
                im[i] = 0;

        }

        fix_fft(re,im,8); // FFT calculation

        for (i = 0; i < ITERATIONS/2; i++) {
                re[i] = isqrt32(re[i]*re[i]+im[i]*im[i]);
        }
        graphFreqorWave(re,1,ITERATIONS/2);
}


/**
 * Main function of the program. Infinite loop that checks buttons and
 * switches. If Button 4 is pressed, the sampling frequency is set to
 * 20 kHz and if Button 3 is pressed, the sampling frequency is set to 10 kHz.
 * Important to note is that the highest frequency that can be shown is half the
 * sampling frequency (Nyquist Frequency).
 *
 * If Switch 3 is enabled, the program will calculate and display a visual
 * representation (pulsating oval) of the sound amplitude.
 *
 * If Switch 4 is enabled, the program will calculate and display an
 * amplitude-frequency graph.
 *
 * If neither Switch 3 or Switch 4 are enabled, the program will
 * calculate and display an amplitude-time graph.
 **/
int main(void) {
        setUp();
        while( 1 )
        {
                if (BTN4_ON) {
                        AD1CON3 = 0x019a; // 20 kHz sampling rate
                }
                else if (BTN3_ON) {
                        AD1CON3 = 0x04fa; // 10 kHz sampling rate
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
