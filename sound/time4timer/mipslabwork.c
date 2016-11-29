/* mipslabwork.c

   This file written 2015 by F Lundevall

   This file should be changed by YOU! So add something here:

   This file modified 2015-12-24 by Ture Teknolog

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;
int timeoutcount = 0;
volatile int* trise = (volatile int*) 0xbf886100;
volatile int* porte = (volatile int*) 0xbf886110;
unsigned char count = 0;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
        return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
        *trise &= ~0xff;
        TRISD |= 0xfe0;


        T2CON = 0x8070;
        PR2 = 31250;
        TMR2 = 0;
        return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
        
}
