#define LED_PIN 2

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Gradually Increase Duty Cycle
  for(int i=0; i<255; i++)
  {
    analogWrite(LED_PIN, i);
    delay(5);
  }
  // Gradually Decrease Duty Cycle
  for(int i=255; i>0; i--)
  {
    analogWrite(LED_PIN, i);
    delay(5);
  }
}
