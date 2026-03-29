/** Copyright 2026 DaringCuteSeal/Cikitta T.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions: The above copyright
 * notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
 * EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <Arduino.h>
#include <tinyNeoPixel.h>
#include <DS3231-RTC.h>
#include "cfg.h"

tinyNeoPixel led_strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
DS3231 rtc;
RTClib rtclib;
uint16_t pixelNumber = LED_COUNT;

bool isr_is_triggered = false;

#if SET_TIME == true
DateTime compile_time(__DATE__, __TIME__);
#endif

#if SET_TIME == true
void set_time()
{
  rtc.setEpoch(compile_time.getUnixTime());
  DateTime now = rtclib.now();
#if SET_DBG == true
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println(F("Time has been set to: "));
  Serial.print(now.getYear(), DEC);
  Serial.print("-");
  Serial.print(now.getMonth(), DEC);
  Serial.print("-");
  Serial.print(now.getDay(), DEC);
  Serial.print(" ");
  Serial.print(now.getHour(), DEC);
  Serial.print(":");
  Serial.print(now.getMinute(), DEC);
  Serial.print(":");
  Serial.print(now.getSecond(), DEC);
  Serial.end();
#endif
}
#endif

void isr()
{
  isr_is_triggered = true;
}

// Set an alarm that calls ISR on the next (multiple of 10) minute.
void set_alarm()
{
  uint8_t min_now = rtc.getMinute();
  rtc.enableOscillator(false, false, 0);

  byte alarm_day = 0;
  byte alarm_hour = 0;
  byte alarm_min = (min_now - (min_now % 10) + 10) % 60; // next wake-up minute, multiples of 10
  byte alarm_sec = 0;
  byte alarm_bits = 0b01100000; // minute & second matches
  bool alarm_day_isday = false;
  bool alarm_H12 = false;
  bool alarm_PM = false;

  rtc.setA1Time(alarm_day, alarm_hour, alarm_min, alarm_sec, alarm_bits, alarm_day_isday, alarm_H12, alarm_PM);
  rtc.turnOnAlarm(1);

  // disable the second alarm completely.

  alarm_day = 1;
  alarm_hour = 1;
  alarm_day_isday = false;
  alarm_H12 = false;
  alarm_PM = false;
  alarm_min = 0xFF;       // give gibberish value (255)
  alarm_min = 0b01100000; // when min matches (never)

  rtc.setA2Time(alarm_day, alarm_hour, alarm_min, alarm_bits, alarm_day_isday, alarm_H12, alarm_PM);
  rtc.turnOffAlarm(2);

  attachInterrupt(PIN_INT, isr, FALLING);
}

inline void unlock_ccp()
{
  CCP = CCP_IOREG_gc;
}

void set_bod_config()
{
  cli(); // Disable interrupts (for writing to the configuration change protected register)
  // set brownout detector to sampled when sleeping to save power
  unlock_ccp();
  BOD_CTRLA = (BOD_CTRLA & ~BOD_SLEEP_gm) | BOD_SLEEP_SAMPLED_gc;
  sei();
}

// Sets the system to power down mode.
void system_power_down()
{
  SLPCTRL_CTRLA = (SLPCTRL_CTRLA & ~SLPCTRL_SMODE_gm) | SLPCTRL_SMODE_PDOWN_gc;
  SLPCTRL_CTRLA |= SLPCTRL_SEN_bm;
  asm("SLEEP");
}

void setup()
{
#if SET_TIME == true
  set_time();
#endif
  Wire.begin();
  led_strip.begin();
  led_strip.show();
  led_strip.setBrightness(50);
  set_bod_config();
  isr_is_triggered = false;
  delay(2000);
  set_alarm();
  sei();
  system_power_down();
}

void loop()
{
  if (isr_is_triggered)
  {
    rtc.turnOffAlarm(1);
    set_alarm();
  }
}
