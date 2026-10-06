/**
 * @file main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

// #define SLEEP_MS 1

int main(void) {
  while (1) {
  }
  return 0;
}


  //k_msleep(SLEEP_MS);

/*if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }
    */