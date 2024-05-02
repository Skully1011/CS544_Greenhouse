#define PUMP 4

void setup() {
  // put your setup code here, to run once:
  pinMode(PUMP, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PUMP, LOW);
  delay(1000);
  digitalWrite(PUMP, HIGH);
  while (true){}
}
