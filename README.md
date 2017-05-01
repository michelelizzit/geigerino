# Geigerino - Open-source low-power Arduino geiger

More informations about the project are available on the project website: https://lizzit.it/geiger

## Description of the project
Geigerino is a simple low-power geiger counter.  
The geiger blinks when a particle is detected and, if the buzzer enabled, it makes a "tick" sound.  
Geigerino can be easily connected to [PhyLab 2.0](https://lizzit.it/phylab) or used standalone.  
With a few hardware modifications Geigerino can be powered by a single 18650 li-ion battery for years, depending on the average amount of radiation measured (most of the battery power is used to power the LED that blinks when a particle is detected).  
In order to reduce the power consumption to the minimum it is necessary to use an Atmega328 (not an Arduino board), and to use a low quiescent current regulator (or no regulator at all, an Atmega328 can be powered directly by a single cell li-ion battery like the 18650); if power is not a concern (like most applications that do not require a year-long battery life) a regular Arduino can be used.  
Geigerino relies on a Theremino Geiger Adapter (approx 22€) in order to connect to the geiger probe.  
Future releases of Geigerino will allow the connection with a smartphone via Bluetooth LE.  

An upgraded version of the project, that includes a display and configurable software, will be added soon to the repository.  

## Schematics

![Schematics](/schematic.png)

| Arduino pin | |
|---|---|
| 2 | Signal from Theremino Geiger Adapter |
| 4 | Button (to vcc) |
| A0 | Button LED |
| 6 | Blue LED |
| 7 | Green LED |
| 10 | Piezo buzzer |

