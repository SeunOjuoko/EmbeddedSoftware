#include <B31DGMonitor.h>
#include <Ticker.h>
#include <Arduino.h>


#define FRAME_DURATION_MS 2     // 2ms
//For Task1
#define Signal1 5
//For Task2
#define Inpulse2 6 

B31DGCyclicExecutiveMonitor monitor;
Ticker FrameTick;

unsigned long frameTime = 0;
unsigned long frameCounter = 0;
int bt = 0;
int Cycle2 = 0;
int Frequency2 = 0;

//----------------------------------------------------------------------------------------------------------------------------------------------
void setup(void)
{
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Ready");

  //Assigned for task1 
  pinMode(Signal1, OUTPUT);
  //Assigned for tasK2
  pinMode(Inpulse2, INPUT);
    
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

// Task 2, takes 1.8ms
void JobTask2(void) 
{
  monitor.jobStarted(2);
  int bT = micros();
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

// Task 3, takes 1ms
void JobTask3(void) 
{
   monitor.jobStarted(3);
   delayMicroseconds(1000);
   monitor.jobEnded(3); 
} 

// Task 4, takes 2ms
void JobTask4(void) 
{
   monitor.jobStarted(4);
   delayMicroseconds(2000);
   monitor.jobEnded(4); 
} 
