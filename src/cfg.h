// Set this to true in order to configure the RTC's time to the compilation time.
#define SET_TIME true

// Set this to true in order for the microcontroller to print out debug messages (via serial communication).
#define SET_DBG true

// Color of LED. Default: rgb(146, 61, 4) (dim amber)
#define COLOR_R 146
#define COLOR_G 61
#define COLOR_B 4

// Baud rate (for serial communication, used only for debugging if the debug flag is set).
#define SERIAL_BAUD_RATE 38400

// The pin for sending the LED array data.
#define LED_PIN PIN_PA5

// Count of LEDs.
#define LED_COUNT 24

// Interupt pin used for sleep & wake-up.
#define PIN_INT PIN_PA2