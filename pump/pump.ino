#define pumpPin 6

void setup() {
  // put your setup code here, to run once:
  pinMode(pumpPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // negative logic, 1 - on, 0 - off
  digitalWrite(pumpPin, 1);
  delay(1000);
  digitalWrite(pumpPin, 0);
  delay(3000);
}
