// our working number
volatile uint8_t number = 1;

// baudrate for serial communication
const int baudRate = 9600;

// initialize Serial
void setup() {
  Serial.begin(baudRate);
}

// main loop
void loop() {
  // print number
  Serial.println(number);

  asm volatile(
    "cmp %[n], #128\n\t"
    "ite ne\n\t"
    "addne %[n], %[n]\n\t"
    "moveq %[n], #1\n\t"
    : [n] "=r" (number)
    : "0" (number)
    : "cc", "memory"
  );
}
