/* ========================================
 * AHT21B Temperatuur & Vochtigheid Library
 * ========================================*/
#include "project.h" // Geeft toegang tot de I2C_1 componenten
#include "AHT21.h"   // Koppel deze .c file aan onze eigen header

#define AHT21_I2C_ADRES 0x38

/* Functie om de sensor even wakker te laten worden na opstarten */
void AHT21_Start(void)
{
    CyDelay(100); // Wacht 100ms zodat de sensor veilig is opgestart
}

/* Functie die met de I2C praat, rekent, en de temperatuur (x10) teruggeeft */
int AHT21_MeetTemperatuur(void)
{
    uint8 data[6];
    uint32 rawTemp = 0;
    float tempCelsius = 0.0;

    // 1. STUUR HET 'MEET' COMMANDO NAAR DE SENSOR
    I2C_1_MasterSendStart(AHT21_I2C_ADRES, I2C_1_WRITE_XFER_MODE);
    I2C_1_MasterWriteByte(0xAC); 
    I2C_1_MasterWriteByte(0x33); 
    I2C_1_MasterWriteByte(0x00); 
    I2C_1_MasterSendStop();

    // 2. WACHT TOT DE METING KLAAR IS
    CyDelay(80); 

    // 3. LEES DE 6 BYTES AAN DATA UIT
    I2C_1_MasterSendStart(AHT21_I2C_ADRES, I2C_1_READ_XFER_MODE);
    
    data[0] = I2C_1_MasterReadByte(I2C_1_ACK_DATA); 
    data[1] = I2C_1_MasterReadByte(I2C_1_ACK_DATA); 
    data[2] = I2C_1_MasterReadByte(I2C_1_ACK_DATA); 
    data[3] = I2C_1_MasterReadByte(I2C_1_ACK_DATA); 
    data[4] = I2C_1_MasterReadByte(I2C_1_ACK_DATA); 
    data[5] = I2C_1_MasterReadByte(I2C_1_NAK_DATA); // Laatste is NAK
    I2C_1_MasterSendStop();

    // 4. BEREKEN DE TEMPERATUUR
    rawTemp = ((uint32)(data[3] & 0x0F) << 16) | ((uint32)data[4] << 8) | data[5];
    tempCelsius = ((float)rawTemp / 1048576.0) * 200.0 - 50.0;

    // Return als heel getal (bijv 21.5 graden = 215)
    return (int)(tempCelsius * 10.0);
}