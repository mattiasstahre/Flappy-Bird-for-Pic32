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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265


//Alla våra objekt nedan


// prime = nextprime( prime );




int mytime = 0x5957;

int score = 12345;

int TacoX = 15;       //Tacon som flyger
int TacoY = 5;

int Tube1X = 60;
int Tube1Y = 25;

int BottomLineX = 0;      //Statiska linjen längst ner
int BottomLineY = 30;

int TopLineX = 0;       //Statiska linjen längst upp
int TopLineY = 0;

int Tube2X = 160;
int Tube2Y = 0;

//int GameOverX = 0;
//int GameOverY = 0;

int rnum;



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

rnum = random();


  if (gameState == 1){

    drawGameOver();
    display_image(0, icon);
    delay(2000);
    TacoX = 15;       //Tacon som flyger
    TacoY = 7;

    display_update();
    gameState = 0;


}
/*
  switch (gameState){
    case 0:
    break;

    case 1:
    while(0){

      drawGameOver();

    }
    break;

    case 2:
    break;

    default:
    break;
  }

  */


  display_string( 12, itoaconv( score ) );

  int i;
  int j;


  TacoY += 1;


  //Game over

  // Emulerar position för objekt.

  clearScreenMemory();







//drawGameOver (GameOverX, GameOverY);
drawTopLine (TopLineX, TopLineY);

drawBottomLine(BottomLineX, BottomLineY);
drawTube1(Tube1X, Tube1Y, rnum);
drawTube2(Tube2X, Tube2Y);


drawTaco(TacoX, TacoY);

// int write = ~128;
// icon[384+x] = icon[384+x] & write;


// gameState = crash(Tube1X, TacoX, TacoY, Tube1Y);


/*
if ((Tube1X -TacoX) <= 0 && (TacoY - Tube1Y) <= 0)) {
    if ((Tube1X -TacoX) <= -3 && (TacoY - Tube1Y) <= -9) {
        drawGameOver (GameOverX, GameOverY);
    }
  }

if ((Tube1X -TacoX) >= 0 && (TacoY - Tube1Y) >= 0)){
    if ((Tube1X -TacoX) <= 3 && (TacoY - Tube1Y) <= 9) {
        drawGameOver (GameOverX, GameOverY);
      }
    }*/
                                        // rör --> Game Over

/*  for(i = 0; i < 128; i++){
    for(j = 0; j < 32; j++){
      markPixel(GameOverX+i, j+GameOverY);    // Tar emot X, Y
    }
  }*/



  display_image(0, icon);


  TacoX = TacoX;
  TacoY = TacoY;

  Tube1X = Tube1X - 1 ;
  Tube1Y = Tube1Y ;

  Tube2X = Tube2X - 1 ;
  Tube2Y = Tube2Y ;


  display_string( 3, textstring );
  //display_update();
  tick( &mytime );



  delay(50);

  time2string( textstring, mytime );  // mytime är hex t ex 0x5957
    // nollställer rätt bitar ex 8:5 i mytime. Vi lägger på värdet från getsw i dem bitarna.

    if ( getbtns() == 1){
      mytime = ((mytime & (~0xf0)) |  (getsw() << 4 ));
      time2string( textstring, mytime );
    }

    if ( getbtns() == 2 ){
      mytime = ((mytime & (~0xf00)) |  (getsw() << 8));
      TacoY -= 4;
      time2string( textstring, mytime );
    }

    if ( getbtns() == 4 ){
      mytime = ((mytime & (~0xf000)) |  (getsw() << 12));
      time2string( textstring, mytime);
    }





  // +1 varje gång labwork anropas.
  *initPORTE += 1;





}
