/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Start de hardware blokken */
    Clock_1_Start(); // Start de klok
    PWM_1_Start();   // Start de PWM
    UART_1_Start(); // Start de communicatie
    
    /* Nextion Opstart procedure */
    CyDelay(1000); // Wacht even tot het scherm is opgestart
    
    // Test: Stuur een commando naar Nextion.
    // Nextion commando's sluit je ALTIJD af met 3x 0xFF (hex)
    UART_1_PutString("rest"); // "rest" reset de Nextion
    UART_1_PutChar(0xFF);
    UART_1_PutChar(0xFF);
    UART_1_PutChar(0xFF);
    
    
    for(;;)
    {
        /* Draai naar links (1ms = 1000) */
        PWM_1_WriteCompare(1000);
        CyDelay(1000); // Wacht 1 seconde

        /* Draai naar midden (1.5ms = 1500) */
        PWM_1_WriteCompare(1500);
        CyDelay(1000);

        /* Draai naar rechts (2ms = 2000) */
        PWM_1_WriteCompare(2000);
        CyDelay(1000);
    }
}

/* [] END OF FILE */
 