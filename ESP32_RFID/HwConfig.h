/*
 * Supported boards
 *   BOARD_V1_0, BOARD_V1_1, BOARD_V1_2, 
 * Define one of these
 */
//#define BOARD_V1_0
#define BOARD_V1_1
//#define BOARD_V1_2

//#define USE_SDCARD

//#define NO_DISPLAY // Define this to prevent all attempts to use a display
#define USING_ST7735 // Define if Display is ST7735 / else SSD1306

#define RC522_CS_PIN    21
#define RC522_RST_PIN   22

#define ST7735_CS_PIN    5
#define ST7735_RST_PIN   4  // Or set to -1 and connect to Arduino RESET pin
#define ST7735_DC_PIN    2

#if defined (BOARD_V1_1) || defined(BOARD_V1_2)
#define ADJUSTABLE_BACKLIGHT
#define ST7735_LED_PIN  15
#ifdef USE_SDCARD
#define SDCARD_PIN      16
#endif
#endif

#ifdef BOARD_V1_0
#define AUDIO_PIN       16 // ESP32 pin connected to piezo buzzer
#else
#define AUDIO_PIN       17 // ESP32 pin connected to piezo buzzer
#endif

#define BACKGROUND_COLOR BLACK
#define TEXT_COLOR       WHITE
