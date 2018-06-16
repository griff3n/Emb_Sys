const uint8_t ButtonPinb = PC_4;
const uint8_t ButtonPind = PC_5;
const uint8_t PBuzzerPin = PC_6;

template <const uint8_t PORT_NB>
class TButton {
  public:
  TButton(const uint8_t f_btnState = LOW) : m_btnState(f_btnState) {
    pinMode(PORT_NB, INPUT);
  }

  uint8_t state() {
    m_btnState = digitalRead(PORT_NB);
    return m_btnState;
  }

  private:
  uint8_t m_btnState;
};

template<const uint8_t PORT_NB>
class PBuzzer {
  public:
  PBuzzer(const uint8_t f_pBuzzerState = LOW) : m_pBuzzerState(f_pBuzzerState) {
    pinMode(PORT_NB, OUTPUT);
    digitalWrite(PORT_NB, m_pBuzzerState);
  }

  void setPBuzzerState(const uint8_t f_pBuzzerState) {
    if(f_pBuzzerState != LOW || f_pBuzzerState != HIGH) {
      return;
    }
    m_pBuzzerState = f_pBuzzerState;
    digitalWrite(PORT_NB, m_pBuzzerState);
  }

  private:
  uint8_t m_pBuzzerState;
};

TButton<ButtonPinb> buttonB;
TButton<ButtonPind> buttonD;
PBuzzer<PBuzzerPin> pBuzzer;

void setup() {}

void loop() {
  if(buttonB.state() == HIGH && buttonD.state() == HIGH) {
    pBuzzer.setPBuzzerState(HIGH);
  } else {
    pBuzzer.setPBuzzerState(LOW);
  }
}
