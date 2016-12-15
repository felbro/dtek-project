/* setup.c
 *   Written 2016 by Felix Brober and Julia Duong
 *   All settings before //OWN SETUP mark taken from
 *   mipslabmain.c with comment:
          mipslabmain.c

          This file written 2015 by Axel Isaksson,
          modified 2015 by F Lundevall

          Latest update 2015-08-28 by F Lundevall

 *
 *   For copyright and licensing, see file COPYING
 *
 * Sets up the chipkit with correct values for ports,
 * IO-shield and analog input. The analog input is set
 * to 10 kHz sampling frequency, automatic sampling and
 * enabled interrupt and will sample from AN2 (A0 on
 * the Basic Shield).
 */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */

void setUp(){
        /* Set up peripheral bus clock */
        /* OSCCONbits.PBDIV = 1; */
        OSCCONCLR = 0x100000; /* clear PBDIV bit 1 */
        OSCCONSET = 0x080000; /* set PBDIV bit 0 */

        /* Set up output pins */
        AD1PCFG = 0xFFFF;
        ODCE = 0x0;
        TRISECLR = 0xFF;
        PORTE = 0x0;

        /* Output pins for display signals */
        PORTF = 0xFFFF;
        PORTG = (1 << 9);
        ODCF = 0x0;
        ODCG = 0x0;
        TRISFCLR = 0x70;
        TRISGCLR = 0x200;

        /* Set up input pins */
        TRISDSET = (1 << 8);
        TRISFSET = (1 << 1);

        /* Set up SPI as master */
        SPI2CON = 0;
        SPI2BRG = 4;
        /* SPI2STAT bit SPIROV = 0; */
        SPI2STATCLR = 0x40;
        /* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
        /* SPI2CON bit MSTEN = 1; */
        SPI2CONSET = 0x20;
        /* SPI2CON bit ON = 1; */
        SPI2CONSET = 0x8000;
        display_init();


// OWN SETUP
        AD1PCFG = 0xFFFB;         // PORTB = Digital; RB2 = analog

        AD1CON1 = 0x00E0;         // Enables automatic sampling with interrupts.

        AD1CHS = 0x00020000;         // Connect RB2/AN2 as CH0 input
        AD1CSSL = 0;
        //AD1CON3 = 0x019a;         // Sample 20kHz - show 10khz
        AD1CON3 = 0x04fa;           // Sample 10khz - show 5kHz


        /**  FOR INTERRUPTS ONLY **/
        AD1CON2 = 0x0000;         // 1 sample nefore conversion.

        IPC(6) |= 0x1c00;         // Set Priority to 5. Could be set to 7 instead
                                  // With sub-prio set to whatever (e.g. 3 if necessary)
        IFS(1) &= ~0x0002;         // Ensure the interrupt flag is clear
        IEC(1) |= 0x0002;           // Enable interrupt for the conversion

        /** -------------------- **/

        AD1CON1SET = 0x8000;         // turn on the ADC

        enable_interrupt();          // Enables interrupts on the device.
        return;
}
