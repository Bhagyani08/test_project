/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

// Added Thread to run blink_led concurrently
Thread blink_thread;
volatile int blink_delay_ms = 500;

void blink_led() {
  DigitalOut led(LED1);
  while (true) {
    led = !led;
    ThisThread::sleep_for(std::chrono::milliseconds(blink_delay_ms));
  }
}

int main() {
#if !defined(LED1) || !defined(BUTTON1)
#error "This example requires both LED1 and BUTTON1 pin definitions."
#endif

  DigitalOut led(LED1, 0);
  DigitalIn button(BUTTON1, PullUp);

  bool previous_pressed = false;
  int press_count = 0;

  // Start blink_led in a separate thread so it doesn't block main
  blink_thread.start(blink_led);
  printf("Application started. Blink thread launched.\n");

  while (true) {
    const bool pressed = (button.read() == 0);

    if (pressed && !previous_pressed) {
      led = !led;
      press_count++;
      
      // Update the blink rate based on press count to add complexity
      if (press_count % 3 == 0) {
          blink_delay_ms = 100; // Fast blink
      } else if (press_count % 3 == 1) {
          blink_delay_ms = 250; // Medium blink
      } else {
          blink_delay_ms = 500; // Slow blink
      }
      
      printf("Button is pressed (Count: %d)! Blink delay is now %d ms\n", press_count, blink_delay_ms);
      ThisThread::sleep_for(50ms); // Debounce delay
    }
    
    // Note: Removed direct call to blink_led() to prevent the main loop from blocking.
    
    previous_pressed = pressed;
    ThisThread::sleep_for(10ms);
  }
}
