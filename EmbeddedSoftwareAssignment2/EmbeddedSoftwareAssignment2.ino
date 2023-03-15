#include <B31DGMonitor.h>
#include <Ticker.h>
#include <Arduino.h>


#define FRAME_DURATION_MS 2     // 2ms
//For Task1
#define Signal1 17
//For Task2
#define Inpulse2 6 
//For Task2
#define Inpulse3 5 

B31DGCyclicExecutiveMonitor monitor;
Ticker FrameTick;

unsigned long frameTime = 0;
unsigned long frameCounter = 0;
//Task2
int bt2 = 0;
int Cycle2 = 0;
int Frequency2 = 0;
//Task3
int bt3 = 0;
int Cycle3 = 0;
int Frequency3 = 0;
//

//----------------------------------------------------------------------------------------------------------------------------------------------
void setup(void)
{
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Ready");

  //Assigned for task1 
  pinMode(Signal1, OUTPUT);
  //Assigned for task2
  pinMode(Inpulse2, INPUT);
  //Assigned for task3
  pinMode(Inpulse3, INPUT);
  //Assigned for task4

    
}
//----------------------------------------------------------------------------------------------------------------------------------------------
void frame() {
   
   unsigned int slot = frameCounter % 10;

   switch (slot) {
     case 0: JobTask1();             JobTask3();               break;
     case 1:             JobTask2();                           break;
     case 2: JobTask1();                                       break;
     case 3:                                     JobTask4();   break;
     case 4:             JobTask2();                           break;
     case 5: JobTask1();                                       break;
     case 6:             JobTask2();                           break;
     case 7: JobTask1();                                       break;
     case 8: JobTask1();                                       break;
     case 9:             JobTask2();                                 
  }
    
}

//----------------------------------------------------------------------------------------------------------------------------------------------
void loop(void) // Single time slot function of the Cyclic Executive (repeating)
{

  // uncomment this to find execution time of given tasks 
  /*
  unsigned long bT = micros();
  for (int i=0; i<1000; i++) {
    JobTask1();
  }
  unsigned long timeItTook = micros()-bT;
  Serial.print("Duration SerialOutput Job = ");
  Serial.print(timeItTook);
  exit(0);
  */

  JobTask2();
  //frame();

  // TO-DO: wait the next frame  
}

//----------------------------------------------------------------------------------------------------------------------------------------------
// Task 1, takes 0.9ms
void JobTask1(void) 
{
  monitor.jobStarted(1);
  digitalWrite(Signal1, HIGH);
  delayMicroseconds(200); // Wait for 1000 millisecond(s)
  digitalWrite(Signal1, LOW);
  delayMicroseconds(50);
  digitalWrite(Signal1, HIGH);
  delayMicroseconds(30); // Wait for 1000 millisecond(s)
  digitalWrite(Signal1, LOW);
  delayMicroseconds(3720); 
  monitor.jobEnded(1);
} 
//----------------------------------------------------------------------------------------------------------------------------------------------

// Task 2
void JobTask2(void) 
{
  monitor.jobStarted(2);
  int bT2 = micros();
  Cycle2 = pulseIn(Inpulse2, HIGH);
  if(Cycle2 > 0){
  Frequency2 = 1/(2*Cycle2*0.000001);
  }else{
    Frequency2 = 333;
  }
  Serial.println("Task 2 Frequency is now: ");
  Serial.println(Frequency2);
  monitor.jobEnded(2);
} 

// Task 3
void JobTask3(void) 
{
   monitor.jobStarted(3);
  int bT3 = micros();
  Cycle3 = pulseIn(Inpulse3, HIGH);
  if(Cycle3 > 0){
  Frequency3 = 1/(2*Cycle3*0.000001);
  }else{
    Frequency3 = 500;
  }
  Serial.println("Task 3 Frequency is now: ");
  Serial.println(Frequency3);
  monitor.jobEnded(3);
} 

// Task 4
void JobTask4(void) 
{
  monitor.jobStarted(4);
  int R1 = analogRead(PWM);  //Measure four seperate analogue readings
  int R2 = analogRead(PWM);  //Store each one in a seperate float
  int R3 = analogRead(PWM);
  int R4 = analogRead(PWM);
  
  //Then caclulate average of the 4 values and convert to voltage value
  float Average = (R1 + R2 + R3 + R4)/(4*4095/3.3);
  Serial.println(Average);

  if (Average >= 1.65){      //light LED when the voltage is above 1.65V
  //This should be LED two (green)
    digitalWrite(LED1, HIGH);
    delay(100);
    digitalWrite(LED1, LOW);                       //Turn off LED when voltage is below 1.65V
  }
   monitor.jobEnded(4); 
} 
