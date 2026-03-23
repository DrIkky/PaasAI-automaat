#include "project.h"

volatile uint8 servoCounts[4] = {0, 0, 0, 0}; 

volatile uint8 startCommandReceived = 0;
volatile uint8 receptionState = 0; // 0=Wacht op S, 1=Data verzamelen
volatile uint8 byteIndex = 0;      // Welke van de 4 zijn we aan het lezen?

CY_ISR(MyUART_Handler)
{
    while(UART_1_GetRxBufferSize() > 0)
    {
        uint8 receivedByte = UART_1_GetChar();
        
        if(receptionState == 0)
        {
            if(receivedByte == 'S')
            {
                receptionState = 1; // Ga data verzamelen
                byteIndex = 0;      // Begin bij de eerste (n0)
            }
        }
        // STAAT 1: De 4 bytes vullen
        else if(receptionState == 1)
        {
            // Sla de byte op in het juiste vakje van de lijst
            servoCounts[byteIndex] = receivedByte;
            byteIndex++; // Ga naar het volgende vakje
            
            // Hebben we ze alle 4 binnen? (0, 1, 2, 3)
            if(byteIndex >= 4)
            {
                startCommandReceived = 1; // Sein naar Main!
                receptionState = 0;       // Reset en wacht weer op 'S'
            }
        }
    }
}

/* Hulpfunctie om servo te bewegen (scheelt typwerk) */
/* We geven mee: Welke PWM functie, en hoe vaak */
void MoveServo(void (*writeCompareFunc)(uint16), uint8 count)
{
    for(int i=0; i<count; i++)
    {
        writeCompareFunc(2400); // Rechts
        CyDelay(800);
        writeCompareFunc(1500);  // Midden
        CyDelay(800);
    }
}

int main(void)
{
    CyGlobalIntEnable; 

    UART_1_Start();
    isr_UART_StartEx(MyUART_Handler);
    
    PWM_1_Start();
    PWM_2_Start();
    PWM_3_Start();
    PWM_4_Start();
    
    PWM_1_WriteCompare(1500);
    PWM_2_WriteCompare(1500);
    PWM_3_WriteCompare(1500);
    PWM_4_WriteCompare(1500);

    for(;;)
    {
        if(startCommandReceived == 1)
        {
            startCommandReceived = 0; // Vlag omlaag
            
            // Servo 1 (n0)
            MoveServo(PWM_1_WriteCompare, servoCounts[0]);
            
            // Servo 2 (n1)
            MoveServo(PWM_2_WriteCompare, servoCounts[1]);
            
            // Servo 3 (n2)
            MoveServo(PWM_3_WriteCompare, servoCounts[2]);
            
            // Servo 4 (n3)
            MoveServo(PWM_4_WriteCompare, servoCounts[3]);
        }
    }
}