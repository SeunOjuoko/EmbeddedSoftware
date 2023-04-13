#ifndef __FREERTOS_H__ //Header guards prevents multiple inclusions of the same header file
#define __FREERTOS_H__ //If the symbol __FREERTOS_H__ has not been defined, then it is defined now.

#endif                          //This ends header guard
#if CONFIG_FREERTOS_UNICORE     //Sets ARDUINO_RUNNING_CORE based on CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif
#define FRAME_DURATION_MS 2     //Defines duration of each frame in 2ms

//Defines wrapper for FreeRTOS function vTaskDelay() that converts delayed time into ticks by portTICK_PERIOD_MS macro 
#define TaskDelay(t) (vTaskDelay(t/portTICK_PERIOD_MS))

//Pin definitions for Task 1
#define LED1 19                 //First LED (Pin 17)
//Pin definitions for Task 2
#define Inpulse1 6              //First inpulse (pin 6)
//Pin definitions for Task 3
#define Inpulse2 7              //Second inpulse (pin 7)
//Pin definitions for Task 4
#define AC 0                    //AC (pin 0)
#define LED2 18                 //Second LED (pin 18)
#define Switch 8                //Switch (pin 8)
#define LED3 1                  //Third LED (pin 1)

//Defines a data structure that stores... 
typedef struct Frequency {
    double Frequency1;        //FrequencyA as a double from Task 2
    double Frequency2;        //FrequencyB as a double from Task 3
} Frequency;

//Variables for Task 2
int Frequency1 = 0;           //First frequency is declared as an integer
//Variables for Task 3
int Frequency2 = 0;           //Second frequency is declared as an integer
//Variables for Task 5
int FrequencyValue1 = 0;      //First frequency value is declared as an integer
int FrequencyValue2 = 0;      //Second frequency value is declared as an integer

typedef unsigned char uint8;  //Defined character 

//Funtion Prototypes for each JobTask
void JobTask1(void *pvParameters);  
void JobTask2(void *pvParameters);
void JobTask3(void *pvParameters);
void JobTask4(void *pvParameters);
void JobTask5(void *pvParameters);
void JobTask6(void *pvParameters);
void JobTask7(void *pvParameters);

Frequency DataFrequency;                //Declared DataFrequency with Frequency Data Structure
SemaphoreHandle_t SemaphoreFrequency;   //Declared SemaphoreFrequency as a SemaphoreHandle_t type
QueueHandle_t EventQueue;               //Declared EventQueue as a QueueHandle_t type

//----------------------------------------------------------------------------------------------------------------------------------------------
void setup(void).
{
  //Initialises the serial communication to communicate with the PC
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Ready");

  //Task 1 Pin Assignment 
  pinMode(LED1, OUTPUT);     //Initialises the first LED as an output signal (Pin 17)
  //Task 2 Pin Assignment
  pinMode(Inpulse1, INPUT);  //Initialises the first impulse as an input signal (Pin 6)
  //Task 3 Pin Assignment
  pinMode(Inpulse2, INPUT);  //Initialises the second impulse as an input signal (Pin 5)
  //Task 4 Pin Assignments
  pinMode(AC, INPUT);        //Initialises the AC as an input signal (Pin 0)
  pinMode(LED2, OUTPUT);     //Initialises the second LED as an output signal (Pin 18)
  //Additional Pin Assignments
  pinMode(Switch, INPUT);    //Initialises the second LED as an output signal (Pin 18
  pinMode(LED3, OUTPUT);     //Initialises the third LED as an output signal (Pin 18) 

  //Initialised the Frequencies Task 2 and Task 3
  DataFrequency.Frequency1 = 0;
  DataFrequency.Frequency2 = 0;

  //Created Semaphores for JobTask 2, 3 and 5  
  SemaphoreFrequency = xSemaphoreCreateMutex();
  //Created a queue for JobTask 6 and 7
  EventQueue = xQueueCreate(1, sizeof(uint8));
  
  //xTaskCreate function creates the seven tasks
  xTaskCreate(JobTask1,"task1",512,(void*) 1,7,NULL); //1st Priority
  xTaskCreate(JobTask2,"task2",512,(void*) 1,5,NULL); //3rd Priority
  xTaskCreate(JobTask3,"task3",512,(void*) 1,6,NULL); //2nd Priority
  xTaskCreate(JobTask4,"task4",512,(void*) 1,4,NULL); //4th Priority
  xTaskCreate(JobTask5,"task5",512,(void*) 1,3,NULL); //5th Priority
  xTaskCreate(JobTask6,"task6",512,(void*) 1,2,NULL); //6th Priority
  xTaskCreate(JobTask7,"task7",512,(void*) 1,1,NULL); //7th Priority
  
    
}  
//----------------------------------------------------------------------------------------------------------------------------------------------
void loop(void)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------
// Period = 4ms / Rate = 250Hz
void JobTask1(void *pvParameters) 
{
  //Void input parameter 
  (void) pvParameters;
  //Infinite iterations runs continuously
  for(;;)
  {
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
    //Waits 4 ticks before repeating execution to avoid overloading system  
    TaskDelay(4);
  }
} 
//----------------------------------------------------------------------------------------------------------------------------------------------
// Period = 20ms / Rate = 50Hz
void JobTask2(void *pvParameters) 
//This is one of the longest tasks that is kept apart from Job Task 3
{
  //Void input parameter 
  (void) pvParameters;
  //Infinite iterations runs continuously
  for(;;)
  {
    int bT1 = micros();
    //Receieves first input signal from frequency generator
    int Cycle1 = pulseIn(Inpulse1, HIGH, 4000);
    //When the cycle is above 0 
    if(Cycle1 > 0){
    //Calculates the Frequency through the equation 
      Frequency1 = 1/(2*Cycle1*0.000001);
      if(Frequency1 < 333)
      {
        Frequency1 = 333;
      }
      if (Frequency1 > 1000)
      {
        Frequency1 = 1000;
      } 
    }
    //xSemaphoreTake function takes the semaphore indefinitely 
    if(xSemaphoreTake(SemaphoreFrequency, portMAX_DELAY) == pdTRUE) {
      //The calculated frequency is stored in DataFrequency.Frequency1
      DataFrequency.Frequency1 = Frequency1;
      //xSemaphoreGive releases the semaphore using semaphore 
      xSemaphoreGive(SemaphoreFrequency);
      Serial.println(Frequency1);
    }
    //Presents the first Frequency from Task 2 on the serial monitor
    Serial.printf("The Task 2 Frequency is now:", Frequency1);
    
    //Waits 20 ticks before repeating execution to avoid overloading system 
    TaskDelay(20);
  }
} 
//----------------------------------------------------------------------------------------------------------------------------------------------
// Period = 8ms / Rate = 125Hz
void JobTask3(void *pvParameters)
//This is one of the longest tasks that is kept apart from Job Task 2
{
  //Void input parameter 
  (void) pvParameters;
  //Infinite iterations runs continuously
  for(;;)
  { 
    int bT2 = micros();
    //Receieves second input signal from frequency generator
    int Cycle2 = pulseIn(Inpulse2, HIGH, 4000);
    //When the cycle is above 0 
    if(Cycle2 > 0){
    //Calculates the Frequency through the equation 
      Frequency2 = 1/(2*Cycle2*0.000001);
      if(Frequency2 < 333)
      {
        Frequency1 = 333;
      }
      if (Frequency2 > 1000)
      {
        Frequency2 = 1000;
      } 
    }
    //xSemaphoreGive releases the semaphore using semaphore 
    if(xSemaphoreTake(SemaphoreFrequency, portMAX_DELAY) == pdTRUE) {
      //The calculated frequency is stored in DataFrequency.Frequency2
      DataFrequency.Frequency2 = Frequency2;
      xSemaphoreGive(SemaphoreFrequency);
      //Serial.println(Frequency2);
    }
    //Presents the second Frequency from Task 3 on the serial monitor
    printf("The Task 3 Frequency is now: ", Frequency2);
    
    //Waits 8 ticks before repeating execution to avoid overloading system
    TaskDelay(8);
  }
} 
//----------------------------------------------------------------------------------------------------------------------------------------------
// Period = 20ms / Rate = 50Hz
void JobTask4(void *pvParameters) 
{
  //Void input parameter 
  (void) pvParameters;
  //Infinite iterations runs continuously
  for(;;)
  {
    //Measures the four inidvidual Analogue Readings and stored as float
    float R1 = analogRead(AC);  //Analogue Reading #1
    float R2 = analogRead(AC);  //Analogue Reading #2
    float R3 = analogRead(AC);  //Analogue Reading #3
    float R4 = analogRead(AC);  //Analogue Reading #4
    
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
    //Waits 20 ticks before repeating execution to avoid overloading system
    TaskDelay(20);
  }
} 
//----------------------------------------------------------------------------------------------------------------------------------------------
// Period = 100ms / Rate = 10Hz
void JobTask5(void *pvParameters)
{
  //Void pointer as input argument
  (void) pvParameters;
  for(;;)
  {
    //Waits until semaphore is available from xSemaphoreTake function by portMAX_DELAY
    if(xSemaphoreTake(SemaphoreFrequency, portMAX_DELAY) == pdTRUE) {
   
      //Map function re-maps the first Frequency from 333 to 0 and 1000 to 99
      FrequencyValue1 = map(DataFrequency.Frequency1,333,1000, 0, 99);
      //Map function re-maps the second Frequency from 500 to 0 and 1000 to 99
      FrequencyValue2 = map(DataFrequency.Frequency2,500,1000, 0, 99); 
      //Constrain function limits the first Frequency value between 0 and 99 
      FrequencyValue1 = constrain(FrequencyValue1,0,99);   
      //Constrain function limits the second Frequency value between 0 and 99 
      FrequencyValue2 = constrain(FrequencyValue2,0,99); 

      //Presents the FrequencyValue1 and FrequencyValue2 from Task 5 through the serial monitor
      Serial.printf("%d, %d \n", FrequencyValue2, FrequencyValue1); 
      xSemaphoreGive(SemaphoreFrequency);
    }
    //Waits 100 ticks before repeating execution to avoid overloading system
    TaskDelay(100);
  }
}

// Period = 10ms / Rate = 100Hz
void JobTask6(void *pvParameters) {
  //Void pointer arguement
  (void) pvParameters;
  //Infinite Iterations 
  for (;;) {
    //Reads digital state of switch (stored in uint8)
    uint8 Button = digitalRead(Switch);
    //Sends button value to EventQueue as pointer data is sent to &Button 
    xQueueSend(EventQueue, &Button, 10);
    //Waits 10 ticks before repeating execution to avoid overloading system
    TaskDelay(10);
  }
}

// Period = 8ms / Rate = 125Hz
void JobTask7(void *pvParameters) {
  //Void input parameter  
  (void) pvParameters;
  //Infinite iterations runs continuously
  for (;;) {
    //Button recieves data from the EventQueue by xQueueReceive function 
    uint8 Button = 0;
    //If data recieved from EventQueue within 10 ticks
    //The button value is updated with value recieved
    if(xQueueReceive(EventQueue, &Button, 10) == pdPASS) {
      //This value turns on the LED
      digitalWrite(LED3, Button);
    }
    //Waits 8 ticks before repeating execution to avoid overloading system.
    TaskDelay(8);
  }
}
