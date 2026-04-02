#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "AHT21.h"
#include <stdio.h>

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

void BeweegBandTijd(uint32 milliseconden)
{

    uint32 delayUs = 125; 
    
    uint32 totaleTijdPerStapUs = delayUs * 2;
    uint32 aantalStappen = (milliseconden * 1000) / totaleTijdPerStapUs;
    
    for(uint32 i = 0; i < aantalStappen; i++)
    {
        Pin_Step_Write(1);
        CyDelayUs(delayUs); 
        Pin_Step_Write(0);
        CyDelayUs(delayUs);
    }
    
    // Even wachten zodat de band en het doosje helemaal stilstaan
    CyDelay(500); 
}

void MoveServoWithCheck(void (*dispenseServo)(uint16), void (*wokkelServo)(uint16), uint8 targetCount)
{
    for(int i = 0; i < targetCount; i++)
    {
        uint8 dropSuccess = 0;
        uint8 attempts = 0;

        // Blijf proberen zolang het ei niet is gevallen (max 3 keer)
        while(!dropSuccess && attempts < 3) 
        {
            attempts++;
            
            // 1. Reset de Hardware Latch (wis geheugen)
            Pin_Reset_Laser_Write(1);
            CyDelayUs(10); 
            Pin_Reset_Laser_Write(0);

            dispenseServo(600);   
            CyDelay(800); 

            dispenseServo(1500);  
            CyDelay(500); 

            if((Status_Laser_Read() & 0x01) == 0) 
            {
                dropSuccess = 1; 
            }
            else 
            {
                // JAM DETECTED
                
                wokkelServo(1400);
                CyDelay(800);       
                
                wokkelServo(1500);
                CyDelay(500);
            }
        }
    }
}

void DispenseBakje(void)
{
    int stapGrootte = 30; 
    int wachtTijd = 15;   
    
    int slagLengte = 900; // GETAL KLEINER OM DE AFSTAND TE VERKLEINEN

    // 1. LANGZAAM OPENEN (Bakje laten vallen)
    for(int i = 0; i <= slagLengte; i += stapGrootte)
    {
        PWM_9_WriteCompare(600 + i);   // Start op 600, telt op
        PWM_10_WriteCompare(2400 - i); // Start op 2400, trekt af
        CyDelay(wachtTijd);            
    }
    
    CyDelay(1000); // Wacht even tot bakje valt
    
    // 2. LANGZAAM WEER DICHT (Terug naar de startpositie)
    // We laten de 'i' nu weer netjes aftellen van de maximale uitslag naar 0
    for(int i = slagLengte; i >= 0; i -= stapGrootte)
    {
        PWM_9_WriteCompare(600 + i);  
        PWM_10_WriteCompare(2400 - i);  
        CyDelay(wachtTijd);
    }
    
    CyDelay(500); 
}

/* NIEUWE HULPFUNCTIE: Laat het Nextion scherm naar een andere pagina springen */
void Nextion_GaNaarPagina(uint8 paginaNummer)
{
    char buffer[20];
    
    // Maak het commando, bijvoorbeeld "page 2"
    sprintf(buffer, "page %d", paginaNummer); 
    
    // Stuur het commando via UART
    UART_1_PutString(buffer);
    
    // Stuur de 3 verplichte afsluit-bytes
    UART_1_PutChar(0xFF);
    UART_1_PutChar(0xFF);
    UART_1_PutChar(0xFF);
}

int main(void)
{
    CyGlobalIntEnable; 

    // Start UART and interrupts
    UART_1_Start();
    isr_UART_StartEx(MyUART_Handler);
    
    I2C_1_Start();
    AHT21_Start(); // Roep je eigen start-functie aan
    
    // Start Servo PWMs voor de deurtjes
    PWM_1_Start();
    PWM_2_Start();
    PWM_3_Start();
    PWM_4_Start();
    
    // Start Servo PWMs voor de wokkels
    PWM_5_Start();
    PWM_6_Start();
    PWM_7_Start();
    PWM_8_Start();

    PWM_9_Start();
    PWM_10_Start();
    // Initialize Deurtjes-Servos to middle (dicht)
    PWM_1_WriteCompare(1500);
    PWM_2_WriteCompare(1500);
    PWM_3_WriteCompare(1500);
    PWM_4_WriteCompare(1500);
    
    // Zet alle wokkels direct stil (1500)
    PWM_5_WriteCompare(1500);
    PWM_6_WriteCompare(1500);
    PWM_7_WriteCompare(1500);
    PWM_8_WriteCompare(1500);
    
    // ---> VOEG DIT TOE: Zet de bakjes-servo's in rustpositie <---
    PWM_9_WriteCompare(600);   // 10 graden
    PWM_10_WriteCompare(2400); // 170 graden

    // Initialiseer Stepper pinnen
    Pin_Step_Write(0);
    Pin_Dir_Write(1); 
        

    
    for(;;)
    {
        // WACHTEN OP SCHERM (Selectie gemaakt?)
        if(startCommandReceived == 1)
        {
            startCommandReceived = 0; // Vlag direct weer omlaag
            
            
            
            // ... jouw bestaande automaat code ...
            
            // Voorbeeld: Lees de temperatuur en stuur hem naar je Nextion variabele 'vaTemp'
            int huidigeTemp = AHT21_MeetTemperatuur();
            
            char buffer[30];
            sprintf(buffer, "page0.vaTemp.val=%d", huidigeTemp);
            UART_1_PutString(buffer);
            UART_1_PutChar(0xFF); UART_1_PutChar(0xFF); UART_1_PutChar(0xFF);
        
        
        
            DispenseBakje();
            
            // Omdat we geen sensor meer hebben om op het doosje te wachten, 
            // bouwen we hier een veiligheidspauze in van 2 seconden nadat op
            // het scherm is gedrukt. Zo heeft de klant tijd om z'n hand weg te halen!
            CyDelay(2000);
            
            // 1. BEWEEG NAAR SERVO 1 (Vanaf de startpositie)
            // Vul in hoeveel milliseconden hij hiervoor moet rijden (bijv. 3000 ms = 3 sec)
            BeweegBandTijd(2500); 
            MoveServoWithCheck(PWM_1_WriteCompare, PWM_5_WriteCompare, servoCounts[0]);

            // 2. BEWEEG NAAR SERVO 2 (Vanaf servo 1)
            // Vul in hoeveel milliseconden de afstand tussen dispenser 1 en 2 is.
            BeweegBandTijd(2200); 
            MoveServoWithCheck(PWM_2_WriteCompare, PWM_6_WriteCompare, servoCounts[1]);

            // 3. BEWEEG NAAR SERVO 3 (Vanaf servo 2)
            BeweegBandTijd(2300); 
            MoveServoWithCheck(PWM_3_WriteCompare, PWM_7_WriteCompare, servoCounts[2]);

            // 4. BEWEEG NAAR SERVO 4 (Vanaf servo 3)
            BeweegBandTijd(2300); 
            MoveServoWithCheck(PWM_4_WriteCompare, PWM_8_WriteCompare, servoCounts[3]);
            
            // 5. BEWEEG NAAR DE UITGANG
            // Laat de band nog een paar seconden draaien zodat het doosje bij de klant komt
            BeweegBandTijd(4000); 
            
            Nextion_GaNaarPagina(6);
            
            
            // HIERNA IS DE SEQUENTIE KLAAR EN WACHT HIJ OP DE VOLGENDE KLANT!
        }
    }
}