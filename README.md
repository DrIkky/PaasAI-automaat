# Embedded Systems Project: PaasAI-automaat

Dit repository bevat de broncode voor het Embedded Systems project (Periode 2.2/2.3). 

## Projectbeschrijving
In dit project wordt een embedded systeem ontwikkeld met de **PSoC 5** als hoofdcontroller. Het systeem combineert actuatoren (servo's en een stappenmotor) met een HMI (Human Machine Interface) touchscreen en slimme sensoren. Uiteindelijk moet het systeem draaien op een **RTOS** (Real-Time Operating System).

## Hardware Setup
Het project maakt gebruik van de volgende componenten:

* **Microcontroller:** Infineon PSoC 5LP (CY8CKIT-059).
* **Display:** Nextion NX8048P070 (7.0" HMI Touch Display).
* **Actuatoren:** 4x SG90 Micro Servo (50Hz PWM) & 1x Nema 17 Stappenmotor (met TMC2209 driver).
* **Sensoren:** HC-SR04 Ultrasoonsensor & KY-008 Lasersensor.
* **Voeding:** Externe Meanwell voeding (5V & 12V, met gedeelde massa met PSoC).

### Pinout Configuratie (PSoC Creator)
| Component / Signaal | Pin op PSoC | Opmerking |
| :--- | :--- | :--- |
| **Rx_1** | P12[2] | UART Ontvangst (Touchscreen TX) |
| **Tx_1** | P12[3] | UART Verzending (Touchscreen RX) |
| **Pin_1** | P3[0] | PWM Aansturing Hopper 1 |
| **Pin_2** | P3[2] | PWM Aansturing Hopper 2 |
| **Pin_3** | P3[4] | PWM Aansturing Hopper 3 |
| **Pin_4** | P3[6] | PWM Aansturing Hopper 4 |
| **Pin_Step** | P0[7] | Stappenmotor Step-signaal |
| **Pin_Dir** | P0[6] | Stappenmotor Richting-signaal |
| **Pin_Trigger** | P15[4] | Ultrasoonsensor Trigger |
| **Pin_Echo** | P15[5] | Ultrasoonsensor Echo |
| **Pin_Laser** | P0[0] | Lasersensor Input (gekoppeld aan Verilog) |
| **Pin_LED** | P0[1] | Status LED Output (aangestuurd via Verilog) |
| **LED** | P2[1] | Extra status LED |
| **GND** | GND | **Cruciaal:** Verbind alle gronden (Common Ground)! |

## Software & Tools
* **IDE:** PSoC Creator 4.4
* **Interface Design:** Nextion Editor
* **Versiebeheer:** Git & GitHub Desktop 

## Projecteisen (Status)
Conform de projecthandleiding wordt er gewerkt aan de volgende eisen:

- [x] **PSoC 5 als hoofdrol:** De kern van het systeem.
- [x] **Versiebeheer:** Git repository is opgezet en schoon gehouden (via .gitignore).
- [x] **RTOS / Architectuur:** Basis interrupt-gedreven architectuur staat, migratie naar FreeRTOS loopt.
- [x] **HDL/Verilog:** Ei-detectie hardware component geschreven in Verilog en gekoppeld aan de lasersensor.
- [x] **Communicatie:** Eigen UART-afhandeling voor het scherm. (I2C/SPI gereserveerd voor latere sensoren).
- [ ] **Energiezuinig:** Low-power optimalisaties (implementatie in finale fase).

## Installatie & Gebruik
1. Clone deze repository naar je lokale machine.
2. Open `Workspace01.cywrk` in PSoC Creator.
3. Zorg dat de `.gitignore` actief is om gegenereerde bestanden buiten de repo te houden.
4. Klik op **Build -> Generate Application** om de drivers te genereren en de pin-routing toe te passen.
5. Flash de PSoC via de MiniProg3/KitProg (USB).

## Versiegeschiedenis
* **v0.2:** Hardware mapping compleet. Implementatie van stappenmotor, ultrasoonsensor en Verilog ei-detectie.
* **v0.1:** Initiële opzet. Servo (PWM) en Display (UART) drivers functioneel in bare-metal C.
