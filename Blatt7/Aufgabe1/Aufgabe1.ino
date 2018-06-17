const uint8_t ButtonPinb = PC_4;
const uint8_t ButtonPind = PC_5;
const uint8_t PBuzzerPin = PC_6;
int state = 0;

template <const uint8_t PORT_NB>
class TButton {
  public:
    TButton(const uint8_t f_btnState = LOW, const int stateAddition=0) : m_btnState(f_btnState) {
      pinMode(PORT_NB, INPUT);
      this->stateAddition=stateAddition;
    }

    void updateState() {
      m_btnState = digitalRead(PORT_NB);
      if (m_btnState != lastState) {
        if (m_btnState == HIGH) {
          state=state+stateAddition;
        } else {
          state=state-stateAddition;
        }
        lastState = m_btnState;
      }
    }

  private:
    uint8_t m_btnState;
    uint8_t lastState = LOW;
    int stateAddition=0;
};

template<const uint8_t PORT_NB>
class PBuzzer {
  public:
    PBuzzer(const uint8_t f_pBuzzerState = LOW) : m_pBuzzerState(f_pBuzzerState) {
      pinMode(PORT_NB, OUTPUT);
      digitalWrite(PORT_NB, m_pBuzzerState);
    }

    void setPBuzzerState(const uint8_t f_pBuzzerState) {
      if (!(f_pBuzzerState == LOW || f_pBuzzerState == HIGH)) {
        return;
      }
      m_pBuzzerState = f_pBuzzerState;
      digitalWrite(PORT_NB, m_pBuzzerState);
    }

  private:
    uint8_t m_pBuzzerState;
};

TButton<ButtonPinb> buttonB = TButton<ButtonPinb>(LOW,1);
TButton<ButtonPind> buttonD = TButton<ButtonPind>(LOW,2);
PBuzzer<PBuzzerPin> pBuzzer;


const int baudRate = 9600;
void setup() {
  Serial.begin(baudRate);
}
void loop() {
  //  if (buttonB.state() == HIGH && buttonD.state() == HIGH) {
  //    state = 3;
  //    Serial.println(state);
  //  } else if (buttonB.state() == HIGH) {
  //    state = 1;
  //    Serial.println(state);
  //  } else if (buttonD.state() == HIGH) {
  //    state = 2;
  //    Serial.println(state);
  //  } else {
  //    state = 0;
  //    Serial.println(state);
  //  }
  buttonB.updateState();
  buttonD.updateState();
  
  Serial.println(state);
  if (state == 3) {
    pBuzzer.setPBuzzerState(HIGH);
  } else {
    pBuzzer.setPBuzzerState(LOW);
  }
}
