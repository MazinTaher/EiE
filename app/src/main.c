/**
 * @file main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 1

int main(void) {

  printk("Before BTN init\n");
  if (0 > BTN_init()) {
    return 0;
  }

  printk("After BTN init\n");

  printk("Before LED init\n");
  if (0 > LED_init()) {
    return 0;
  }
  printk("After init\n");
  printk("Starting loop\n");


  
  while (1) {
    k_msleep(SLEEP_MS);
  }
  return 0;
}
