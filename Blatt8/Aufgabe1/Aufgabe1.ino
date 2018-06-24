#define Tw 5000
#define Tg 5000
#define Tu 1000

const uint8_t LedPinGruen1 = PC_4;
const uint8_t LedPinRot1 = PC_5;
const uint8_t LedPinGruen2 = PC_6;
const uint8_t LedPinGelb2 = PC_7;
const uint8_t LedPinRot2 = PD_6;
const uint8_t ButtonPin = PUSH2;

enum Status {
  DEF,
  START_SEQUENZ1,
  GRUEN_ROT,
  GELB_ROT,
  ROT_ROT1,
  ROT_GRUEN,
  START_SEQUENZ2,
  ROT_ROT2,
  ROTGELB_ROT
};

Status zustand = DEF;

template <const uint8_t PORT>
class Led {
  public:
  Led(const uint8_t f_ledState = LOW) : m_ledState(f_ledState) {
    pinMode(PORT, OUTPUT);
    digitalWrite(PORT, m_ledState);
  }
  void an() {
    m_ledState = HIGH;
    digitalWrite(PORT, m_ledState);
  }
  void aus() {
    m_ledState = LOW;
    digitalWrite(PORT, m_ledState);
  }
  private:
  uint8_t m_ledState;
};

template <const uint8_t PORT>
class Button {
  public:
    Button(const uint8_t f_btnState = LOW) : m_btnState(f_btnState) {
      pinMode(PORT, INPUT);
    }

    uint8_t state() {
      m_btnState = digitalRead(PORT);
      return m_btnState;
    }

  private:
    uint8_t m_btnState;
};

struct Fahrzeugampel {
  Led<LedPinRot2> rot;
  Led<LedPinGelb2> gelb;
  Led<LedPinGruen2> gruen;
};

struct Fussgaengerampel {
  Led<LedPinRot1> rot;
  Led<LedPinGruen1> gruen;
};

Fahrzeugampel fahrzeugampel;
Fussgaengerampel fussgaengerampel;
Button<ButtonPin> button;

const int baudRate = 9600;
void setup() {
  Serial.begin(baudRate);
}

void loop() {
  switch(zustand) {
    case DEF:
      Serial.println("DEF");
      fahrzeugampel.gelb.aus();
      fahrzeugampel.rot.aus();
      fahrzeugampel.gruen.an();
      fussgaengerampel.rot.an();
      if(button.state() == LOW) {
        zustand = START_SEQUENZ1;
      }
      break;
    case START_SEQUENZ1:
      Serial.println("START_SEQUENZ1");
      delay(Tw);
      zustand = GRUEN_ROT;
      break;
    case GRUEN_ROT:
      Serial.println("GRUEN_ROT");
      delay(Tu);
      zustand = GELB_ROT;
      break;
    case GELB_ROT:
      Serial.println("GELB_ROT");
      fahrzeugampel.gruen.aus();
      fahrzeugampel.gelb.an();
      delay(Tu);
      zustand = ROT_ROT1;
      break;
    case ROT_ROT1:
      Serial.println("ROT_ROT1");
      fahrzeugampel.gelb.aus();
      fahrzeugampel.rot.an();
      delay(Tu);
      zustand = ROT_GRUEN;
      break;
    case ROT_GRUEN:
      Serial.println("ROT_GRUEN");
      fussgaengerampel.rot.aus();
      fussgaengerampel.gruen.an();
      delay(Tg);
      zustand = START_SEQUENZ2;
      break;
    case START_SEQUENZ2:
      Serial.println("START_SEQUENZ2");
      delay(Tu);
      zustand = ROT_ROT2;
      break;
    case ROT_ROT2:
      Serial.println("ROT_ROT2");
      fussgaengerampel.gruen.aus();
      fussgaengerampel.rot.an();
      delay(Tu);
      zustand = ROTGELB_ROT;
      break;
    case ROTGELB_ROT:
      Serial.println("ROTGELB_ROT");
      fahrzeugampel.gelb.an();
      delay(Tu);
      zustand = DEF;
      break;
  }
}
