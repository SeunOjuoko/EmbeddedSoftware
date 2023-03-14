//Task 1
int LED = 19;

void setup()
{
  pinMode(LED, OUTPUT);
}

void loop()
{
  digitalWrite(LED, HIGH);
  delayMicroseconds(200); // Wait for 1000 millisecond(s)
  digitalWrite(LED, LOW);
  delayMicroseconds(50);
  digitalWrite(LED, HIGH);
  delayMicroseconds(30); // Wait for 1000 millisecond(s)
  digitalWrite(LED, LOW);
  delayMicroseconds(3720);
}