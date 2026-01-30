# Embedded Systems Project: PaasAI-automaat

Dit repository bevat de broncode voor het Embedded Systems project (Periode 2.2/2.3). 

## Projectbeschrijving
[cite_start]In dit project wordt een embedded systeem ontwikkeld met de **PSoC 5** als hoofdcontroller[cite: 53]. [cite_start]Het systeem combineert actuatoren (servo's) met een HMI (Human Machine Interface) en moet uiteindelijk draaien op een **RTOS** (Real-Time Operating System)[cite: 58].

## Hardware Setup
Het project maakt gebruik van de volgende componenten:

* **Microcontroller:** Infineon PSoC 5LP (CY8CKIT-059).
* **Display:** Nextion NX8048P070 (7.0" HMI Touch Display).
* **Actuator:** SG90 Micro Servo (50Hz PWM).
* **Voeding:** Externe 5V voeding (Gedeelde massa met PSoC).

### Pinout Configuratie
| Component | Pin op PSoC | Opmerking |
| :--- | :--- | :--- |
| **Servo PWM** | P2[1] | Aansturing (50Hz) |
| **Nextion TX** | P12[6] (RX) | UART Ontvangst |
| **Nextion RX** | P12[7] (TX) | UART Verzending |
| **GND** | GND | **Cruciaal:** Verbind alle gronden! |

## Software & Tools
* **IDE:** PSoC Creator 4.4
* **Interface Design:** Nextion Editor
* [cite_start]**Versiebeheer:** Git & GitHub Desktop [cite: 12]

## Projecteisen (Status)
Conform de projecthandleiding wordt er gewerkt aan de volgende eisen:

- [x] [cite_start]**PSoC 5 als hoofdrol:** De kern van het systeem[cite: 53].
- [x] [cite_start]**Versiebeheer:** Git repository is opgezet en schoon gehouden (via .gitignore)[cite: 12].
- [ ] [cite_start]**RTOS:** Implementatie van FreeRTOS (in ontwikkeling)[cite: 58].
- [ ] [cite_start]**HDL/Verilog:** Een hardware component beschreven in Verilog (moet nog ontworpen worden)[cite: 56].
- [ ] [cite_start]**Communicatie:** Eigen driver voor I2C/SPI (later stadium)[cite: 60].
- [ ] [cite_start]**Energiezuinig:** Low-power optimalisaties[cite: 59].

## Installatie & Gebruik
1.  Clone deze repository naar je lokale machine.
2.  Open `Workspace01.cywrk` in PSoC Creator.
3.  Zorg dat de `.gitignore` actief is om gegenereerde bestanden buiten de repo te houden.
4.  Klik op **Build -> Generate Application** om de drivers te genereren.
5.  Flash de PSoC via de MiniProg3/KitProg (USB).

## Versiegeschiedenis
* **v0.1:** Initiële opzet. Servo (PWM) en Display (UART) drivers functioneel in bare-metal C.
