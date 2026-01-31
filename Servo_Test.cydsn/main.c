#include "project.h"

/* Variabelen */
volatile uint8 servoCount = 0; 
volatile uint8 startCommandReceived = 0;

/* Status machine variabelen */
volatile uint8 state = 0;       // 0 = Wacht op 'S', 1 = Wacht op Data
volatile uint8 byteCounter = 0; // Telt de bytes NA de 'S'

CY_ISR(MyUART_Handler)
{
    /* Lees de buffer leeg zolang er data is */
    while(UART_1_GetRxBufferSize() > 0)
    {
        uint8 receivedByte = UART_1_GetChar();
        
        /* STAAT 0: Zoeken naar de startletter 'S' */
        if(state == 0)
        {
            if(receivedByte == 'S' || receivedByte == 0x53)
            {
                state = 1;      // Gevonden! Ga naar volgende fase
                byteCounter = 0; // Reset de teller
            }
        }
        /* STAAT 1: De 4 bytes data verwerken */
        else if(state == 1)
        {
            byteCounter++; // We hebben weer een byte binnen
            
            // Volgens jouw screenshot stuurt hij: 00 00 00 02
            // We zijn dus alleen geïnteresseerd in de 4e byte.
            
            if(byteCounter == 4) 
            {
                // DIT is het getal dat we willen (de 02)
                servoCount = receivedByte;
                
                // Veiligheid: niet meer dan 10x
                if(servoCount > 5) { servoCount = 5; }
                if(servoCount < 1)  { servoCount = 1; }
                
                // Sein naar main loop
                startCommandReceived = 1;
                
                // Reset alles voor de volgende keer
                state = 0;
            }
        }
    }
}

int main(void)
{
    CyGlobalIntEnable; 

    /* Start Hardware */
    PWM_1_Start();
    UART_1_Start();
    
    /* Start Interrupt */
    isr_UART_StartEx(MyUART_Handler);

    /* Servo naar midden */
    PWM_1_WriteCompare(1500);

    for(;;)
    {
        if(startCommandReceived == 1)
        {
            startCommandReceived = 0; // Vlag omlaag
            
            // Voer de beweging uit
            
            for(int i = 0; i < servoCount; i++)
            {
                // Links
                PWM_1_WriteCompare(500); // Of pas aan naar jouw waardes
                CyDelay(800); 
                
                // Rechts
                PWM_1_WriteCompare(2500);
                CyDelay(800);
            }
        }
    }
}