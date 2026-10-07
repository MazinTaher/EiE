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
  
  if (0 > BTN_init()){
    return 0;
  }

  LED_set(LED0, LED_OFF);
  LED_set(LED1, LED_OFF);
  LED_set(LED2, LED_OFF);
  LED_set(LED3, LED_OFF);

  int counter = 0;

  while (1) {

    if(counter == 16){
      LED_set(LED0, LED_OFF);
      LED_set(LED1, LED_OFF);
      LED_set(LED2, LED_OFF);
      LED_set(LED3, LED_OFF);
      counter = 0;
    }



    if(BTN_check_clear_pressed(BTN0)){
      counter++;
      printk("Button 0 pressed: %d\n", counter);
    }

    if(counter % 2 == 1){
      LED_set(LED0, LED_ON);
    }
    else{
      LED_set(LED0, LED_OFF);
    }

    if(counter % 4 == 2 || counter % 4 == 3 ){
       LED_set(LED1, LED_ON);
    }
    else{
      LED_set(LED1, LED_OFF);
    }

    if(counter % 8 == 4 || counter % 8 == 5 || counter % 8 == 6 || counter % 8 == 7){
       LED_set(LED2, LED_ON);
    }
    else{
      LED_set(LED2, LED_OFF);
    }

    if(counter >= 8 && counter < 16){
      LED_set(LED3, LED_ON);
    }
    else{
      LED_set(LED3, LED_OFF);
    }

    k_msleep(SLEEP_MS);

  }
  return 0;
}
