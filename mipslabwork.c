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
#define   TMR2PERIOD ((80000000 / 256) / 25 )  // = 31250 < 65 536 (=2^16)


// test

// make install TTYDEV=/dev/cu.usbserial-AJV9JY30

//Alla våra objekt nedan

// prime = nextprime( prime );

int mytime = 0x5957;

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

int Tube2X = 100;
int Tube2Y = 10;

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
  IFSCLR(0) = 0x100;    // Nollställ interuptflaggan!
  InteruptFlag40ms = 1;
  //Lägg counter här

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


  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  int i;
  int j;

  gameScore();

  if (gameState == 1){                    //1 = Game over

    drawGameOver(TacoX, TacoY);
    delay(2000);

    updateGameScore();
    gameScoreSort();

    display_string(0, "  High score:");
    display_score(1, s);
    display_update();

    while( getbtns() != 4 ){

    }

    display_string_clear();

    StartCountDown();



// Återställer alla objekt på banan
    for(i=0; i<64*2; i++){
    objectPosLevel1[i]=objectPosLevel1Reset[i];
  }

    resetGameField();             //Ändrar tillbaka Taco och Tube x- och y-positioner
    clearScreenMemory();
    display_image(0, icon);
    display_string_clear();
    display_update();
    gameScoreZero();
    gameState = 0;

}


if (gameState == 2){                //2 = Main Menu/Start Screen
  while(gameState == 2){
      countStart++;
      display_string_clear();
      delay(10);
      display_string(2, "  Flappy Taco");
      display_update();


      if(getbtns() == 4){
        MainMenuFade();           //Animation till höger av Flappy Taco
        StartCountDown();         //Kör 3,2,1,go på skärmen

      }
    }
}

//display_string( 12, itoaconv( score ) );



clearScreenMemory();

//drawGameOver (GameOverX, GameOverY);
drawTopLine ();

drawBottomLine();

for(i =0 ; i<= 64; i = i + 2){


  drawObjectTube(objectPosLevel1[i], objectPosLevel1[i+1]);   //i+1  = y-värdet
  objectPosLevel1[i]= objectPosLevel1[i]-1;     //Flyttar rören åt vänster

  if (getsw() == 8){    //Om switch längst till vänster är nertryckt
  objectPosLevel1[i+1] = objectPosLevel1[i+1]-1;
  }

  if (getsw() == 4){    //Om switch nästlängst till vänster är nertryckt
  objectPosLevel1[i+1] = objectPosLevel1[i+1]+1;
  }
  if (objectPosLevel1[i+1] == -80) {
    objectPosLevel1[i+1] = objectPosLevel1[i+1]+120;
  }


  if (objectPosLevel1[i+1] < -80) {
    objectPosLevel1[i+1] = objectPosLevel1[i+1]+1;
  }

}



// 0 2 4 6

drawTaco(TacoX, TacoY);
display_image(0, icon);


  //display_string( 3, textstring );
  //display_update();
  //tick( &mytime );
/*

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
    delay(30);
  }
  */

//  time2string( textstring, mytime );  // mytime är hex t ex 0x5957

    if ( getbtns() == 1){
      gameState = 2;
    }

    if ( getbtns() == 2 ){
      TacoY -= 3;
    }

    if ( getbtns() == 4 ){
    }

TacoY += 1;

// Styr Led-lampor
  *initPORTE += 1;



}
