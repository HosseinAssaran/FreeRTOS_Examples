#include <ledImplementation.h>


/**
 * LED implementation for Red, Green, and Blue discrete LED's on
 * Nucleo-F767ZI
 */

void GreenOn ( void ) {printf("GreenOn\n");}
void GreenOff ( void ) {printf("GreenOff\n");}
iLed GreenLed = { GreenOn, GreenOff };

void BlueOn ( void ) {printf("BlueOn\n");}
void BlueOff ( void ) {printf("BlueOff\n");}
iLed BlueLed = { BlueOn, BlueOff };

void RedOn ( void ) {printf("RedOn\n");}
void RedOff ( void ) {printf("RedOff\n");}
iLed RedLed = { RedOn, RedOff };
