#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

int main(void){
    int led0_active;
    int led1_active;
    int led2_active;
    int led3_active;

    if(!gpio_is_ready_dt(&led0) || 
       !gpio_is_ready_dt(&led1) || 
       !gpio_is_ready_dt(&led2) || 
       !gpio_is_ready_dt(&led3) ){
        return -1;
    }

    led0_active = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
    led1_active= gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
    led2_active= gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
    led3_active= gpio_pin_configure_dt(&led3, GPIO_OUTPUT_ACTIVE);

    if(led0_active < 0) {
        return led0_active;
    }
    else if (led1_active < 0) {
        return led3_active;
    }
    else if (led2_active < 0) {
        return led3_active;
    }
    else if (led3_active < 0) {
        return led3_active;
    }

    while(1){
        k_msleep(500);
        gpio_pin_set_dt(&led0, 0);
        k_msleep(500);
        gpio_pin_set_dt(&led0, 1);
        gpio_pin_toggle_dt(&led1);
        gpio_pin_toggle_dt(&led2);
        gpio_pin_toggle_dt(&led3);
    }

    return 0;
}