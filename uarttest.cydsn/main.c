#include "project.h"

int main(void)
{
    CyGlobalIntEnable; 
    
    /* 1. Start the UART */
    UART_1_Start();
    
    /* 2. Flash LED 3 times on startup so you know the PSoC is alive */
    for(int i=0; i<3; i++) {
        Pin_LED_Write(1);
        CyDelay(100);
        Pin_LED_Write(0);
        CyDelay(100);
    }

    uint8 rxData;

    for(;;)
    {
        /* Check if data is available */
        if (UART_1_GetRxBufferSize() > 0)
        {
            /* Read one byte */
            rxData = UART_1_GetByte();
            
            /* Logic: We saw on ESP32 that the first byte is 65 (0x65).
               If we receive 65, toggle the LED. 
            */
            if (rxData == 0x65) // 0x65 is Hex for 65
            {
                Pin_LED_Write( !Pin_LED_Read() );
                
                /* Optional: Clear buffer to ignore the trailing FF FF FF 
                   so we don't process "garbage" 
                */
                UART_1_ClearRxBuffer();
            }
        }
    }
}