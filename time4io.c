#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */


int getsw (void){

  int sw = PORTD >> 8;

   sw = sw & 0xf;

return sw;
}


int getbtns (void){

  int btn = PORTD >> 5;    // Knapp nedtryckt ner låg signal så vi inverterar

  btn =  btn & 0x7;  // Vi andar bit 11:5 och förskjuter

return btn;
}
