/**
 * @file main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 500

int main(void) {
   if (0 > LED_init()) {
    return 0;
  }

  LED_set(LED0, LED_OFF);
  LED_set(LED1, LED_OFF);
  LED_set(LED2, LED_OFF);
  LED_set(LED3, LED_OFF);

 

  while (1) {
    k_msleep(SLEEP_MS);
    LED_toggle(LED2);
    k_msleep(SLEEP_MS);
    LED_toggle(LED2);
    LED_toggle(LED0);
    LED_toggle(LED1);
    LED_toggle(LED3);
   
  }
  return 0;
}


  

/*if (0 > BTN_init()) {
    return 0;
  }
    */