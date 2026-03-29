// Set this to true in order to configure the RTC's time to the compilation time.
#define SET_TIME true

// Set this to true in order for the microcontroller to print out debug messages (via serial communication).
#define SET_DBG true

// The starting hour of the bookmark.
#define HOUR_OFFSET 3

// Color of LED, when combined together (two LEDs will be lit up, so this is the
// sum of both). Default: rgb(180, 60, 0) (amber).
//
// It is recommended to set the
// value here to multiples of 60 so that the linear interpolation with
// resolution of COLOR ÷ 60 will be able to replicate the original color values
// listed below.

// Red channel
#define COLOR_R 180
// Green channel
#define COLOR_G 60
// Blue channel
#define COLOR_B 0

// Baud rate (for serial communication, used only for debugging if the debug flag is set).
#define SERIAL_BAUD_RATE 38400

// The pin for sending the LED array data.
#define LED_PIN PIN_PA5

// Count of LEDs.
#define LED_COUNT 24

// Interupt pin used for sleep & wake-up.
#define PIN_INT PIN_PA2