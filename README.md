# MD_Parola_for_RaspberryPi
The RaspberryPi  CPP wrapper for using MD_Parola arduino library 

* For compile it needs MD_MAX72XX & MD_Parola source code from git.

https://github.com/MajicDesigns/MD_MAX72XX

https://github.com/MajicDesigns/MD_Parola



# GPIO pin-outs
The breakout board has two headers to allow daisy-chaining:

Board Pin	Name	  Remarks	      RPi Pin	    RPi Function
1	        VCC	    +5V Power	    2	          5V0
2	        GND	    Ground	      6	          GND
3	        DIN	    Data In	      19	        GPIO 10 (MOSI)
4	        CS	    Chip Select	  24	        GPIO 8 (SPI CE0)
5	        CLK	    Clock	        23	        GPIO 11 (SPI CLK)
