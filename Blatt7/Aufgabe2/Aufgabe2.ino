
//baudrate for serial communication
#define baudRate 9600
//loop variable and delay for debounce method
#define debounceLoops 50
#define debounceDelay 2

const uint8_t LedPortOut = RED_LED; //define port for led output
const uint8_t BuzzerPortOut = PC_6; // define port for buzzer output
const uint8_t ButtonPinPC4 = PC_4; //define pin for button input on PC4
const uint8_t ButtonPinPC5 = PC_5; //define pin for button input on PC5
String inputString = ""; //complete input string
String inputBuffer = ""; //input string from serial communication
unsigned int reservedBytes = 128; //reserved bytes for the input string
boolean stringComplete = false; //whether the string is complete

//long, short delay, delay between signals and delay between words
const unsigned int DIT = 200;
const unsigned int DAH = 3 * DIT;
const unsigned int LETTEREND = 2 * DIT; // Plus 1 DIT Signal-Delay
const unsigned int WORDEND = 4 * DIT; // Plus 1 DAH Didget-Delay

unsigned int buzzerFrequency = 100; //the frequency of the tone of the buzzer in hertz
boolean buzzer = false; //which output is used

//Output class
template<const uint8_t outputPin>
class MorseOutput {
public:
  virtual void longSignal() = 0;
  virtual void shortSignal() = 0;
  virtual ~MorseOutput() {
  }
  ;
};

//Led output class
template<const uint8_t outputPin>
class LedOutput: public MorseOutput<outputPin> {
public:
  void longSignal() {
    digitalWrite(outputPin, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(DAH);
    digitalWrite(outputPin, LOW); // turn the LED off by making the voltage LOW
    delay(DIT);
  }
  void shortSignal() {
    digitalWrite(outputPin, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(DIT);
    digitalWrite(outputPin, LOW); // turn the LED off by making the voltage LOW
    delay(DIT);
  }
};

//Buzzer output class
template<const uint8_t outputPin>
class BuzzerOutput: public MorseOutput<outputPin> {
public:
  void longSignal() {
    tone(outputPin, buzzerFrequency); //turns on the buzzer with a specified frequency
    delay(DAH);
    noTone(outputPin); //turns off the buzzer
    delay(DIT);
  }
  void shortSignal() {
    tone(outputPin, buzzerFrequency); //turns on the buzzer with a specified frequency
    delay(DIT);
    noTone(outputPin); //turns off the buzzer
    delay(DIT);
  }
};

//global instance for buzzer on PC6
BuzzerOutput<BuzzerPortOut> buzzerOutput;
//global instance for led
LedOutput<LedPortOut> ledOutput;

uint8_t debounce(uint8_t PIN) {
  uint8_t m_buttonState;
  int i = 0;
  while (digitalRead(PIN) == HIGH && i < debounceLoops) {
    i++;
    delay(debounceDelay);
  }
  if (i == debounceLoops) {
    m_buttonState = HIGH;
    return m_buttonState;
  } else {
    m_buttonState = LOW;
    return m_buttonState;
  }
}

//short output signal with delay afterwards
void shortSignal() {
  if (buzzer) {
    buzzerOutput.shortSignal();
  } else {
    ledOutput.shortSignal();
  }
}
//long output signal with delay afterwards
void longSignal() {
  if (buzzer) {
    buzzerOutput.longSignal();
  } else {
    ledOutput.longSignal();
  }
}

//letter A in morse code
void letterA() {
  shortSignal();
  longSignal();
}
//letter B in morse code
void letterB() {
  longSignal();
  shortSignal();
  shortSignal();
  shortSignal();
}
//letter C in morse code
void letterC() {
  longSignal();
  shortSignal();
  longSignal();
  shortSignal();
}
//letter D in morse code
void letterD() {
  longSignal();
  shortSignal();
  shortSignal();
}
//letter E in morse code
void letterE() {
  shortSignal();
}
//letter F in morse code
void letterF() {
  shortSignal();
  shortSignal();
  longSignal();
  shortSignal();
}
//letter G in morse code
void letterG() {
  longSignal();
  longSignal();
  shortSignal();
}
//letter H in morse code
void letterH() {
  shortSignal();
  shortSignal();
  shortSignal();
  shortSignal();
}
//letter I in morse code
void letterI() {
  shortSignal();
  shortSignal();
}
//letter J in morse code
void letterJ() {
  shortSignal();
  longSignal();
  longSignal();
  longSignal();
}
//letter K in morse code
void letterK() {
  longSignal();
  shortSignal();
  longSignal();
}
//letter L in morse code
void letterL() {
  shortSignal();
  longSignal();
  shortSignal();
  shortSignal();
}
//letter M in morse code
void letterM() {
  longSignal();
  longSignal();
}
//letter N in morse code
void letterN() {
  longSignal();
  shortSignal();
}
//letter O in morse code
void letterO() {
  longSignal();
  longSignal();
  longSignal();
}
//letter P in morse code
void letterP() {
  shortSignal();
  longSignal();
  longSignal();
  shortSignal();
}
//letter Q in morse code
void letterQ() {
  longSignal();
  longSignal();
  shortSignal();
  longSignal();
}
//letter R in morse code
void letterR() {
  shortSignal();
  longSignal();
  shortSignal();
}
//letter S in morse code
void letterS() {
  shortSignal();
  shortSignal();
  shortSignal();
}
//letter T in morse code
void letterT() {
  longSignal();
}
//letter U in morse code
void letterU() {
  shortSignal();
  shortSignal();
  longSignal();
}
//letter V in morse code
void letterV() {
  shortSignal();
  shortSignal();
  shortSignal();
  longSignal();
}
//letter W in morse code
void letterW() {
  shortSignal();
  longSignal();
  longSignal();
}
//letter X in morse code
void letterX() {
  longSignal();
  shortSignal();
  shortSignal();
  longSignal();
}
//letter Y in morse code
void letterY() {
  longSignal();
  shortSignal();
  longSignal();
  longSignal();
}
//letter Z in morse code
void letterZ() {
  longSignal();
  longSignal();
  shortSignal();
  shortSignal();
}
//number 1 in morse code
void number1() {
  shortSignal();
  longSignal();
  longSignal();
  longSignal();
  longSignal();
}
//number 2 in morse code
void number2() {
  shortSignal();
  shortSignal();
  longSignal();
  longSignal();
  longSignal();

}
//number 3 in morse code
void number3() {
  shortSignal();
  shortSignal();
  shortSignal();
  longSignal();
  longSignal();
}
//number 4 in morse code
void number4() {
  shortSignal();
  shortSignal();
  shortSignal();
  shortSignal();
  longSignal();
}
//number 5 in morse code
void number5() {
  shortSignal();
  shortSignal();
  shortSignal();
  shortSignal();
  shortSignal();
}
//number 6 in morse code
void number6() {
  longSignal();
  shortSignal();
  shortSignal();
  shortSignal();
  shortSignal();
}
//number 7 in morse code
void number7() {
  longSignal();
  longSignal();
  shortSignal();
  shortSignal();
  shortSignal();
}
//number 8 in morse code
void number8() {
  longSignal();
  longSignal();
  longSignal();
  shortSignal();
  shortSignal();
}
//number 9 in morse code
void number9() {
  longSignal();
  longSignal();
  longSignal();
  longSignal();
  shortSignal();
}
//number 0 in morse code
void number0() {
  longSignal();
  longSignal();
  longSignal();
  longSignal();
  longSignal();
}
//special character DOT in morse code
void characterDOT() {
  letterA();
  letterA();
  letterA();
}
//special character COMMA in morse code
void characterCOMMA() {
  letterM();
  letterI();
  letterM();
}
//special character COLON in morse code
void characterCOLON() {
  letterO();
  letterS();
}
//special character SEMICOLON in morse code
void characterSEMICOLON() {
  letterN();
  letterN();
  letterN();
}
//special character QUESTIONMARK in morse code
void characterQUESTIONMARK() {
  letterI();
  letterM();
  letterI();
}
//special character HYPHEN in morse code
void characterHYPHEN() {
  letterB();
  letterA();
}
//special character UNDERSCORE in morse code
void characterUNDERSCORE() {
  letterU();
  letterK();
}
//special character OPENINGBRACKETS in morse code
void characterOPENINGBRACKETS() {
  letterK();
  letterN();
}
//special character CLOSINGBRACKETS in morse code
void characterCLOSINGBRACKETS() {
  letterK();
  letterK();
}
//special character INVERTEDCOMMA in morse code
void characterINVERTEDCOMMA() {
  letterJ();
  letterN();
}
//special character EQUALS in morse code
void characterEQUALS() {
  letterB();
  letterT();
}
//special character PLUS in morse code
void characterPLUS() {
  letterA();
  letterR();
}
//special character SLASH in morse code
void characterSLASH() {
  letterD();
  letterN();
}
//special character AT in morse code
void characterAT() {
  letterA();
  letterC();
}
//parses every character of the input string
void parseString(String input) {
  int length = input.length();
  for (int i = 0; i < length; i++) {
    switch (input[i]) {
    case 'a':
    case 'A':
      letterA();
      delay(LETTEREND);
      break;
    case 'b':
    case 'B':
      letterB();
      delay(LETTEREND);
      break;
    case 'c':
    case 'C':
      letterC();
      delay(LETTEREND);
      break;
    case 'd':
    case 'D':
      letterD();
      delay(LETTEREND);
      break;
    case 'e':
    case 'E':
      letterE();
      delay(LETTEREND);
      break;
    case 'f':
    case 'F':
      letterF();
      delay(LETTEREND);
      break;
    case 'g':
    case 'G':
      letterG();
      delay(LETTEREND);
      break;
    case 'h':
    case 'H':
      letterH();
      delay(LETTEREND);
      break;
    case 'i':
    case 'I':
      letterI();
      delay(LETTEREND);
      break;
    case 'j':
    case 'J':
      letterJ();
      delay(LETTEREND);
      break;
    case 'k':
    case 'K':
      letterK();
      delay(LETTEREND);
      break;
    case 'l':
    case 'L':
      letterL();
      delay(LETTEREND);
      break;
    case 'm':
    case 'M':
      letterM();
      delay(LETTEREND);
      break;
    case 'n':
    case 'N':
      letterN();
      delay(LETTEREND);
      break;
    case 'o':
    case 'O':
      letterO();
      delay(LETTEREND);
      break;
    case 'p':
    case 'P':
      letterP();
      delay(LETTEREND);
      break;
    case 'q':
    case 'Q':
      letterQ();
      delay(LETTEREND);
      break;
    case 'r':
    case 'R':
      letterR();
      delay(LETTEREND);
      break;
    case 's':
    case 'S':
      letterS();
      delay(LETTEREND);
      break;
    case 't':
    case 'T':
      letterT();
      delay(LETTEREND);
      break;
    case 'u':
    case 'U':
      letterU();
      delay(LETTEREND);
      break;
    case 'v':
    case 'V':
      letterV();
      delay(LETTEREND);
      break;
    case 'w':
    case 'W':
      letterW();
      delay(LETTEREND);
      break;
    case 'x':
    case 'X':
      letterX();
      delay(LETTEREND);
      break;
    case 'y':
    case 'Y':
      letterY();
      delay(LETTEREND);
      break;
    case 'z':
    case 'Z':
      letterZ();
      delay(LETTEREND);
      break;
    case '1':
      number1();
      delay(LETTEREND);
      break;
    case '2':
      number2();
      delay(LETTEREND);
      break;
    case '3':
      number3();
      delay(LETTEREND);
      break;
    case '4':
      number4();
      delay(LETTEREND);
      break;
    case '5':
      number5();
      delay(LETTEREND);
      break;
    case '6':
      number6();
      delay(LETTEREND);
      break;
    case '7':
      number7();
      delay(LETTEREND);
      break;
    case '8':
      number8();
      delay(LETTEREND);
      break;
    case '9':
      number9();
      delay(LETTEREND);
      break;
    case '0':
      number0();
      delay(LETTEREND);
      break;
    case '.':
      characterDOT();
      delay(LETTEREND);
      break;
    case ',':
      characterCOMMA();
      delay(LETTEREND);
      break;
    case ':':
      characterCOLON();
      delay(LETTEREND);
      break;
    case ';':
      characterSEMICOLON();
      delay(LETTEREND);
      break;
    case '?':
      characterQUESTIONMARK();
      delay(LETTEREND);
      break;
    case '-':
      characterHYPHEN();
      delay(LETTEREND);
      break;
    case '_':
      characterUNDERSCORE();
      delay(LETTEREND);
      break;
    case '(':
      characterOPENINGBRACKETS();
      delay(LETTEREND);
      break;
    case ')':
      characterCLOSINGBRACKETS();
      delay(LETTEREND);
      break;
    case '\'':
      characterINVERTEDCOMMA();
      delay(LETTEREND);
      break;
    case '=':
      characterEQUALS();
      delay(LETTEREND);
      break;
    case '+':
      characterPLUS();
      delay(LETTEREND);
      break;
    case '/':
      characterSLASH();
      delay(LETTEREND);
      break;
    case '@':
      characterAT();
      delay(LETTEREND);
      break;
    case ' ':
      delay(WORDEND);
      break;
    default:
      break;
    }
  }
}
//starts the output of morsecode, is called by button interrupt
void start() {
  if (debounce(ButtonPinPC5) == HIGH) {
    if (stringComplete) {
      parseString(inputString);
    }
  }
}
//switches the output device, is called by button interrupt
void switchOutput() {
  if (debounce(ButtonPinPC4) == HIGH) {
    buzzer = !buzzer;
  }
}

void setup() {
  Serial.begin(baudRate);
  inputString.reserve(reservedBytes);

  //init outputs
  pinMode(LedPortOut, OUTPUT);
  pinMode(BuzzerPortOut, OUTPUT);

  //init inputs
  pinMode(ButtonPinPC4, INPUT_PULLDOWN);
  pinMode(ButtonPinPC5, INPUT_PULLDOWN);

  //set interrupts
  attachInterrupt(ButtonPinPC4, switchOutput, RISING);
  attachInterrupt(ButtonPinPC5, start, RISING);
}
//
void serialEvent() {
  while (Serial.available()) { // get the new byte:
    char inChar = (char) Serial.read(); // add it to the inputString
    inputBuffer += inChar;
    if (inChar == '\n') { // if the incoming character is a newline, set a flag
      stringComplete = true; // if the flag is set, the string can be parsed
      if(inputBuffer.length() > reservedBytes){
        inputString = "";
        for(int i = 0; i < reservedBytes; i++){
          inputString += inputBuffer[i];
        }
      }else inputString = inputBuffer;
      inputBuffer = "";
    }
  }
}

void loop() {

}

