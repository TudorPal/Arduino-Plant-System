#define soilPin A0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(soilPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Humidity: ");
  int reading = analogRead(soilPin);
  Serial.println(reading);
}
