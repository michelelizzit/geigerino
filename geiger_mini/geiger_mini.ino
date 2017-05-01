/*  
 *  Geiger - low-power geiger counter
 *  Author: Michele Lizzit <michele@lizzit.it> - lizzit.it
 *  v1.0 - 30/4/2017
 *  
 *  Please go to https://lizzit.it/geiger for more informations about the project
 *  
 *  
 *  Copyright (C) 2017  Michele Lizzit
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "Config.h"

int events;
boolean prev_button1;
boolean button1;
boolean buzzerEnabled;

#ifdef DEBUG
const int DEBUG_VAR = 1;
#endif
#ifndef DEBUG
const int DEBUG_VAR = 0;
#endif

void setup() {
  events = 0;
	button1 = false;
	prev_button1 = false;
  buzzerEnabled = false;
  
	pinMode(BUZZER, OUTPUT);
	pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RGB_GND, OUTPUT);
  digitalWrite(RGB_GND, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, LOW);

  power_adc_disable(); //ADC is not used in the geiger, disabling it reduces power consumption
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  //attach an interrupt on the geiger pin
  attachInterrupt(GEIGER, count, RISING);
  
  //Enable Pin Change interrupts for the button
  cli(); //disable interrupt while setting these registers
  PCICR |= 0b00000100; //enable Pin Change Interrupts on pin[23:16]
  PCMSK2 |= 0b00010000; //enable Pin Change Interrupts on pin 16
  sei(); //reenable interrupts
}

//ISR called when the button is pressed
ISR (PCINT2_vect) {
  //read the button state
  button1 = PIND & 0b00010000;
  //if the button is pressed
  if (button1) {
      //enable/disable the buzzer
      buzzerEnabled = !buzzerEnabled;
      //turn on the button LED to give a feedback to the user
      //it will be turned off next time the CPU wakes up
      digitalWrite(BUTTON_LED, HIGH);
  }
}

void loop() {
  //If one or more particles have been detected by the geiger
	while (events > 0) {
		events--;

    //When a particle is detected activate the buzzer for 3ms
    if (buzzerEnabled) digitalWrite(BUZZER, HIGH);
    PORTD = PORTD | 0b11000000; //enable the LED for 3ms
		delay(3);
		if (buzzerEnabled) digitalWrite(BUZZER, LOW);
    delay(50); //wait 50ms
    PORTD = PORTD & 0b00111111; //disable the LED
    
    digitalWrite(BUZZER, LOW);
    digitalWrite(BUTTON_LED, LOW);
	}

  //Enter sleep, an interrupt will wake the CPU when something happens
  sleep_enable();
  sleep_mode();
  //When an interrupt is called disable sleep
  sleep_disable();
}

//ISR called when the geiger detects a particle
void count() {
	events++;
}

