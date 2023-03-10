/**
 * CFPushButton.h
 * 
 * A library for Arduino that helps interface for Push Buttons.
 * 
 * @author  Caio Frota <caiofrota@gmail.com>
 * @version 1.0
 * @since   Mar, 2021
 */

//#include <Arduino.h>
#include <functional>
#include <stm32f1xx_hal.h>


#ifndef CFPushButton_h
#define CFPushButton_h

#define LOW 0
#define HIGH 1
#define STM_PORT(X) (((uint32_t)(X) >> 4) & 0xF)
#define STM_PIN(X)  ((uint32_t)(X) & 0xF)

class CFPushButton {
    private:
        // Attributes.
        uint16_t _pinPushButton;                                                                         // Push Button Pin.
        GPIO_TypeDef* _pinBank;
        int _shortPressTime;                                                                        // Short Time (in millis). 3 seconds default.
        int _longPressTime;                                                                         // Long Time (in millis). 10 seconds default.
        int _state;                                                                                 // Button state.
        long _pressTime;                                                                            // Store the time that the button was pressed.
        bool _pulledHigh;                                                                           // Indicate the button is pulled high.
        int _callbackTime;                                                                          // Control attribute to avoid multiple callbacks.
        
        // Methods.
        void _buttonReleased(int pressedTime);                                                      // Called when the button is released.

		bool digitalRead(int pin);
        long millis()
        {return HAL_GetTick();}

	GPIO_TypeDef* getBankAndHALPin(int &pin);

        // Callbacks.
        std::function<void()> _onShortPressReachedCallback;                                         // Called when short time is reached.
        std::function<void()> _onLongPressReachedCallback;                                          // Called when long time is reached.
        std::function<void()> _onPressCallback;                                                     // Called when button is pressed.
        std::function<void()> _onShortPressCallback;                                                // Called when button is short pressed.
        std::function<void()> _onLongPressCallback;                                                 // Called when button is longe pressed.
    public:
        // Methods.
        CFPushButton( GPIO_TypeDef* pinBank, uint16_t pinPushButton, int shortPressTime = 3000, int longPressTime = 10000);                                                            // Default constructor.
        void begin();                                                                               // Initial setup.
        void loop();                                                                                // Control push button state.
        
        // Accessors.
        int getState();                                                                             // Get button state.
        bool isPulledUp();                                                                          // True if button is pulled up.
        void setPulledUp();                                                                         // Define button is pulled up.
        void setPulledDown();                                                                       // Define button is pulled down.
        int getShortPressTime();                                                                    // Get short time.
        void setShortPressTime(int shortPressTime);                                                 // Define short time.
        int getLongPressTime();                                                                     // Get long time.
        void setLongPressTime(int shortPressTime);                                                  // Define long time.
        
        // Callbacks
        void setOnShortPressReachedCallback(std::function<void()> func);                            // Define short time reached callback.
        void setOnLongPressReachedCallback(std::function<void()> func);                             // Define long time reached callback.
        void setOnPressCallback(std::function<void()> func);                                        // Define button press callback.
        void setOnShortPressCallback(std::function<void()> func);                                   // Define button short press callback.
        void setOnLongPressCallback(std::function<void()> func);                                    // Define button long press callback.
};

#endif
