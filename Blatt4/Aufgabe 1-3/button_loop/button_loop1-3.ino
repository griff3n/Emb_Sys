// Demo illustrating blinking led and reading of button.
// Led is finally turned off when button is pressed. 

// define port for led output
const uint8_t LedPortOut = RED_LED;
// define pin for button input
const uint8_t ButtonPinIn = PC_4;
// define delay for blinking in ms
const uint32_t Delay = 2000;

//! LED handling class. Has disable() function for emergency stop.
//! Parameter (in): PORT_NB (output port for connected led)
template <const uint8_t PORT_NB>
class TLed {
  public:
  //! Constructor takes state (HIGH, LOW) only if given. 
  //! Defaults: value for state = LOW, and is not disabled. 
  TLed(const uint8_t f_ledState = LOW) : m_ledState(f_ledState), m_disabled(false) {
    // led is always output
    pinMode(PORT_NB, OUTPUT);
    // set led to default state 
    digitalWrite(PORT_NB, m_ledState);
  }

  //! If this led is disable, nothing happens, otherwise
  //! toggles state of led (from HIGH to LOW or from LOW to HIGH).
  void toggle() {
    if (m_disabled) {
      // somehow no longer active
      return; 
    }
    // toggle state
    if (m_ledState == LOW) {
      m_ledState = HIGH;
    } else {
      m_ledState = LOW;
    }
    // set led to current state
    digitalWrite(PORT_NB, m_ledState);
  }

  //! Turn led finally off (emergency stop), state is set LOW, functionality off.
  void off() {
    m_disabled = true; 
    m_ledState = LOW;
    // set led to current state      
    digitalWrite(PORT_NB, m_ledState);
  }
  private:
  // current state of led
  uint8_t m_ledState;
  // disable flag (on if led is finally turned off)
  bool m_disabled;
};

//....TODO: INSERT CODE FOR CLASS TBUTTON....
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


// global instances for led output
TLed<LedPortOut> Led;
// and for button pin 
TButton<ButtonPinIn> Button;
unsigned long time1, time2, time;

void setup() {
  Serial.begin(9600);
}

void loop() {  
  if(Button.state() == HIGH) {
    Serial.println("Start");
    time1 = micros();
    Led.off();
    time2 = micros();
    time = time2 - time1;
    Serial.println(time);
  } else {
    Led.toggle(); 
  }
  delay(Delay);   
}

