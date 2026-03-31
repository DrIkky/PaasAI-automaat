#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

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
            servoCounts[byteIndex] = receivedByte;
            byteIndex++; 
            
            if(byteIndex >= 4)
            {
                startCommandReceived = 1; // Sein naar Main!
                receptionState = 0;       // Reset en wacht weer op 'S'
            }
        }
    }
}

/* Hulpfunctie om afstand te meten met een standaard ultrasoonsensor (HC-SR04) */
uint16 MeetAfstandCM(void)
{
    uint16 timeout = 0;
    uint16 echoTijd = 0;

    // 1. Stuur een trigger puls
    Pin_Trigger_Write(1);
    CyDelayUs(10);
    Pin_Trigger_Write(0);

    // 2. Wacht tot de Echo pin HOOG wordt
    while(Pin_Echo_Read() == 0)
    {
        CyDelayUs(1);
        timeout++;
        if(timeout > 30000) return 999; // Fout/Timeout
    }

    // 3. Meet hoelang de Echo pin HOOG blijft
    while(Pin_Echo_Read() == 1)
    {
        CyDelayUs(1);
        echoTijd++;
        if(echoTijd > 30000) return 999; // Fout/Timeout
    }

    // 4. Bereken afstand (Kalibratie getal, hier 20 zoals je in je code had)
    return (echoTijd / 20); 
}

/* NIEUWE Hulpfunctie: Laat de stappenmotor draaien tot een specifieke afstand is bereikt */
void BeweegBandTotAfstand(uint16 doelAfstand)
{
    uint16 huidigeAfstand = MeetAfstandCM();
    
    // Zorg voor een geldige startmeting voordat we beginnen
    while(huidigeAfstand == 999) {
        huidigeAfstand = MeetAfstandCM();
        CyDelay(10);
    }

    // Zolang het doosje dichterbij is dan het doel, moet de band draaien (afstand neemt toe)
    while(huidigeAfstand < doelAfstand)
    {
        // Laat de stappenmotor een klein stukje (bijv. 10 stappen) draaien
        for(int i=0; i<10; i++)
        {
            Pin_Step_Write(1);
            CyDelayUs(1000); 
            Pin_Step_Write(0);
            CyDelayUs(1000);
        }
        
        // Meet daarna opnieuw de afstand
        uint16 nieuweAfstand = MeetAfstandCM();
        
        // Alleen de afstand updaten als het een geldige meting was (geen 999 fout)
        if(nieuweAfstand != 999) 
        {
            huidigeAfstand = nieuweAfstand;
        }
    }
    
    // Even wachten zodat de band helemaal stilstaat voordat er eieren vallen
    CyDelay(500); 
}

/* Hulpfunctie om servo te bewegen */
void MoveServo(void (*writeCompareFunc)(uint16), uint8 count)
{
    for(int i=0; i<count; i++)
    {
        writeCompareFunc(610);   // links
        CyDelay(800);
        writeCompareFunc(1400);  // Midden
        CyDelay(800);
    }
}

int main(void)
{
    CyGlobalIntEnable; 

    // Start UART and interrupts
    UART_1_Start();
    isr_UART_StartEx(MyUART_Handler);
    
    // Start Servo PWMs
    PWM_1_Start();
    PWM_2_Start();
    PWM_3_Start();
    PWM_4_Start();
    
    // Initialize Servos to middle
    PWM_1_WriteCompare(1500);
    PWM_2_WriteCompare(1500);
    PWM_3_WriteCompare(1500);
    PWM_4_WriteCompare(1500);

    // Initialiseer Stepper pinnen
    Pin_Step_Write(0);
    Pin_Dir_Write(1); 

    for(;;)
    {
        // WACHTEN OP SCHERM (Selectie gemaakt?)
        if(startCommandReceived == 1)
        {
            startCommandReceived = 0; // Vlag direct weer omlaag
            
            // 1. WACHTEN OP DOOSJE (< 5cm)
            // Blijf hier hangen zolang er geen doosje voor de sensor staat.
            while(MeetAfstandCM() > 5)
            {
                CyDelay(200); // Check elke 0.2 seconden
            }
            
            // Even 1 seconde wachten zodat de gebruiker zijn hand weg kan halen
            CyDelay(1000);
            
            // 2. BEWEEG NAAR SERVO 1 (15 cm)
            BeweegBandTotAfstand(15);
            MoveServo(PWM_1_WriteCompare, servoCounts[0]);
            
            // 3. BEWEEG NAAR SERVO 2 (29 cm)
            BeweegBandTotAfstand(29);
            MoveServo(PWM_2_WriteCompare, servoCounts[1]);
            
            // 4. BEWEEG NAAR SERVO 3 (42 cm)
            BeweegBandTotAfstand(42);
            MoveServo(PWM_3_WriteCompare, servoCounts[2]);
            
            // 5. BEWEEG NAAR SERVO 4 (56 cm)
            BeweegBandTotAfstand(56);
            MoveServo(PWM_4_WriteCompare, servoCounts[3]);
            
            // Optioneel: Laat de band daarna nog een stukje doorrijden (bijv. tot 70cm) 
            // zodat de klant het bakje er aan het einde af kan pakken.
            BeweegBandTotAfstand(70);
            
            // HIERNA IS DE SEQUENTIE KLAAR EN WACHT HIJ OP DE VOLGENDE KLANT!
        }
    }
}