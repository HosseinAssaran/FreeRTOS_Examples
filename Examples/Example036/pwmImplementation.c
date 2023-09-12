#include <pwmImplementation.h>

/**
 * PWM implementation supplies implementations of iPWM for the red, green, and blue
 * LED's on the Nucleo board
 */

/**
 * Initialize pins for PWM output to the 3 LED's
 * Configures each timer to provide 16 bits of PWM resolution
 * per channel : 0 - 65535 (0-100%)
 *
 * Color	Pin		Timer		Alt Func	main.h
 * Red		PB14	TIM12CH1 	AF9			LD3_Pin
 * Green	PB0		TIM3CH3		AF2			LD1_Pin
 * Blue		PB7		TIM4CH2		AF2			LD2_Pin
 */
void PWMInit( void )
{
	printf("Implement PWM initialization here.\n");
}

void SetBlueDuty( float DutyCycle )
{
	printf("--SetBlueDuty to %d.\n", (int) DutyCycle);

}
iPWM BluePWM = {.SetDutyCycle = SetBlueDuty};

void SetGreenDuty( float DutyCycle )
{
	printf("--SetGreenDuty to %d.\n", (int) DutyCycle);
}
iPWM GreenPWM = {.SetDutyCycle = SetGreenDuty};

void SetRedDuty( float DutyCycle )
{
	printf("--SetRedDuty to %d.\n", (int) DutyCycle);
}
iPWM RedPWM = {.SetDutyCycle = SetRedDuty};
