# OOPic18
* DPTP System - OOPic18 -> Object oriented code for PIC18
* title:       main program, VGM Player (SEGA GameGear, SEGA MesterSystem)
* autor:       DPTP System - Tóth Péter
* date:        2024.01.04.
* email:       don_peter[kukac]freemail[pont]hu
* device:      Special PIC18 device
* platform:    VS Code, MPLAB X, C Code, XC8
* MCU:         PIC18F4x2 and PIC18F46k22

# Leírás
Microchip PIC családból a PIC18F442, 452 és PIC18F46k22 MCU-ra 99%-ban felhúzható objektum orientált struktúrában írodott modul és driver teszt program. Követve a verziókat MPLAB X IDE-el és XC8-as fordító használatával valósult meg a kódtér.
A projekt drivere kifjezetten PIC18F452-re van oiptimalizálva, 442-nék a linker állomány változik és emiatt a memória területünk csak fele akkora. PIC18F46k22-nél pedig, mind a configurációs beállítás mind pedig az egyes regiszterek változhatnak ezért 
a drivert mindenkép illeszteni illetve áttírni szükséges. 

Megjegyzés: Sajnos a kivánt célt, hogy objektum orientált struktúrában készítsül el a VGM jeljátszónkat, nem lehetséges mert a PIC18-as családban a stack mérete korlátozza a driverek és modulok alkalmazását.

# Folyamat
A programozási és tesztelési feladatok folyamatosan fognak bővülni, függően a lehetőségeimtől. Egyes részek közt akár hetek vagy hónapok is eltellhetnek. A projekt várható befejezése nincs meghatározva, nem sürgős.

# Projekt haladás
* 2024.01.04. - alap program elkészült, fordul
* 2024.01.09. - IO driver alap IO elérésekhez elkészült
* 2024.01.10. - Megszakítások és timer0 1uS-es időzítés elkészült (callback miatt lehet szükséges később)
* 2024.01.12. - 1uS megszakítás beállítva és fő ciklusban egy 1mS-es feltétel ág.
* 2024.01.30. - uart megszakítás kezelése, beállítása. Ha az uart driver elkészül, akkor lehet csak véglegesíteni a megszakítás kezelést
* 2024.01.31. - uart driver hozzáadása a projekthez (még nincs befejezve)
* 2024.02.01. - Sajnos a PIC18-as család nem képes kezelni ezt a fajta driver struktúrális felépítést mivel túl kicsi a stack memóriája, így a projekt maximum a dio
driver szintjén ragad meg. Ebből is lehet bőven tanulni. A projekt folytatása egy fejlettebb MCU-ra fog átköltözni, amely ARM elapú lesz.

# Cél
* OO programozás és struktúrális programozás tanulása, elmélyítése
* Egy már korábban megírt és működő kis VGM lejátszó program ebben az OO struktúrára való átültetése
