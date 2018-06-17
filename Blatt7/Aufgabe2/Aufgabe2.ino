const uint8_t ButtonPinb = PC_4;
const uint8_t ButtonPind = PC_5;
const uint8_t PBuzzerPin = PC_6;
const uint8_t LedPin = RED_LED;
const uint32_t dit = 2000;

static bool ende;

class Node {
  public:
  Node(const char val, const char val2, Node * dot = nullptr, Node * line = nullptr) {
    this->val = val;
    this->val2 = val2;
    this->dot = dot;
    this->line = line;
  }

  ~Node() {
    delete dot;
    delete line;
  }

  char getVal() {
    return val;
  }

  char getVal2() {
    return val2;
  }

  Node * getDot() {
    return dot;
  }

  Node * getLine() {
    return line;
  }

  String findMorsecode(char letter, String code) {
    String morsecode;
    Serial.print("Suche in Node ");
    Serial.print(val);
    Serial.print(" nach ");
    Serial.println(letter);
    ende = false;
    if (letter == ' ') {
      return " ";
    }
    if (letter == val || letter == val2) {
      ende = true;
      return code;
    }
    if (dot != nullptr) {
      morsecode = dot->findMorsecode(letter, code + "d");
      if (ende) {
        return morsecode;
      }
    }
    if (line != nullptr) {
      morsecode = line->findMorsecode(letter, code + "l");
      if (ende) {
        return morsecode;
      }
    }
    Serial.println("Standardausgabe");
    return "";   
  }
  private:
  char val;
  char val2;
  Node * dot;
  Node * line;
};

class Uebersetzer {
  public:
    
  void init() {
    Node * imi = new Node('?' , ' '              );//DDLLDD
    Node * uk  = new Node('_' , ' '              );//DDLLDL
    Node * aaa = new Node('.' , ' '              );//DLDLDL
    Node * ac  = new Node('@' , ' '              );//DLLDLD
    Node * jn  = new Node('\'', ' '              );//DLLLLD
    Node * ba  = new Node('-' , ' '              );//LDDDDL
    Node * nnn = new Node(';' , ' '              );//LDLDLD
    Node * kk  = new Node(')' , ' '              );//LDLLDL
    Node * mim = new Node(',' , ' '              );//LLDDLL
    Node * os  = new Node(':' , ' '              );//LLLDDD
    Node * _5  = new Node('5' , ' '              );//DDDDD
    Node * _4  = new Node('4' , ' '              );//DDDDL
    Node * _3  = new Node('3' , ' '              );//DDDLL
    Node * nu  = new Node('\n', ' ', imi    , uk );//DDLLD
    Node * _2  = new Node('2' , ' '              );//DDLLL
    Node * ar  = new Node('+' , ' ', nullptr, aaa);//DLDLD
    Node * wa  = new Node('\n', ' ', ac          );//DLLDL
    Node * _1  = new Node('1' , ' ', jn          );//DLLLL
    Node * _6  = new Node('6' , ' ', nullptr, ba );//LDDDD
    Node * bt  = new Node('=' , ' '              );//LDDDL
    Node * dn  = new Node('/' , ' '              );//LDDLD
    Node * ka  = new Node('\n', ' ', nnn         );//LDLDL
    Node * kn  = new Node('(' , ' ', nullptr, kk );//LDLLD
    Node * _7  = new Node('7' , ' '              );//LLDDD
    Node * ga  = new Node('\n', ' ', nullptr, mim);//LLDDL
    Node * _8  = new Node('8' , ' ', os          );//LLLDD
    Node * _9  = new Node('9' , ' '              );//LLLLD
    Node * _0  = new Node('0' , ' '              );//LLLLL
    Node * h   = new Node('H' , 'h', _5      , _4);//DDDD
    Node * v   = new Node('V' , 'v', nullptr , _3);//DDDL
    Node * f   = new Node('F' , 'f'              );//DDLD
    Node * ue  = new Node('\n', ' ', nu      , _2);//DDLL
    Node * l   = new Node('L' , 'l'              );//DLDD
    Node * ae  = new Node('\n', ' ', ar          );//DLDL
    Node * p   = new Node('P' , 'p', nullptr , wa);//DLLD
    Node * j   = new Node('J' , 'j', nullptr , _1);//DLLL
    Node * b   = new Node('B' , 'b', _6      , bt);//LDDD
    Node * x   = new Node('X' , 'x', dn          );//LDDL
    Node * c   = new Node('C' , 'c', nullptr , ka);//LDLD
    Node * y   = new Node('Y' , 'y', kn          );//LDLL
    Node * z   = new Node('Z' , 'z', _7      , ga);//LLDD
    Node * q   = new Node('Q' , 'q'              );//LLDL
    Node * oe  = new Node('\n', ' ', _8          );//LLLD
    Node * ch  = new Node('\n', ' ', _9      , _0);//LLLL
    Node * s   = new Node('S' , 's', h       , v );//DDD
    Node * u   = new Node('U' , 'u', f       , ue);//DDL
    Node * r   = new Node('R' , 'r', l       , ae);//DLD
    Node * w   = new Node('W' , 'w', p       , j );//DLL
    Node * d   = new Node('D' , 'd', b       , x );//LDD
    Node * k   = new Node('K' , 'k', c       , y );//LDL
    Node * g   = new Node('G' , 'g', z       , q );//LLD
    Node * o   = new Node('O' , 'o', oe      , ch);//LLL
    Node * i   = new Node('I' , 'i', s       , u );//DD
    Node * a   = new Node('A' , 'a', r       , w );//DL
    Node * n   = new Node('N' , 'n', d       , k );//LD
    Node * m   = new Node('M' , 'm', g       , o );//LL
    Node * e   = new Node('E' , 'e', i       , a );//D
    Node * t   = new Node('T' , 't', n       , m );//L
    Node * morsetabelle  = new Node(' ' , ' ', e, t);
  }

  void uebersetzen(String text) {   
      Serial.println("uebersetze: "+text);
    for(char zeichen : text) {
      Serial.print("uebersetze Zeichen: ");
      Serial.println(zeichen);
      ausgabeMorseZeichen(morsetabelle->findMorsecode(zeichen, ""));
    }
  }

  virtual void ausgabeMorseZeichen(String code) =0;
  
  private:
  Node * morsetabelle;
};


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
class PBuzzer : public Uebersetzer {
  public:
  PBuzzer() {
    pinMode(PORT_NB, OUTPUT);
    init();
  }
  
  void ausgabeMorseZeichen(String code) {
    
    Serial.println("Ausgabe: "+code);
    if(code == "") {
      return;
    }
    for(char c : code) {
      if(c == ' ') {
        digitalWrite(PORT_NB, LOW);
        delay(dit * 4);
      }
      if(c == 'd') {
        digitalWrite(PORT_NB, HIGH);
        delay(dit);
      }
      if(c == 'l') {
        digitalWrite(PORT_NB, HIGH);
        delay(dit * 3);
      }
    }
    digitalWrite(PORT_NB, LOW);
    delay(dit * 3);
    Serial.println("Ausgabe beendet: "+code);
  }
};

template<const uint8_t PORT_NB>
class TLed : public Uebersetzer {
  public:
  TLed() {
    pinMode(PORT_NB, OUTPUT);
    init();
  }

  void ausgabeMorseZeichen(String code) {
    
      Serial.println("Ausgabe: "+code);
    if(code == "") {
      return;
    }
    for(char c : code) {
      if(c == ' ') {
        digitalWrite(PORT_NB, LOW);
        delay(dit * 4);
      }
      if(c == 'd') {
        digitalWrite(PORT_NB, HIGH);
        delay(dit);
      }
      if(c == 'l') {
        digitalWrite(PORT_NB, HIGH);
        delay(dit * 3);
      }
    }
    digitalWrite(PORT_NB, LOW);
    delay(dit * 3);
    
    Serial.println("Ausgabe beendet: "+code);
  }
};

TButton<ButtonPinb> buttonB;
TButton<ButtonPind> buttonD;
PBuzzer<PBuzzerPin> pBuzzer;
TLed<LedPin> led;
bool aktive = false;
String inputString = "";
String lastString="t";
//bool stringComplete = false;

void setup() {
  Serial.begin(9600);
  inputString.reserve(128);
}

void loop() {
  if(inputString==lastString){
    translate();
  }
  if(buttonB.state() == HIGH) {
    aktive = !aktive;
  } 
  if(buttonD.state() == HIGH) {
    translate();
  }
  inputString = "";
  //serialEvent();
}

void translate(){

  if(aktive) {
      Serial.println("Buzzer übersetzt");
      pBuzzer.uebersetzen(lastString);
    } else {
      Serial.println("Led übersetzt");
      led.uebersetzen(lastString);
    }
  }

void serialEvent() {
  int i = 0;
  while(Serial.available() && i < 128) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    i++;
  }
  
  Serial.println("Eingelesen: "+inputString);
  lastString=inputString;
}

