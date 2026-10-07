/**
 * @file main.c
 */

#include <inttypes.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 250

#define PASS_LEN 4

#define STATE_WAITING 0
#define STATE_LOCKED 1

int main(void) {

   if (0 > LED_init()) {
    return 0;
  }
  
  if (0 > BTN_init()){
    return 0;
  }

  LED_set(LED0, LED_ON);
  LED_set(LED1, LED_OFF);
  LED_set(LED2, LED_OFF);
  LED_set(LED3, LED_OFF);

  char password[5] = "0120";
  char user_pass[5] = "";
  int i = 0;

  int state = STATE_LOCKED;

  while (1) {

    if(state == STATE_LOCKED){
      LED_set(LED0, LED_ON);

      if(BTN_check_clear_pressed(BTN0) && i < PASS_LEN){
        user_pass[i] = '0';
        i++;
        user_pass[i] = '\0';
        printk("Password: %s\n", user_pass);
      }

      if(BTN_check_clear_pressed(BTN1) && i < PASS_LEN){
        user_pass[i] = '1';
        i++;
        user_pass[i] = '\0';
        printk("Password: %s\n", user_pass);
      }

      if(BTN_check_clear_pressed(BTN2) && i < PASS_LEN){
        user_pass[i] = '2';
        i++;
        user_pass[i] = '\0';
        printk("Password: %s\n", user_pass);
      }

      if(BTN_check_clear_pressed(BTN3)){

        if(strcmp(password, user_pass) == 0){
          printk("Correct!\n");
        }
        else{
          printk("Incorrect!\n");
        }

        LED_set(LED0, LED_OFF);
        state = STATE_WAITING;
      }
      k_msleep(SLEEP_MS);
    }

    if(state == STATE_WAITING){
      LED_set(LED0, LED_OFF);

      if(BTN_check_clear_pressed(BTN0) || BTN_check_clear_pressed(BTN1) || 
        BTN_check_clear_pressed(BTN2) || BTN_check_clear_pressed(BTN3)){
          state = STATE_LOCKED;
          user_pass[0] = '\0';
          i = 0;
        k_msleep(SLEEP_MS);
        }
      } 
    }
  return 0;
}
