![image](https://github.com/Belalhossam7100/stopwatch-project/assets/141184780/31219e36-0670-484b-a12c-6d2ad8bc432e)



Overview:
This project implements a stopwatch system using an ATmega32 microcontroller running at 1MHz. The stopwatch functionality is achieved by configuring Timer1 in CTC mode. The system features six Common Anode 7-segment displays, controlled using multiplexing techniques with a 7447 decoder and NPN BJT transistors. Each display's enable/disable pins are connected to the first 6 pins of PORTA, while the 7447 decoder is connected to the first 4 pins of PORTC. Upon power connection to the MCU, the stopwatch starts counting, providing accurate timekeeping functionality.
