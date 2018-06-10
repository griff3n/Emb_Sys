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
    "sub sp, #12\n\t"
    "movs r3, #2\n\t"
    "str r3, [sp, #4]\n\t"
    "ldr r3, [sp, #4]\n\t"
    "cmp r3, #12\n\t"
    "bgt.n exit\n\t"
    "ldr r0, [%[fD], #0]\n\t"
    "back: ldr r4, [sp, #4]\n\t"
    "ldr r3, [sp, #4]\n\t"
    "ldr r1, [sp, #4]\n\t"
    "ldr r2, [sp, #4]\n\t"
    "add r3, r0\n\t"
    "add r2, #1\n\t"
    "add r1, r0\n\t"
    "str r2, [sp, #4]\n\t"
    "ldrb.w r3, [r3, #-1]\n\t"
    "ldrb.w r1, [r1, #-2]\n\t"
    "ldr r2, [sp, #4]\n\t"
    "add r3, r1\n\t"
    "cmp r2, #12\n\t"
    "strb r3, [r0, r4]\n\t"
    "ble.n back\n\t"
    "exit: movs r0, #0\n\t"
    "add sp, #12\n\t"
    :: [fD] "r" (fibData)
    :"r0" , "r1" , "r2" , "r3" , "r4" , "cc" , "memory"
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
