#include <Arduino.h>
#include <i2c_driver.h>
#include "imx_rt1060/imx_rt1060_i2c_driver.h"

#include "common.h"
#include "port_io.h"

// Variables for storing the mode of operation and any error codes
volatile uint8_t lastTriggerValue = 0;
volatile Mode_t mode = INITIALIZING;
volatile Error_t errCode = NONE_ERROR;

void triggerISR(void);

#if defined(__IMXRT1062__)
extern "C" uint32_t set_arm_clock(uint32_t frequency); //Serial.println(F_CPU_ACTUAL);
extern float tempmonGetTemp(void); // in deg-celsius
#endif

void setup() {
  Serial.begin(115200);
 
  setup_port_io();

  // Go into the waiting mode
  mode = WAITING;

  // mode = ARMED;
  // triggerISR();

  

}


void loop() {

  if (mode == FIRING)
  {
    uint8_t currTriggerValue = digitalReadFast(TRIGGER_PIN);

    if (currTriggerValue != lastTriggerValue)
    {
      DEBUG_PRINT("WARNING: Noise on trigger line after firing\n");
      lastTriggerValue = currTriggerValue;
    }
    
    captureTimeChange();

    if (ARM_DWT_CYCCNT > shotStopTime)
    {
      mode = WAITING;

      DEBUG_PRINT("Number of triggers: %ld\n", gpioIdx);      
      for(uint32_t i = 0 ; i < gpioIdx ; i++)
      {
        if (i > MAX_TRIGGERS_PER_SHOT)
        {
           DEBUG_PRINT("Too many beam triggers\n");
           return;
        }
        
        volatile uint32_t timeval = gpioTimes[i];
        volatile uint64_t output = unscrambleBits(gpio6Vals[i], gpio7Vals[i], gpio8Vals[i], gpio9Vals[i]);       
        DEBUG_PRINT("\tTime: %3.9f Output: 0x%016llx\n", (double)timeval/(double)816000000, output);
      }
    }
  }  
  else
  {
    for (int i = 0 ; i < 10000 ; i++)
    {
      long unsigned ts = ARM_DWT_CYCCNT;
      captureTimeChange();
      if (ARM_DWT_CYCCNT > shotStopTime)
      {
        DEBUG_PRINT("Begin processing beams\n");
        mode = WAITING;
      }
      long unsigned te = ARM_DWT_CYCCNT;
      long unsigned el = te-ts;
      if ( el > maxelapsed)
        maxelapsed = el;
      sumelapsed += el;
      iter++;
    
    }
    Serial.print((double)sumelapsed/(double)iter);
    Serial.print("   ");
    Serial.println(maxelapsed);

    sumelapsed = 0;
    maxelapsed = 0;
    iter = 0;
  }
}

void triggerISR(void)
{
  if (mode == ARMED)
  {
    DEBUG_PRINT("Trigger set\n");
    ARM_DWT_CYCCNT = 0;
    mode = FIRING;
    gpioIdx = 0;
    lastTriggerValue = 0;
  }
}
