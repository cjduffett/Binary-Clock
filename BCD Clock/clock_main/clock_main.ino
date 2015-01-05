/*
Binary Coded Decimal (BCD) Clock
Carlton Duffett

Contact:
carlton.duffett@gmail.com
carltonduffett.com

Current Version:
v.1.0

*/

// Set current time here, format HHMM
// DO NOT include leading zeros
#define TIME 1200

// Time constants
#define SECOND 995 // program takes approx 5ms to execute
#define MOMENT 20

// pin mappings:
// ----------------
#define CLK_SEC   P1_0
#define CLK_SEC10 P2_6
#define CLK_MIN10 P2_7

#define MIN_1  P1_3
#define MIN_2  P1_4
#define MIN_4  P1_5
#define MIN_8  P1_6

#define HR_1   P2_0
#define HR_2   P2_1
#define HR_4   P2_2
#define HR_8   P2_3

#define HR10_1 P2_4
#define HR10_2 P2_5

// clock digits
unsigned short int second = 0;
unsigned short int second_10 = 0;
unsigned short int minute = 0;
unsigned short int minute_10 = 0;
unsigned short int hour = 0;
unsigned short int hour_10 = 0;

/********************************************/
/* INITIALIZATION                           */
/********************************************/

void setup()
{ 
  // open serial port
  Serial.begin(9600);
  
  // parse user-defined current time
  second = 0;
  second_10 = 0;
  minute = TIME % 10;
  minute_10 = (TIME % 100) / 10;
  hour = (TIME % 1000) / 100;
  hour_10 = (TIME % 10000) / 1000;  
  
  // SETUP OUTPUT PINS
  // ------------------------------------
  
  /*
  clk signal to BCD counters:
  BCD counters control second, second_10,
  and minute_10 LED arrays.
  */
  pinMode(CLK_SEC,OUTPUT);
  pinMode(CLK_SEC10,OUTPUT);
  pinMode(CLK_MIN10,OUTPUT);
  
  // pinouts for minute LED array
  pinMode(MIN_1,OUTPUT);
  pinMode(MIN_2,OUTPUT);
  pinMode(MIN_4,OUTPUT);
  pinMode(MIN_8,OUTPUT);
  
  // pinouts for hour LED array
  pinMode(HR_1,OUTPUT);
  pinMode(HR_2,OUTPUT);
  pinMode(HR_4,OUTPUT);
  pinMode(HR_8,OUTPUT);
  
  // pinouts for hour_10 LED array
  pinMode(HR10_1,OUTPUT);
  pinMode(HR10_2,OUTPUT);
  
  // initialize all pins to LOW
  digitalWrite(CLK_SEC,LOW);
  digitalWrite(CLK_SEC10,LOW);
  digitalWrite(CLK_MIN10,LOW);
  digitalWrite(MIN_1,LOW);
  digitalWrite(MIN_2,LOW);
  digitalWrite(MIN_4,LOW);
  digitalWrite(MIN_8,LOW);
  digitalWrite(HR_1,LOW);
  digitalWrite(HR_2,LOW);
  digitalWrite(HR_4,LOW);
  digitalWrite(HR_8,LOW);
  digitalWrite(HR10_1,LOW);
  digitalWrite(HR10_2,LOW);
  
  // SETUP BCD COUNTERS
  // ------------------------------------
  
  /*
  Pulse BCD clk signals to set time
  shown by respective LED arrays. Max
  clk frequency of counters is 16MHz.
  After setup, clk pins will initially 
  be set to LOW.
  
  All counters initialize to 1 after setup.
  I therefore reduce the loop by 1 iteration.
  */
  
  unsigned short int i = 0;
  unsigned short int limit;
  
  // setup seconds counter (cycle back to zero)
  for (i = 0; i < 9; i++)
  {
    digitalWrite(CLK_SEC,HIGH);
    delay(MOMENT); // a negligible unit of time
    digitalWrite(CLK_SEC,LOW);
    delay(MOMENT);
  }
  
  // setup second_10 counter (cycle back to zero)
  for (i = 0; i < 5; i++)
  {
    digitalWrite(CLK_SEC10,HIGH);
    delay(MOMENT); // a negligible unit of time
    digitalWrite(CLK_SEC10,LOW);
    delay(MOMENT);
  }
   
  // setup minute_10 counter
  if (minute_10 == 0) {
    limit = 6; // needed to cycle back to zero
  } else {
    limit = minute_10;
  }
  
  for (i = 1; i < limit; i++)
  {
    digitalWrite(CLK_MIN10,HIGH);
    delay(MOMENT); // a negligible unit of time
    digitalWrite(CLK_MIN10,LOW);
    delay(MOMENT);
  }
  
  // SETUP LED ARRAYS
  // ------------------------------------
  
  // minute array
  if (minute & 1)
    digitalWrite(MIN_1,HIGH);
  
  if (minute & 2)
    digitalWrite(MIN_2,HIGH);
    
  if (minute & 4)
    digitalWrite(MIN_4,HIGH);
    
  if (minute & 8)
    digitalWrite(MIN_8,HIGH);
    
  // hour array
  if (hour & 1)
    digitalWrite(HR_1,HIGH);
  
  if (hour & 2)
    digitalWrite(HR_2,HIGH);
    
  if (hour & 4)
    digitalWrite(HR_4,HIGH);
    
  if (hour & 8)
    digitalWrite(HR_8,HIGH);
    
  // hour_10 array
  if (hour_10 & 1)
    digitalWrite(HR10_1,HIGH);
  
  if (hour_10 & 2)
    digitalWrite(HR10_2,HIGH);  
  
  // wait second at :00
  delay(SECOND);
  
} // END INITIALIZATION

/********************************************/
/* CLOCK CONTROLLER                         */
/********************************************/

void loop()
{ 
  // prevent interrupts
  noInterrupts();
    
  // RESET NON-BCD LED ARRAYS
  // ------------------------------------
  
  // minutes
  digitalWrite(MIN_1,LOW);
  digitalWrite(MIN_2,LOW);
  digitalWrite(MIN_4,LOW);
  digitalWrite(MIN_8,LOW);
  
  // hours
  digitalWrite(HR_1,LOW);
  digitalWrite(HR_2,LOW);
  digitalWrite(HR_4,LOW);
  digitalWrite(HR_8,LOW);
  
  // hour_10
  digitalWrite(HR10_1,LOW);
  digitalWrite(HR10_2,LOW);
  
  
  // SECONDS
  // ------------------------------------
  
  // increment seconds counter
  digitalWrite(CLK_SEC,HIGH);
  second++;
  
  // evaluate second_10 counter
  if (second > 9) {
    second = 0;
    second_10++;
    digitalWrite(CLK_SEC10,HIGH);
  }
  
  // MINUTES
  // ------------------------------------
  
  // increment minute after 60 seconds
  if (second_10 > 5) {
    second_10 = 0;
    minute++;
    
    // evaluate minute_10 counter
    if (minute > 9) {
      minute = 0;
      minute_10++;
      digitalWrite(CLK_MIN10,HIGH);
    }  
  }
    
  // minute array
  if (minute & 1)
    digitalWrite(MIN_1,HIGH);
  
  if (minute & 2)
    digitalWrite(MIN_2,HIGH);
    
  if (minute & 4)
    digitalWrite(MIN_4,HIGH);
    
  if (minute & 8)
    digitalWrite(MIN_8,HIGH);  
  
  // HOURS
  // ------------------------------------
  
  // imcrement hour after 60 minutes
  if (minute_10 > 5) {
    minute_10 = 0;
    hour++;
    
    // reset after 12 hours
    if (hour_10 == 1 && hour > 2) {
      hour = 1;
      hour_10 = 0; 
    }  
  }
  
  // increment hour_10 counter after 10 hours
  if (hour > 9) {
     hour = 0;
     hour_10++;
  }
    
  // hour array
  if (hour & 1)
    digitalWrite(HR_1,HIGH);
  
  if (hour & 2)
    digitalWrite(HR_2,HIGH);
    
  if (hour & 4)
    digitalWrite(HR_4,HIGH);
    
  if (hour & 8)
    digitalWrite(HR_8,HIGH);
    
  // hour_10 array
  if (hour_10 & 1)
    digitalWrite(HR10_1,HIGH);
  
  if (hour_10 & 2)
    digitalWrite(HR10_2,HIGH);
  
  // FINISH
  // ------------------------------------

  // set CLK signals LOW for next pass
  digitalWrite(CLK_SEC,LOW);
  digitalWrite(CLK_SEC10,LOW);
  digitalWrite(CLK_MIN10,LOW);
  
  // print current internal time to monitor
  Serial.print(hour_10);
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute_10);
  Serial.print(minute);
  Serial.print(":");
  Serial.print(second_10);
  Serial.print(second);
  Serial.println("");
  
  // wait for one second
  delay(SECOND);
  
} // END CONTROLLER LOOP

