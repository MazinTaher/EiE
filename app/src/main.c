/**
 * @file main.c
 */

#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 250
#define START_UP_MAX 3000

#define PASS_LEN 4

#define STATE_WAITING 0
#define STATE_LOCKED 1
#define STATE_ENTRY 2

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

  char password[5] = "";
  char user_pass[5] = "";
  char start_key[5] = "0120";
  bool new_pass = 0;
  int i = 0;
  int j = 0;

  int start_up_counter = 0;

  int state = STATE_LOCKED;

  while (1) {
     
    if(start_up_counter <= 3000){
      start_up_counter += SLEEP_MS;
    }

    if(state == STATE_LOCKED){
      LED_set(LED0, LED_ON);
      if(start_up_counter <= START_UP_MAX){
        LED_set(LED3, LED_ON);
      }
      else{
        LED_set(LED3, LED_OFF);
      }

      if(start_up_counter <= START_UP_MAX && BTN_check_clear_pressed(BTN3)){
        state = STATE_ENTRY;
      }

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

        if(!new_pass && strcmp(user_pass, start_key) == 0){
          printk("Correct!\n");
        }
        else if(new_pass && strcmp(password, user_pass) == 0){
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

    else if(state == STATE_WAITING){
      LED_set(LED0, LED_OFF);

      if(BTN_check_clear_pressed(BTN0) || BTN_check_clear_pressed(BTN1) || 
        BTN_check_clear_pressed(BTN2) || BTN_check_clear_pressed(BTN3)){
          state = STATE_LOCKED;
          user_pass[0] = '\0';
          i = 0;
        k_msleep(SLEEP_MS);
        }
      }

    else if(state == STATE_ENTRY){
      LED_blink(LED3, LED_2HZ);
      start_up_counter = 3001;

      if(BTN_check_clear_pressed(BTN0) && j < PASS_LEN){
        password[j] = '0';
        j++;
        password[j] = '\0';
        printk("Password selection: %s\n", password);
      }

      if(BTN_check_clear_pressed(BTN1) && j < PASS_LEN){
        password[j] = '1';
        j++;
        password[j] = '\0';
        printk("Password selection: %s\n", password);
      }

      if(BTN_check_clear_pressed(BTN2) && j < PASS_LEN){
        password[j] = '2';
        j++;
        password[j] = '\0';
        printk("Password selection: %s\n", password);
      }

      if(BTN_check_clear_pressed(BTN3)){
        LED_set(LED3, LED_OFF);
        new_pass = 1;
        state = STATE_LOCKED;
      }
      k_msleep(SLEEP_MS);
    }
  }
  return 0;
}
