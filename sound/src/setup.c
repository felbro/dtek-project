
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
        TRISE &= ~0xff;

        TRISB |= 0x0004;
        AD1PCFG = 0xFFFB;         // PORTB = Digital; RB2 = analog


        //AD1CON1 = 0x0000; // SAMP bit = 0 ends sampling
        // and starts converting
        //AD1CON1 = 0x0004; // automatically starts after conv done.
        AD1CON1 = 0x00E0;         // FOR INTERRUPTS

        AD1CHS = 0x00020000;         // Connect RB2/AN2 as CH0 input
        // in this example RB2/AN2 is the input
        AD1CSSL = 0;
        //AD1CON3 = 0x014c;         // TAD = 154 internal TPB, Samples for 1 TAD. AKA sample 20kHz
        AD1CON3 = 0x047c;

        //AD1CON2 = 0; //normal -- NO INTERRUPTS

        /**  FOR INTERRUPTS ONLY **/
        //AD1CON2 = 0x0004; // 2 samp and conv before conversion
        AD1CON2 = 0x0000;         // 1 samp nefore conversion.

        IPC(6) |= 0x1c00;         // Set Priority to 5
        //IPCS(6) |= 0x0003; // Set Sub Priority to 3
        //
        IFS(1) &= ~0x0002;         // Ensure the interrupt flag is clear
        IEC(1) |= 0x0002;

        /** -------------------- **/

        AD1CON1SET = 0x8000;         // turn on the ADC

//FOR SW4
        //IEC(0) |= 0x80000;
        //IPC(4) |= 0x1f000000;

        enable_interrupt();
        return;
}
