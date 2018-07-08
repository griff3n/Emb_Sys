#include <Energia.h>

#if defined(PART_TM4C129XNCZAD)
#include "inc/tm4c129xnczad.h"
#elif defined(PART_TM4C1294NCPDT)
#include "inc/tm4c1294ncpdt.h"
#elif defined(PART_TM4C1233H6PM) || defined(PART_LM4F120H5QR)
#include "inc/tm4c123gh6pm.h"
#else
#error "**** No PART defined or unsupported PART ****"
#endif

#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/eeprom.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#ifdef __cplusplus
extern "C" {

void _init(void) {
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
	if (ROM_EEPROMInit() == EEPROM_INIT_ERROR) {
		if (ROM_EEPROMInit() != EEPROM_INIT_ERROR)
			EEPROMMassErase();
	}

	timerInit();

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
#ifdef TARGET_IS_SNOWFLAKE_RA0
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOR);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOS);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOT);
#endif

	//Unlock and commit NMI pins PD7 and PF0
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0x4C4F434B;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x1;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x4C4F434B;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;

	// Deep Sleep mode init
	// DSLP clock = PIOSC / 16 = 1MHz
	// Note: Couldn't find the define constants for SysCtlDeepSleepPowerSet in the driverlib.
	//
#ifdef TARGET_IS_BLIZZARD_RB1
	ROM_SysCtlDeepSleepClockSet(SYSCTL_DSLP_DIV_16 | SYSCTL_DSLP_OSC_INT);
	SysCtlDeepSleepPowerSet(0x21); // FLASHPM = LOW_POWER_MODE, SRAMPM = STANDBY_MODE
	SysCtlLDODeepSleepSet(SYSCTL_LDO_1_00V); // Going lower tends to be very flaky and cause continual resets
	// particularly when measuring MCU current.
#endif
	//
#ifdef TARGET_IS_SNOWFLAKE_RA0
	ROM_SysCtlDeepSleepClockConfigSet(16, SYSCTL_DSLP_OSC_INT);
	SysCtlDeepSleepPowerSet(0x121); // TSPD, FLASHPM = LOW_POWER_MODE, SRAMPM = STANDBY_MODE
#endif
	//
} /* void _init(void) */

} /* extern "C" */
#endif

const uint8_t FotoPinIn = PD_1;
const uint8_t KondensatorPinIn = PD_0;
const uint8_t KondensatorPinOut = PC_4;
const int tmax = 1000;
const int tmin = 300;
const int hmax = 3300;
TaskHandle_t xTask1Handle = NULL;
boolean task1akive;

enum Status {
  K,
  H,
  T1nK,
  T1nH
};

Status zustand = H;

void loop() {
  int val = 0;
  switch(zustand) {
    case K:
    Serial.println("Zustand: K");
    if(task1akive) {
      val = analogRead(KondensatorPinIn);
      Serial.println(val, DEC);
      if(val < tmin) {
        zustand = H;
      } else {
        digitalWrite(KondensatorPinOut, LOW);
      }
    } else {
      val = analogRead(FotoPinIn);
      //Serial.println(val, DEC);
      if(val > hmax) {
        vTaskSuspend(xTask1Handle);
        zustand = T1nK;
      }
    }
    break;
    case H:
    Serial.println("Zustand: H");
    if(task1akive) {
      val = analogRead(KondensatorPinIn);
      Serial.println(val, DEC);
      if(val > tmax) {
        zustand = K;
      } else {
        digitalWrite(KondensatorPinOut, HIGH);
      }
    } else {
      val = analogRead(FotoPinIn);
      //Serial.println(val, DEC);
      if(val > hmax) {
        vTaskSuspend(xTask1Handle);
        zustand = T1nH;
      }
    }
    break;
    case T1nK:
    Serial.println("Zustand: T1nK");
    if(!task1akive) {
      val = analogRead(FotoPinIn);
      //Serial.println(val, DEC);
      if(val < hmax) {
        vTaskResume(xTask1Handle);
        zustand = K;
      }
    }
    break;
    case T1nH:
    Serial.println("Zustand: T1nH");
    if(!task1akive) {
      val = analogRead(FotoPinIn);
      //Serial.println(val, DEC);
      if(val < hmax) {
        vTaskResume(xTask1Handle);
        zustand = H;
      }
    }
    break;
  }
}

void setup() {
  // initialize the digital pin as an output.
  pinMode(FotoPinIn, INPUT);
  pinMode(KondensatorPinIn, INPUT);
  pinMode(KondensatorPinOut, OUTPUT);
  Serial.begin(9600);
}

void task1(void * pvParameters) {
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    Serial.println("Task 1");
    task1akive = true;
    loop();
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
  }
  vTaskDelete( NULL );
}

void task2(void * pvParameters) {
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    Serial.println("Task 2");
    task1akive = false;
    loop();
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2000));
  }
  vTaskDelete( NULL );
}

int main(void) {
  setup();
  xTaskCreate(task1, "TASK 1", configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 1UL, &xTask1Handle);
  xTaskCreate(task2, "TASK 2", configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 2UL, NULL);
  vTaskStartScheduler();
}
