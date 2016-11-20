/* mipslabwork.c

   This file written 2015 by F Lundevall

   This file should be changed by YOU! So add something here:

   This file modified 2015-12-24 by Ture Teknolog

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdio.h>
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int ADCValue;
int ADCValue2;
char str[9];

unsigned char count = 0;

char textstring[] = "text, more text, and even more text!";

void convertit(int value) {
        int i;
        int other = 1;
        for (i = 7; i >= 0; i--) {
                str[i] = (value / other) % 10 + 48;
                other *= 10;
        }
}

void updateValues() {
        ADCValue = ADC1BUF0; // yes then get ADC value
        ADCValue2 = ADC1BUF1;
        convertit(ADCValue);
        display_string( 3, str);
        convertit(ADCValue2);
        display_string( 2, str);
        display_update();
}


/* Interrupt Service Routine */
void user_isr( void )
{

        return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
        TRISB |= 0x0004;
        AD1PCFG = 0xFFFB; // PORTB = Digital; RB2 = analog


        //AD1CON1 = 0x0000; // SAMP bit = 0 ends sampling
        // and starts converting
        //AD1CON1 = 0x0004; // automatically starts after conv done.
        AD1CON1 = 0x00E0;

        AD1CHS = 0x00020000; // Connect RB2/AN2 as CH0 input
        // in this example RB2/AN2 is the input
        AD1CSSL = 0;
        AD1CON3 = 0x0002; // Manual Sample, TAD = internal 6 TPB

        //AD1CON2 = 0; //normal

/**  FOR THIS PART ONLY **/
        AD1CON2 = 0x0004;
        IPC(6) |= 0x1c00; // Set Priority to 5
        //IPCS(6) |= 0x0003; // Set Sub Priority to 3
//
        IFS(1) &= ~0x0002; // Ensure the interrupt flag is clear
        IEC(1) |= 0x0002;

/** -------------------- **/

        AD1CON1SET = 0x8000; // turn on the ADC
        enable_interrupt();
        return;
}



/* This function is called repetitively from the main program */
void labwork( void )
{

        /*if(((IFS(1) >> 1) & 1) == 1) {
                IFS(1) &= ~0x0002;
                ADCValue = ADC1BUF0;
                convertit();
                display_string( 3, str);
                display_update();
           }*/

        ADCValue = 0;
        ADCValue2 = 0;
        //AD1CON1SET = 0x0002; // start sampling ...
        //delay(100);     // for 100 ms

        IFS(1) &= ~0x0002;

        AD1CON1SET = 0x0004;


        while(((IFS(1) >> 1) & 1) != 1) ;

        AD1CON1CLR = 0x0004;     // start Converting
        //while (!(AD1CON1 & 0x0001)) ;  // conversion done?
        updateValues();

}
