/**
 * @file main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 500
#define SLEEP_MS_FAST 100

int main(void) {
  LED_init();
  LED_set(LED0, LED_OFF);
  LED_set(LED1, LED_OFF);
  LED_set(LED2, LED_OFF);
  LED_set(LED3, LED_OFF);

  while (1) {
    k_msleep(SLEEP_MS);
    LED_toggle(LED0);
    LED_toggle(LED1);
    LED_toggle(LED3);
    LED_toggle(LED2);
   
  }
  return 0;
}


  

/*if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }
    */