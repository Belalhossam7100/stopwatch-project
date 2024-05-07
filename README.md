![image](https://github.com/Belalhossam7100/stopwatch-project/assets/141184780/31219e36-0670-484b-a12c-6d2ad8bc432e)


Stopwatch System Implementation

Specifications:

Microcontroller: ATmega32 with 1MHz frequency.
Timer Configuration: Timer1 configured in CTC mode for stopwatch timekeeping.
Display: Six Common Anode 7-segment displays.
Multiplexing Technique: Utilizes one 7447 decoder and NPN BJT transistors to enable/disable each 7-segment display.
Display Control: 7447 decoder connected to first 4 pins in PORTC; enable/disable pins for 7-segments connected to first 6 pins in PORTA.
Operation: Stopwatch starts counting upon MCU power connection.
This project implements a stopwatch system using the specified components and techniques. Timer1 on the ATmega32 is utilized for accurate timekeeping, while multiplexing is employed to efficiently control the six 7-segment displays. By cycling through each display rapidly, the stopwatch appears to function seamlessly to the observer. 
