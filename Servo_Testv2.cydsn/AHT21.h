/* ========================================
 * AHT21B Temperatuur & Vochtigheid Library
 * ========================================*/
#ifndef AHT21_H
#define AHT21_H

#include "cytypes.h" // Nodig voor de standaard PSoC datatypes

// Functies die door de main.c gebruikt mogen worden
void AHT21_Start(void);
int AHT21_MeetTemperatuur(void);

#endif /* AHT21_H */