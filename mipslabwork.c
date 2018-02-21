// Session ID: b3a28c59-4e43-43fa-8009-6b914f08bf8b

/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#if TMR2PERIOD > 0xffff
#error "Timer period is too big. No more than 16 bits can be used for the timer"
#endif
#define   TMR2PERIOD ((80000000 / 256) / 10)  // = 31250 < 65 536 (=2^16)

int TimeOutCount = 0;
int globalTimeOutCount = 0;

int prime = 1234567;

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

//#define TRISD PIC_32R (0xbf8860D0)


// Initieras globalt
volatile int* initPORTE = (volatile int*) 0xbf886110;



/* Interrupt Service Routine */
void user_isr( void ) {

TimeOutCount++;

if (TimeOutCount == 10){
 time2string( textstring, mytime );
 display_string( 3, textstring );
 display_update();
 tick( &mytime );
 TimeOutCount = 0;      //nollställ variabel

}
  IFSCLR(0) = 0x100;    // Nollställ interuptflaggan!

}

/* Lab-specific initialization goes here */
void labinit( void )
{

// Initiera portE, TRISE
//Generellt TRISx
//7:0 LSB as output
//TRISE has address 0xbf886100

// Initiera TRISE så att det är output.
volatile int* initTRISE = (volatile int*) 0xbf886100;
*initTRISE = ~0xff;

// PORTE sätts till 0 vid initiering.
*initPORTE = 0;

//Initiera TRISE så att det är input.

TRISD = 0xfe0;

  // Initiera timer 2 för timeouts 100ms Är en typ B timer

  T2CON = 0x70;          // 0111 0000 Vi sätter bit 6:4 till 111 prescale 1:256
                        // set prescaler at 1:1, internal clock source
  TMR2 = 0x0;           // Clear timer register
  PR2 = TMR2PERIOD;         // Load period register

  T2CONSET = 0x8000;         // Start timer



    // Slår på interupts för timer 2.
    // LÄS PÅ OM IPC OCH IEC!!!!!!!
    // Använder pekare.

    int *IEC = 0xbf881060;
    int *IPC2 = 0xbf8810b0;



    *IEC = 0x100;     //Vi sätter bit #9 (index 8) se bild sid 90.
    // Interupt Priority Control
    // http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf Sid 90.
    // Bitar 4:2 sätter priotitet 0-7 där 0-3 är Subpriority.


    *IPC2 = *IPC2 | 0x10;   //Vi sätter prioriteret 4 genom att endast bit #5 (index 4)

/*
    IPC(2) = IPC(2) | 0x10;
    // set bit no 8 to enable interupt
    IEC(0) = 0x100;
    // calling interupt from labwork.S
*/
    enable_interrupt();


    // IEC0SET = 0x00000100;



  return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {
  prime = nextprime( prime );
  display_string( 0, itoaconv( prime ) );
  display_update();
}
