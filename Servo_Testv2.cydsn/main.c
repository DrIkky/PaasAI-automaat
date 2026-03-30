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
                
                // -- DEBUG --
                // LED_Write(~LED_Read()); 
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

    // 1. Stuur een trigger puls van 10 microseconden
    Pin_Trigger_Write(1);
    CyDelayUs(10);
    Pin_Trigger_Write(0);

    // 2. Wacht tot de Echo pin HOOG wordt (signaal is vertrokken)
    while(Pin_Echo_Read() == 0)
    {
        CyDelayUs(1);
        timeout++;
        if(timeout > 30000) return 999; // Fout/Timeout: Niets gevonden
    }

    // 3. Meet hoelang de Echo pin HOOG blijft (signaal reist heen en weer)
    while(Pin_Echo_Read() == 1)
    {
        CyDelayUs(1);
        echoTijd++;
        if(echoTijd > 30000) return 999; // Fout/Timeout: Buiten bereik
    }

    // 4. Bereken afstand in cm (Geluidsnelheid conversie voor HC-SR04)
    // Let op: omdat CyDelayUs(1) in een loopje iets trager is dan 1us in werkelijkheid, 
    // kan deze deler (normaal 58) in de praktijk iets afwijken. Test dit even!
    return (echoTijd / 58); 
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
    Pin_Dir_Write(1); // Verander naar 0 als hij de verkeerde kant op draait

    for(;;)
    {
        // 1. WACHTEN OP SCHERM
        if(startCommandReceived == 1)
        {
            startCommandReceived = 0; // Vlag omlaag
            
            // 2. LOPENDE BAND DRAAIEN TOT AFSTAND <= 50cm is
            uint16 huidigeAfstand = 999;
            
            while(huidigeAfstand > 50)
            {
                // Laat de stappenmotor een klein stukje (bijv. 20 stappen) draaien
                for(int i=0; i<20; i++)
                {
                    Pin_Step_Write(1);
                    CyDelayUs(1000); // Snelheid (pas aan indien nodig)
                    Pin_Step_Write(0);
                    CyDelayUs(1000);
                }
                
                // Meet daarna opnieuw de afstand
                huidigeAfstand = MeetAfstandCM();
            }
            
            // Als we uit de while-loop komen, is de afstand 50cm of minder! De band stopt nu vanzelf.
            // Optioneel even kort wachten tot het doosje echt stilstaat:
            CyDelay(500);
            
            // 3. EIEREN LATEN VALLEN (SERVO'S DRAAIEN)
            // Servo 1 (n0)
            MoveServo(PWM_1_WriteCompare, servoCounts[0]);
            
            // Servo 2 (n1)
            MoveServo(PWM_2_WriteCompare, servoCounts[1]);
            
            // Servo 3 (n2)
            MoveServo(PWM_3_WriteCompare, servoCounts[2]);
            
            // Servo 4 (n3)
            MoveServo(PWM_4_WriteCompare, servoCounts[3]);
            
            // Hierna is het proces klaar en wacht de loop weer op een nieuw 'S' commando!
        }
    }
}