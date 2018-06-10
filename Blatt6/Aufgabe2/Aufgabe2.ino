// our working data
const uint8_t lastFiboIndex = 13;
uint8_t fibData[lastFiboIndex];
const int waitTime = 1000;

// baudrate for serial communication
const int baudRate = 9600;

void setup() {
  // initialize Serial
  Serial.begin(baudRate);

  // init first two Fibonacci numbers
  fibData[0] = 1;
  fibData[1] = 2;
  
  asm volatile(
    "ldr r4, [%fD]"
    "add.w r5, r4, %[lFi]"
    "back mov r6, r4"
    "adds r4, #1"
    "ldrb r7, [r6, #1]"
    "ldrb r6, [r6, #0]"
    "cmp r4, r5"
    "add r7, r6"
    "strb r7, [r4, #1]"
    "bne.n back"
    :: [fD] "r" (fibData[2]), [lFi] "r" (lastFiboIndex -2)
    : "r4" , "r5" , "r6" , "r7" , "cc" , "memory"
  );
}

// main loop
void loop() {
  // print data
  for (int i = 0; i < lastFiboIndex; i++) 
  {
    Serial.println(fibData[i]);
  }
  // delay 1s
  delay(waitTime);
}
