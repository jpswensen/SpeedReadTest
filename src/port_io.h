#ifndef __PORT_IO_H__
#define __PORT_IO_H__

#include "common.h"

#define TRIG_MODE INPUT_PULLUP
#define GATE_MODE INPUT_PULLDOWN

// volatile uint32_t shotStopTime = 2400000000; // This is 4 seconds @ 600 MHz
volatile long unsigned shotStopTime = 4UL*816000000UL;   // This is 4 seconds @ 816 MHz

#define TRIGGER_PIN 0
#define READ_TIME_OFFSET 10
#define SAMPLE_BUFF_SIZE 25000
#define MAX_TRIGGERS_PER_SHOT SAMPLE_BUFF_SIZE

#define IMXRT_GPIO6_DIRECT  (*(volatile uint32_t *)0x42000000)
#define IMXRT_GPIO7_DIRECT  (*(volatile uint32_t *)0x42004000)
#define IMXRT_GPIO8_DIRECT  (*(volatile uint32_t *)0x42008000)
#define IMXRT_GPIO9_DIRECT  (*(volatile uint32_t *)0x4200C000)


volatile uint32_t gpioIdx = 0;
volatile uint32_t triggerIndex = 0;
volatile DMAMEM uint32_t gpioTimes[SAMPLE_BUFF_SIZE];
volatile DMAMEM uint32_t gpio6Vals[SAMPLE_BUFF_SIZE];
volatile DMAMEM uint32_t gpio7Vals[SAMPLE_BUFF_SIZE];
volatile DMAMEM uint32_t gpio8Vals[SAMPLE_BUFF_SIZE];
volatile DMAMEM uint32_t gpio9Vals[SAMPLE_BUFF_SIZE];

void setup_port_io(void);
uint64_t unscrambleBits(volatile uint32_t port6, volatile uint32_t port7, volatile uint32_t port8, volatile uint32_t port9);
bool beamCheck(void);
uint64_t readBeams(void);
void captureTimeChangeNew(void);

void setup_port_io(void)
{
  // Set up the trigger pin as an input
  pinMode(TRIGGER_PIN, TRIG_MODE);

  // Set up all the beam pins as an input
  pinMode(1, GATE_MODE);
  pinMode(2, GATE_MODE);
  pinMode(3, GATE_MODE);
  pinMode(4, GATE_MODE);
  pinMode(5, GATE_MODE);
  pinMode(6, GATE_MODE);
  pinMode(7, GATE_MODE);
  pinMode(8, GATE_MODE);
  pinMode(9, GATE_MODE);
  pinMode(10, GATE_MODE);
  pinMode(11, GATE_MODE);
  pinMode(12, GATE_MODE);

  pinMode(14, GATE_MODE);
  pinMode(15, GATE_MODE);
  pinMode(16, GATE_MODE);
  pinMode(17, GATE_MODE);

  pinMode(20, GATE_MODE);
  pinMode(21, GATE_MODE);
  pinMode(22, GATE_MODE);
  pinMode(23, GATE_MODE);
  pinMode(24, GATE_MODE);
  pinMode(25, GATE_MODE);
  pinMode(26, GATE_MODE);
  pinMode(27, GATE_MODE);
  pinMode(28, GATE_MODE);
  pinMode(33, GATE_MODE);
  pinMode(34, GATE_MODE);
  pinMode(35, GATE_MODE);
  pinMode(36, GATE_MODE);
  pinMode(37, GATE_MODE);
  pinMode(38, GATE_MODE);
  pinMode(39, GATE_MODE);
  pinMode(40, GATE_MODE);
  pinMode(41, GATE_MODE);
}

#define PORT6_MASK (CORE_PIN27_BITMASK|CORE_PIN26_BITMASK|CORE_PIN25_BITMASK|CORE_PIN24_BITMASK|CORE_PIN38_BITMASK|CORE_PIN39_BITMASK| \
        CORE_PIN40_BITMASK|CORE_PIN41_BITMASK|CORE_PIN14_BITMASK|CORE_PIN15_BITMASK|CORE_PIN16_BITMASK|CORE_PIN17_BITMASK| \
        CORE_PIN20_BITMASK|CORE_PIN21_BITMASK|CORE_PIN22_BITMASK|CORE_PIN23_BITMASK|CORE_PIN1_BITMASK)
#define PORT7_MASK (CORE_PIN34_BITMASK|CORE_PIN35_BITMASK|CORE_PIN36_BITMASK|CORE_PIN37_BITMASK|CORE_PIN12_BITMASK|CORE_PIN11_BITMASK| \
        CORE_PIN10_BITMASK|CORE_PIN9_BITMASK|CORE_PIN8_BITMASK|CORE_PIN7_BITMASK|CORE_PIN6_BITMASK)
#define PORT8_MASK (CORE_PIN28_BITMASK)
#define PORT9_MASK (CORE_PIN33_BITMASK|CORE_PIN5_BITMASK|CORE_PIN4_BITMASK|CORE_PIN3_BITMASK|CORE_PIN2_BITMASK)

uint64_t unscrambleBits(volatile uint32_t port6, volatile uint32_t port7, volatile uint32_t port8, volatile uint32_t port9)
{
  return  ((uint64_t)((port9 & CORE_PIN33_BITMASK) >> (CORE_PIN33_BIT))) << 33 |
          ((uint64_t)((port8 & CORE_PIN28_BITMASK) >> (CORE_PIN28_BIT))) << 32 |
          (uint64_t)(
            ((port7 & CORE_PIN34_BITMASK) >> (CORE_PIN34_BIT)) << 31 |
            ((port6 & CORE_PIN27_BITMASK) >> (CORE_PIN27_BIT)) << 30 |
            ((port7 & CORE_PIN35_BITMASK) >> (CORE_PIN35_BIT)) << 29 |
            ((port6 & CORE_PIN26_BITMASK) >> (CORE_PIN26_BIT)) << 28 |
            ((port7 & CORE_PIN36_BITMASK) >> (CORE_PIN36_BIT)) << 27 |
            ((port6 & CORE_PIN25_BITMASK) >> (CORE_PIN25_BIT)) << 26 |
            ((port7 & CORE_PIN37_BITMASK) >> (CORE_PIN37_BIT)) << 25 |
            ((port6 & CORE_PIN24_BITMASK) >> (CORE_PIN24_BIT)) << 24 |
            ((port6 & CORE_PIN38_BITMASK) >> (CORE_PIN38_BIT)) << 23 |
            ((port7 & CORE_PIN12_BITMASK) >> (CORE_PIN12_BIT)) << 22 |
            ((port6 & CORE_PIN39_BITMASK) >> (CORE_PIN39_BIT)) << 21 |
            ((port7 & CORE_PIN11_BITMASK) >> (CORE_PIN11_BIT)) << 20 |
            ((port6 & CORE_PIN40_BITMASK) >> (CORE_PIN40_BIT)) << 19 |
            ((port7 & CORE_PIN10_BITMASK) >> (CORE_PIN10_BIT)) << 18 |
            ((port6 & CORE_PIN41_BITMASK) >> (CORE_PIN41_BIT)) << 17 |
            ((port7 & CORE_PIN9_BITMASK) >> (CORE_PIN9_BIT)) << 16 |
            ((port6 & CORE_PIN14_BITMASK) >> (CORE_PIN14_BIT)) << 15 |
            ((port7 & CORE_PIN8_BITMASK) >> (CORE_PIN8_BIT)) << 14 |
            ((port6 & CORE_PIN15_BITMASK) >> (CORE_PIN15_BIT)) << 13 |
            ((port7 & CORE_PIN7_BITMASK) >> (CORE_PIN7_BIT)) << 12 |
            ((port6 & CORE_PIN16_BITMASK) >> (CORE_PIN16_BIT)) << 11 |
            ((port7 & CORE_PIN6_BITMASK) >> (CORE_PIN6_BIT)) << 10 |
            ((port6 & CORE_PIN17_BITMASK) >> (CORE_PIN17_BIT)) << 9 |
            ((port9 & CORE_PIN5_BITMASK) >> (CORE_PIN5_BIT)) << 8 |
            ((port6 & CORE_PIN20_BITMASK) >> (CORE_PIN20_BIT)) << 7 |
            ((port9 & CORE_PIN4_BITMASK) >> (CORE_PIN4_BIT)) << 6 |
            ((port6 & CORE_PIN21_BITMASK) >> (CORE_PIN21_BIT)) << 5 |
            ((port9 & CORE_PIN3_BITMASK) >> (CORE_PIN3_BIT)) << 4 |
            ((port6 & CORE_PIN22_BITMASK) >> (CORE_PIN22_BIT)) << 3 |
            ((port9 & CORE_PIN2_BITMASK) >> (CORE_PIN2_BIT)) << 2 |
            ((port6 & CORE_PIN23_BITMASK) >> (CORE_PIN23_BIT)) << 1 |
            ((port6 & CORE_PIN1_BITMASK) >> CORE_PIN1_BIT));
}

void print_binary_rec(uint64_t number)
{
  char buffer[35];
  memset(buffer, 0, 35);
  for (int i = 33 ; i >= 0 ; i--)
  {
    buffer[33-i] = (number & 1) ? '1' : '0';
    number = number >> 1;
  }
  buffer[34]='\0';
  DEBUG_PRINT("%s", buffer);    
}

void print_binary(uint64_t number)
{
  DEBUG_PRINT("0xb");
  print_binary_rec(number);
}



uint64_t readBeams(void)
{
  register uint32_t data6  = (IMXRT_GPIO6_DIRECT&PORT6_MASK);  // 0B11111111111111110011000000000000
  register uint32_t data7  = (IMXRT_GPIO7_DIRECT&PORT7_MASK);
  register uint32_t data8  = (IMXRT_GPIO8_DIRECT&PORT8_MASK);
  register uint32_t data9  = (IMXRT_GPIO9_DIRECT&PORT9_MASK);
  //DEBUG_PRINT("D6: 0x%08x  D7: 0x%08x  D8: 0x%08x  D9: 0x%08x\n", data6, data7, data8, data9);
  uint64_t output = unscrambleBits(data6, data7, data8, data9);
  DEBUG_PRINT("readBeams ");
  print_binary(output);
  DEBUG_PRINT("\n");
  return output;
}

uint32_t iter = 0;
uint32_t sumelapsed = 0;
uint32_t maxelapsed = 0;


volatile uint32_t last_ts = 0;
volatile uint32_t last_data6 = 0;
volatile uint32_t last_data7 = 0;
volatile uint32_t last_data8 = 0;
volatile uint32_t last_data9 = 0;


inline void captureTimeChange(void)
{
  noInterrupts();
  
  register uint32_t ts     = ARM_DWT_CYCCNT;
  register uint32_t data6  = (IMXRT_GPIO6_DIRECT&PORT6_MASK);  // 0B11111111111111110011000000000000
  register uint32_t data7  = (IMXRT_GPIO7_DIRECT&PORT7_MASK);
  register uint32_t data8  = (IMXRT_GPIO8_DIRECT&PORT8_MASK);
  register uint32_t data9  = (IMXRT_GPIO9_DIRECT&PORT9_MASK);
  
  if (gpioIdx < MAX_TRIGGERS_PER_SHOT)
  {
    if ((gpioIdx == 0 ||
          data6 != last_data6 ||
          data7 != last_data7 ||
          data8 != last_data8 ||
          data9 != last_data9) )
    {
      gpio6Vals[gpioIdx] = last_data6 = data6;
      gpio7Vals[gpioIdx] = last_data7 = data7;
      gpio8Vals[gpioIdx] = last_data8 = data8;
      gpio9Vals[gpioIdx] = last_data9 = data9;
      gpioTimes[gpioIdx] = last_ts    = ts;

      gpioIdx++;
    }
  }

  interrupts();
}



#endif // __PORT_IO_H__
