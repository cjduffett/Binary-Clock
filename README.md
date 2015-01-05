Binary Clock
================================

Using a **TI MSP430 G2553** microcontroller, this is a 12-hour LED clock where each digit is encoded separately using 4-digit binary numbers. This is commonly known as binary coded decimal (BCD). The clock displays hours, minutes and seconds in the following format:

	   X      X      X
       X   X  X   X  X
    X  X   X  X   X  X
    X  X   X  X   X  X
    ------------------
    H  H : M  M : S  S

The circuitry makes use of three BCD upcounters:

 2x **NTE74LS92** Divide-by-Six up/down counter  
 1x **SN7493AN** Decade Divide-by-Twelve up counter

Each of these counters is controlled by a clock pulse from the MSP430. This allows only 3 pins from the microcontroller to drive a total of 10 LEDs. The remaining LEDs in the array are driven by individual pins. To keep track of the controller's internal time, the current time displayed by the LEDs is also sent via serial port to a computer. This serial output can be monitored to debug the clock's operation and to verify the correct time.

The software for this project was developed using the open source **[Energia](http://energia.nu/ "The Energia homepage")** API for Arduino and TI microcontrollers. This API dramatically simplified programming the microcontroller and offers many great features.

![The Energia logo](http://dangerousprototypes.com/wp-content/media/2012/08/Energia.png)