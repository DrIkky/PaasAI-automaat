/* ========================================
 * Testproject Ultrasoon naar Nextion Display
 * ========================================*/
#include "project.h"
#include <stdio.h>  // Nodig om getallen naar tekst om te zetten (sprintf)

volatile int count;
volatile int afstand_cm;

/* Hulpfunctie om getal naar Nextion te sturen */
void SendToNextion(int waarde)
{
    char buffer[30];
    
    // Maak het commando klaar. 
    // LET OP: Zorg dat je Number component op je Nextion scherm 'n0' heet!
    sprintf(buffer, "page0.n6.val=%d", waarde); 
    
    // Stuur de tekst via UART naar het scherm
    UART_1_PutString(buffer);
    
    // Stuur de 3 verplichte afsluit-bytes (0xFF) zodat Nextion weet dat het commando klaar is
    UART_1_PutChar(0xFF);
    UART_1_PutChar(0xFF);
    UART_1_PutChar(0xFF);
}

int main(void)
{
    CyGlobalIntEnable; // Zet interrupts aan (veiligheid voor de UART communicatie)

    // Start de UART communicatie met het scherm
    UART_1_Start(); 

    // Initialisatie van de sensor pinnen
    CyDelay(500);
    Trigr_Write(0);
    CyDelay(500);
    
    for(;;)
    {
        count = 0;
        
        // 1. Stuur de 10 microseconde Trigger puls
        Trigr_Write(1);
        CyDelayUs(10);
        Trigr_Write(0);
        
        // 2. Wacht tot de geluidsgolf vertrekt (Echo wordt hoog)
        while(Echo_Read() == 0)
        {
            // Wachtloopje
        }
        
        // 3. Tel zolang de Echo pin hoog blijft (geluidsgolf is onderweg)
        while(Echo_Read() == 1)
        {
            count++; 
            CyDelayUs(1); // 1 microseconde wachten per tel, zorgt voor betrouwbaardere meting
        }
        
        // 4. Zet de ruwe 'count' tijd om naar Centimeters.
        // Standaard is de deler voor een HC-SR04 sensor ongeveer 58. 
        // (Is de afstand 3x te groot op je scherm? Verander 58 dan naar bijv. 150)
        afstand_cm = count / 58;
        
        // 5. Stuur de berekende afstand naar je Nextion display!
        SendToNextion(afstand_cm);
        
        // 6. Wacht een kwart seconde voordat we opnieuw meten.
        // Dit voorkomt dat je het display bombardeert met data waardoor hij vastloopt.
        CyDelay(250); 
    }
}
/* [] END OF FILE */