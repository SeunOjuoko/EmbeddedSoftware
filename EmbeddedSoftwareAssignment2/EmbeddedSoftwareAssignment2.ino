#include <B31DGMonitor.h>       //Enables monitor function
#include <Ticker.h>             //Enables Ticker function
#include <Arduino.h>

#define FRAME_DURATION_MS 2     // 2ms

//Pin definitions for Task 1
#define LED1 19                 //First LED (Pin 17)
//Pin definitions for Task 2
#define Inpulse1 6              //First inpulse (pin 6)
//Pin definitions for Task 3
#define Inpulse2 7              //Second inpulse (pin 7)
//Pin definitions for Task 4
#define AC 0                    //AC (pin 0)
#define LED2 18                 //Second LED (pin 18)

B31DGCyclicExecutiveMonitor monitor;
Ticker FrameTick;

//Declares Long variable for the frame procedure
unsigned long frameTime = 0;              
unsigned long frameCounter = 0;           //Initialises Frame Counter
//Variables for Task 2
int Frequency1 = 0;       //First frequency is declared as an integer
//Variables for Task 3
int Frequency2 = 0;       //Second frequency is declared as an integer
//Variables for Task 5
int FrequencyValue1 = 0;  //First frequency value is declared as an integer
int FrequencyValue2 = 0;  //Second frequency value is declared as an integer

//----------------------------------------------------------------------------------------------------------------------------------------------
void setup(void)
{
  //Initialises the serial communication to communicate with the PC
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Ready");

  //Assigned for task1 
  pinMode(LED1, OUTPUT);      //Initialises the first LED as an output signal (Pin 17)
  //Assigned for task2
  pinMode(Inpulse1, INPUT);   //Initialises the first impulse as an input signal (Pin 6)
  //Assigned for task3
  pinMode(Inpulse2, INPUT);   //Initialises the second impulse as an input signal (Pin 5)
  //Assigned for task4
  pinMode(AC, INPUT);        //Initialises the AC as an input signal (Pin 0)
  pinMode(LED2, OUTPUT);     //Initialises the second LED as an output signal (Pin 18)
  
  //Defined the ticker size as 4ms for the frame function
  FrameTick.attach_ms(4, frame); 
  //Begins with the Job Task1 that has the fixed delay of 2ms
  JobTask1();
  delay(2);
  //Begins the start monitoring scheme
  monitor.startMonitoring();      
    
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//Frame organises the job tasks based on their seqeunce (no interference)
void frame() {
  //Frame positions range from case 0 to 9
  unsigned int position = frameCounter % 10;
    //Iterates through the case statement to operate the Job Tasks based their respected case position 
   switch (position) {  
     case 0: JobTask1();              JobTask3(); JobTask4(); JobTask5(); break;
     case 1: JobTask1();  JobTask2();                                     break;
     case 2: JobTask1();              JobTask3();                         break;
     case 3: JobTask1();                                                  break;
     case 4: JobTask1();              JobTask3();                         break;
     case 5: JobTask1();                          JobTask4(); JobTask5(); break;
     case 6: JobTask1();              JobTask3();                         break;
     case 7: JobTask1(); JobTask2();                                      break;
     case 8: JobTask1();              JobTask3();                         break;
     case 9: JobTask1();                                                  break;
  }
  //Frame counter increments until the maximum value 
  frameCounter = frameCounter + 1; 
}
//----------------------------------------------------------------------------------------------------------------------------------------------
void loop(void) // Single time slot function of the Cyclic Executive (repeating)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------
// Task 1, takes 4ms
void JobTask1(void) 
{
  //Starts the first job task
  monitor.jobStarted(1);
  //Turns on the first LED for 200μs
  digitalWrite(LED1, HIGH);
  delayMicroseconds(200); 
  //Turns off the first LED for 50μs
  digitalWrite(LED1, LOW);
  delayMicroseconds(50);
  //Turns on the first LED for 30μs
  digitalWrite(LED1, HIGH);
  delayMicroseconds(30);
  //Turns on the first LED
  digitalWrite(LED1, LOW);
  //Ends the first job task
  monitor.jobEnded(1);
} 
//----------------------------------------------------------------------------------------------------------------------------------------------
void JobTask2(void) 
//This is one of the longest tasks that is kept apart from Job Task 3
{
  //Starts the second job task
  monitor.jobStarted(2);
  int bT1 = micros();
  int Cycle1 = pulseIn(Inpulse1, HIGH, 4000);
  //When the cycle is above 0 
  if(Cycle1 > 0){
  //Calculates the Frequency through the equation 
  Frequency1 = 1/(2*Cycle1*0.000001);
  }else{
    //If lower then the frequency is set to 333
    Frequency1 = 333;
  }
  //Presents the first Frequency from Task 2 on the serial monitor
  Serial.printf("The Task 3 Frequency is now:", Frequency1);
  //Ends the second job task
  monitor.jobEnded(2);
} 
//----------------------------------------------------------------------------------------------------------------------------------------------
void JobTask3(void)
//This is one of the longest tasks that is kept apart from Job Task 2
{
  //Starts the third job task
   monitor.jobStarted(3);
  int bT2 = micros();
  //PulseIn function inputs the cycle from the frequency generator
  int Cycle2 = pulseIn(Inpulse2, HIGH, 3000);
  //When the cycle is above 0 
  if(Cycle2 > 0){
    //Calculates the Frequency through the equation
    Frequency2 = 1/(2*Cycle2*0.000001);
  }else{
    //If lower then the frequency is set to 500
    Frequency2 = 500;
  }
  //Presents the second Frequency from Task 3 on the serial monitor
  Serial.printf("The Task 3 Frequency is now: ", Frequency2);
  //Ends the third job task
  monitor.jobEnded(3);
} 
//----------------------------------------------------------------------------------------------------------------------------------------------
// Task 4
void JobTask4(void) 
{
  //Starts the fourth job task
  monitor.jobStarted(4);
  //Measures the four inidvidual analogue readings and stored as float
  float R1 = analogRead(AC);  
  float R2 = analogRead(AC);  
  float R3 = analogRead(AC);
  float R4 = analogRead(AC);
  
  //Calculate the Average value of the 4 values and convert to voltage value
  float Average = (R1 + R2 + R3 + R4)/(4*4095/3.3);
  //Presents the Average from Task 4 through the serial monitor
  Serial.println(Average);
  
  //When the Average is above 1.65V...
  if (Average >= 1.65){      
    //Momentarily on second LED 
    digitalWrite(LED2, HIGH);                   
  } else {
    digitalWrite(LED2, LOW); 
  }

  //Ends the fourth job task
   monitor.jobEnded(4); 
} 
//----------------------------------------------------------------------------------------------------------------------------------------------
void JobTask5(void)
{
  //This Job Task is operates each 5 frames (every 25, 50, 75, 100)
   if(frameCounter % 25 == 0){           //Run task only at intervals of 100us
    //Starts the fifth job task
    monitor.jobStarted(5);
    //Map function re-maps the first Frequency from 333 to 0 and 1000 to 99
    FrequencyValue1 = map(Frequency1,333,1000, 0, 99);
    //Map function re-maps the second Frequency from 500 to 0 and 1000 to 99
    FrequencyValue2 = map(Frequency2,500,1000, 0, 99); 
    //Constrain function limits the first Frequency value between 0 and 99 
    FrequencyValue1 = constrain(FrequencyValue1,0,99);   
    //Constrain function limits the second Frequency value between 0 and 99 
    FrequencyValue2 = constrain(FrequencyValue2,0,99); 

    //Presents the FrequencyValue1 and FrequencyValue2 from Task 5 through the serial monitor
    Serial.printf("%d, %d \n", FrequencyValue2, FrequencyValue1); 
    //Ends the fifth job task
    monitor.jobEnded(5);
   }
}
