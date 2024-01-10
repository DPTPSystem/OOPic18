# OOPic18
* DPTP System - OOPic18 -> Object oriented code for PIC18
* title:       main program, VGM Player (SEGA GameGear, SEGA MesterSystem)
* autor:       DPTP System - Tóth Péter
* date:        2024.01.04.
* email:       don_peter[kukac]freemail[pont]hu
* device:      PIC18
* platform:    C Code, MPLAB X
* MCU:         PIC18F4x2 and PIC18F46k22

# Leírás
Microchip PIC családból a PIC18F442, 452 és PIC18F46k22 MCU-ra 99%-ban felhúzható objektum orientált struktúrában írodott modul és driver teszt program. Követve a verziókat MPLAB X IDE-el és XC8-as fordító használatával valósult meg a kódtér.
A projekt drivere kifjezetten PIC18F452-re van oiptimalizálva, 442-nék a linker állomány változik és emiatt a memória területünk csak fele akkora. PIC18F46k22-nél pedig, mind a configurációs beállítás mind pedig az egyes regiszterek változhatnak ezért 
a drivert mindenkép illeszteni illetve áttírni szükséges. 

# Folyamat
A programozási és tesztelési feladatok folyamatosan fognak bővülni, függően a lehetőségeimtől. Egyes részek közt akár hetek vagy hónapok is eltellhetnek. A projekt várható befejezése nincs meghatározva, nem sürgős.

# Projekt halladás
* 2024.01.04. - alap program elkészült, fórdul
* 2024.01.09. - IO driver alap IO elérésekhez elkészült
* 2024.01.10. - Megszakítások és timer0 1uS-es időzítés elkészült (callback miatt lehet szükséges később)

# Cél
* OO programozás és struktúrális programozás tanulása, elmélyítése
* Egy már korábban megírt és működő kis VGM lejátszó program ebben az OO struktúrára való átültetése
