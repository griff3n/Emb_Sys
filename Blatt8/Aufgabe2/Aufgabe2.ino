#include "inc/tm4c123gh6pm.h"
#include "inc/hw_timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/hibernate.h"

#define Te 20
#define Tw 10
#define Tg 10
#define Tu 2

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
  ROTGELB_ROT,
  ENERGIESPARMODUS,
  SEQUENZ_ENDE
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

void ISR(void);

class Timer {
  public:
  static Timer& getTimer() {
    static Timer timer;
    return timer;
  }

  void setTimer(int zeitspanne) {
    this->zeitspanne = zeitspanne;
    TimerEnable(TIMER0_BASE, TIMER_A);
  }

  unsigned long getZeitspanne() {
    return zeitspanne;
  }

  void resetTimer() {
    count = 0;
  }

  unsigned long getCount() {
    return count;
  }

  void subCount() {
    count--;
  }

  void addCount() {
    count++;
  }
  
  private:
  Timer() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); 
    IntMasterEnable();
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    unsigned long p = 40000000;          //  40MHz
    TimerLoadSet(TIMER0_BASE, TIMER_A, p); 
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, ISR);
  }
  unsigned long count = 0;
  unsigned long zeitspanne;
  Timer(const Timer&);
  Timer & operator = (const Timer&);
};

void ISR(void) {
  bool buttonaktiv = button.state() == LOW;
  Timer& t = Timer::getTimer();
  TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  if(t.getCount() == t.getZeitspanne() || zustand == DEF || zustand == ENERGIESPARMODUS) {
    switch(zustand) {
    case DEF:
      Serial.println("DEF");
      fahrzeugampel.gruen.an();
      fussgaengerampel.rot.an();
      if(t.getCount() == Te - 1) {
        zustand = ENERGIESPARMODUS;
        t.resetTimer();
        t.setTimer(1);
      }
      if(buttonaktiv) {
        zustand = START_SEQUENZ1;
        t.resetTimer();
        t.setTimer(1);
      }
      break;
    case START_SEQUENZ1:
      Serial.println("START_SEQUENZ1");
      zustand = GRUEN_ROT;
      t.resetTimer();
      t.setTimer(Tw);
      break;
    case GRUEN_ROT:
      Serial.println("GRUEN_ROT");
      zustand = GELB_ROT;
      t.resetTimer();
      t.setTimer(Tu);
      break;
    case GELB_ROT:
      Serial.println("GELB_ROT");
      fahrzeugampel.gruen.aus();
      fahrzeugampel.gelb.an();
      zustand = ROT_ROT1;
      t.resetTimer();
      t.setTimer(Tu);
      break;
    case ROT_ROT1:
      Serial.println("ROT_ROT1");
      fahrzeugampel.gelb.aus();
      fahrzeugampel.rot.an();
      zustand = ROT_GRUEN;
      t.resetTimer();
      t.setTimer(Tu);
      break;
    case ROT_GRUEN:
      Serial.println("ROT_GRUEN");
      fussgaengerampel.rot.aus();
      fussgaengerampel.gruen.an();
      zustand = START_SEQUENZ2;
      t.resetTimer();
      t.setTimer(Tg);
      break;
    case START_SEQUENZ2:
      Serial.println("START_SEQUENZ2");
      zustand = ROT_ROT2;
      t.resetTimer();
      t.setTimer(Tu);
      break;
    case ROT_ROT2:
      Serial.println("ROT_ROT2");
      fussgaengerampel.gruen.aus();
      fussgaengerampel.rot.an();
      zustand = ROTGELB_ROT;
      t.resetTimer();
      t.setTimer(Tu);
      break;
    case ROTGELB_ROT:
      Serial.println("ROTGELB_ROT");
      fahrzeugampel.gelb.an();
      zustand = SEQUENZ_ENDE;
      t.resetTimer();
      t.setTimer(Tu);
      break;
    case SEQUENZ_ENDE:
      Serial.println("SEQUENZ_ENDE");
      fahrzeugampel.gelb.aus();
      fahrzeugampel.rot.aus();
      fahrzeugampel.gruen.an();
      zustand = DEF;
      t.resetTimer();
      t.subCount();
      break;
    case ENERGIESPARMODUS:
      Serial.println("ENERGIESPARMODUS");
      if(buttonaktiv) {
        zustand = DEF;
        t.resetTimer();
        break;
      }    
      fahrzeugampel.gruen.aus();
      fahrzeugampel.gelb.aus();
      fahrzeugampel.rot.aus();
      fussgaengerampel.gruen.aus();
      fussgaengerampel.rot.aus();
      SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
      HibernateEnableExpClk(SysCtlClockGet());
      HibernateGPIORetentionEnable();
      HibernateWakeSet(HIBERNATE_WAKE_PIN);
      HibernateRequest();
      break;
    }
  }
  if(!buttonaktiv) {
    t.addCount();
  }
}

const int baudRate = 9600;
void setup() {
  Serial.begin(baudRate);
  attachInterrupt(PUSH2, ISR, FALLING);
  Timer& t = Timer::getTimer();
  t.setTimer(1);
}

void loop() {}
