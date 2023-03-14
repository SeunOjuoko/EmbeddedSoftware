//Pin assignments for the two push buttons and LEDs to the ESP32 
const int pushButton1 = 4; //First switch at pin 4 (input)
const int pushButton2 = 5; //Second switch at pin 5 (input)
const int signalA = 18;    //First LED at pin 18 (output)
const int signalB = 19;    //Second LED at pin 19 (output)

//Calculations for parameters as integers
int a = 1200;   //(O) 12 * 100 = 1200μs
int b = 1000;   //(J) 10 * 100 = 1000μs
int c = 10;     //(U) 6 + 4 = 10 pulses
int d = 6000;   //(O) 12 * 500 = 6000μs

void setup()
{
  pinMode(pushButton1, INPUT);    //Initialises the first push button as an input signal (Pin 4)
  pinMode(pushButton2, INPUT);    //Initialises the second push button as an input signal (Pin 5)  
  pinMode(signalA, OUTPUT);       //Initialises the first LED as an output signal (Pin 18)
  pinMode(signalB, OUTPUT);       //Initialises the second LED as an output signal (Pin 19)
}

void loop()
{   
  //Mode 4: Half d and b time until switch set back to 0 
  //(K) 11 % 4 = 3 + 1 = 4 (Mode 4)
  if (digitalRead(pushButton2) == HIGH)
  //Conditional statement for the second push button that..
  {
    //Halves the values of "b" and "d" for the delays in the next cycle if the second push button is pressed
    b = 500;    
    d = 3000;   
  } else {
    //Reverts "b" and "d" back to the original values for the next cycle if the second push button is not pressed
    b = 1000;
    d = 6000;
  }
   
  //As first push button is not pressed...
  if(digitalRead(pushButton1) == LOW)
  {  
    //This turns on the second LED for 50μs
    digitalWrite(signalB, HIGH);
    delayMicroseconds(50);
    digitalWrite(signalB, LOW);

    //Conditional loop iterates through c times (10 pulses)
    for(int i = 1; i<=c; i++)
    {
      //This turns on the first LED for "a" amount of time (1200μs)
      digitalWrite(signalA, HIGH);
      delayMicroseconds(a);
      digitalWrite(signalA, LOW);
      //LEDs turn off for "b" amount of time (1000μs)
      delayMicroseconds(b);
      //Each iteration increments the pulse by "a" as 50μs
      a = a + 50;
    }
    //Delay continues for "d" amount of time (6000μs)
    delayMicroseconds(d);
    //After the iterations the integer "a" resets to the original 1200μs
    a = 1200;
  }
}
