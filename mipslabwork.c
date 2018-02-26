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


//Alla våra objekt nedan

// prime = nextprime( prime );



int mytime = 0x5957;

int score = 12345;

int TacoX = 15;       //Tacon som flyger
int TacoY = 5;

int Tube1X = 60;
int Tube1Y = 25;


/*
for(int i=0; i < 6; i++){

  checkIfFree(Tube1X, Tube1Y, Tube1Free);
  generateObjekt(Tube[i*3]Free);
}

*/

int Tube1Free = 0;

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


  if (gameState == 1){                    //Game over

    drawGameOver(TacoX, TacoY);
    display_image(0, icon);
    delay(2000);

    resetGameField();             //Ändrar tillbaka Taco och Tube x- och y-positioner


    display_update();
    gameState = 0;

}


if (gameState == 2){                //Main Menu/Start Screen
  while(gameState == 2){
      countStart++;
      delay(10);


      display_string(0, "             0000");
      display_string(2, "  Flappy Taco");
      display_update();


      if(getbtns() == 4){
        MainMenuFade();           //Animation till höger av Flappy Taco
        StartCountDown();         //Kör 3,2,1,go på skärmen

        gameState = 0;

      }
    }
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


rnum = 8; //random(&countStart);






//drawGameOver (GameOverX, GameOverY);
drawTopLine ();

display_string(0, "");            // "" Behövs för att resna skrärmen av tidigare display_string
display_string(1, "");            //0, 1, 2, behövs för de olika raderna. 0 är raden högst upp
display_string(2, "");
display_string(3, "");
display_string(4, "");
display_string(0, "             0000");
display_update();



drawBottomLine();



drawTube1(Tube1X, Tube1Y, rnum);

// drawTube2(Tube2X, Tube2Y);


drawTaco(TacoX, TacoY);

  display_image(0, icon);


  TacoX = TacoX;
  TacoY = TacoY;

  Tube1X = Tube1X - 1 ;
  Tube1Y = Tube1Y;

  Tube2X = Tube2X - 1 ;
  Tube2Y = Tube2Y;


  //display_string( 3, textstring );
  //display_update();
  //tick( &mytime );


                            //Game speed: Tryck på en switch för att ändra delayen --> ändra hastigheten

  if (getsw() == 1){      // //Om switch längst till vänster är nertryckt
    delay(200);
  }

  if (getsw() == 2){
    delay(35);
  }

  if (getsw() == 4){
    delay(25);
  }

  if (getsw() == 8){    //Om switch längst till vänster är nertryckt
    delay(15);
  }

  else
  {
    delay(50);

  }

//  time2string( textstring, mytime );  // mytime är hex t ex 0x5957

    if ( getbtns() == 1){
      StartCountDown();
    }

    if ( getbtns() == 2 ){
      TacoY -= 4;
    }

    if ( getbtns() == 4 ){
    }


  *initPORTE += 1;





}
