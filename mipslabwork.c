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

int mytime = 0x5957;

int TacoX = 10;
int TacoY = 10;

int Tube1X = 120;
int Tube1Y = 10;


int Tube2X = 160;
int Tube2Y = 0;


char textstring[] = "text, more text, and even more text!";

//#define TRISD PIC_32R (0xbf8860D0)


// Initieras globalt
volatile int* initPORTE = (volatile int*) 0xbf886110;



/* Interrupt Service Routine */
void user_isr( void )
{
  return;
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





  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{

  int i;
  int j;
  // Emulerar position för objekt.

  clearScreenMemory();

  for(i = 0; i < 4; i++){
  	for(j = 0; j < 4; j++){
  		markPixel(TacoX+i, j+TacoY);    // Tar emot X, Y
  	}
  }

  for(i = 0; i < 5; i++){
  	for(j = 0; j < 18 ; j++){
  		markPixel(Tube1X+i, j+Tube1Y);    // Tar emot X, Y
  	}
  }

  for(i = 0; i < 5; i++){
  	for(j = 0; j < 10 ; j++){
  		markPixel(Tube2X+i, j+Tube2Y);    // Tar emot X, Y
  	}
  }

  display_image(0, icon);

  TacoX = TacoX + 1;
  TacoY = TacoY ;

  Tube1X = Tube1X - 1 ;
  Tube1Y = Tube1Y ;

  Tube2X = Tube2X - 1 ;
  Tube2Y = Tube2Y ;





  delay( 80 );

  time2string( textstring, mytime );  // mytime är hex t ex 0x5957
    // nollställer rätt bitar ex 8:5 i mytime. Vi lägger på värdet från getsw i dem bitarna.

    if ( getbtns() == 1){
      mytime = ((mytime & (~0xf0)) |  (getsw() << 4 ));
      time2string( textstring, mytime );
    }

    if ( getbtns() == 2 ){
      mytime = ((mytime & (~0xf00)) |  (getsw() << 8));
      time2string( textstring, mytime );
    }

    if ( getbtns() == 4 ){
      mytime = ((mytime & (~0xf000)) |  (getsw() << 12));
      time2string( textstring, mytime);
    }



  //display_string( 3, textstring );
  //display_update();
  tick( &mytime );

  // +1 varje gång labwork anropas.
  *initPORTE += 1;





}
