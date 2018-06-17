const uint8_t ButtonPinb = PC_4;
const uint8_t ButtonPind = PC_5;
const uint8_t PBuzzerPin = PC_6;
int state = 0;
//loop variable and delay for debounce method
#define debounceLoops 50
#define debounceDelay 2

uint8_t debounce(uint8_t PIN) {
  int i;
  for(i = 0; digitalRead(PIN) == HIGH && i < debounceLoops; i++) {
    delay(debounceDelay);
  }
  if (i == debounceLoops) {
    return HIGH;
  } else {
    return LOW;
  }
}

template <const uint8_t PORT_NB>
class TButton {
  public:
    TButton(const uint8_t f_btnState = LOW, const int stateAddition=0) : m_btnState(f_btnState) {
      pinMode(PORT_NB, INPUT);
      this->stateAddition=stateAddition;
    }

    void updateState() {
      if (debounce(PORT_NB) == HIGH) {
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

void updateBtnB() {
  buttonB.updateState();
}

void updateBtnD() {
  buttonD.updateState();
}

const int baudRate = 9600;
void setup() {
  Serial.begin(baudRate);
  //set interrupts
  attachInterrupt(ButtonPinb, updateBtnB, RISING);
  attachInterrupt(ButtonPind, updateBtnD, RISING);
}
void loop() {
  Serial.println(state);
  if (state == 3) {
    pBuzzer.setPBuzzerState(HIGH);
  } else {
    pBuzzer.setPBuzzerState(LOW);
  }
}
