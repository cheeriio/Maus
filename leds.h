#ifndef _COM_LEDS_H_
#define _COM_LEDS_H_

#define RED_LED_GPIO GPIOA
#define GREEN_LED_GPIO GPIOA
#define BLUE_LED_GPIO GPIOB
#define GREEN2_LED_GPIO GPIOA

#define RED_LED_PIN 6
#define GREEN_LED_PIN 7
#define BLUE_LED_PIN 0
#define GREEN2_LED_PIN 5

#define LED_ON 1
#define LED_OFF 0

static int red_state, green_state, blue_state, green2_state;

#define RedLEDon() \
    do { \
        red_state = LED_ON; \
        RED_LED_GPIO->BSRR = 1 << (RED_LED_PIN + 16); \
    } while(0)
    
#define RedLEDoff() \
    do { \
        red_state = LED_OFF; \
        RED_LED_GPIO->BSRR = 1 << RED_LED_PIN; \
    } while(0)
    
#define RedLEDToggle() \
    do { \
        if(red_state == LED_OFF) { \
            RedLEDon(); \
        } else { \
            RedLEDoff(); \
        } \
    } while(0)


#define GreenLEDon() \
    do { \
        green_state = LED_ON; \
        GREEN_LED_GPIO->BSRR = 1 << (GREEN_LED_PIN + 16); \
    } while(0)
    
#define GreenLEDoff() \
    do { \
        green_state = LED_OFF; \
        GREEN_LED_GPIO->BSRR = 1 << GREEN_LED_PIN; \
    } while(0)
    
#define GreenLEDToggle() \
    do { \
        if(green_state == LED_OFF) { \
            GreenLEDon(); \
        } else { \
            GreenLEDoff(); \
        } \
    } while(0)


#define BlueLEDon() \
    do { \
        blue_state = LED_ON; \
        BLUE_LED_GPIO->BSRR = 1 << (BLUE_LED_PIN + 16); \
    } while(0)
    
#define BlueLEDoff() \
    do { \
        blue_state = LED_OFF; \
        BLUE_LED_GPIO->BSRR = 1 << BLUE_LED_PIN; \
    } while(0)
    
#define BlueLEDToggle() \
    do { \
        if(blue_state == LED_OFF) { \
            BlueLEDon(); \
        } else { \
            BlueLEDoff(); \
        } \
    } while(0)


#define Green2LEDon() \
    do { \
        green2_state = LED_ON; \
        GREEN2_LED_GPIO->BSRR = 1 << GREEN2_LED_PIN; \
    } while(0)
    
#define Green2LEDoff() \
    do { \
        green2_state = LED_OFF; \
        GREEN2_LED_GPIO->BSRR = 1 << (GREEN2_LED_PIN + 16); \
    } while(0)
    
#define Green2LEDToggle() \
    do { \
        if(green2_state == LED_OFF) { \
            Green2LEDon(); \
        } else { \
            Green2LEDoff(); \
        } \
    } while(0)

#endif