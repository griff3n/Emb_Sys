#define PIN PB_3
#define SPEED_S 500
#define SPEED_O 2000

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN, OUTPUT); 
}

void character(int speed) {
  digitalWrite(PIN, LOW);
  delay(speed);
  digitalWrite(PIN, HIGH);
  delay(speed);
}


void loop() {
  for (int x = 1; x <= 3; x++) {
    character(SPEED_S);
  }
  for (int x = 1; x <= 3; x++) {
    character(SPEED_O);
  }
  for (int x = 1; x <= 3; x++) {
    character(SPEED_S);
  }
  delay(2000);
}
