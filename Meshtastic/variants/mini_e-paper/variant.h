#define HAS_GPS 0
#undef GPS_RX_PIN
#undef GPS_TX_PIN

#define BUTTON_PIN 3 // This is the BOOT button
#define BUTTON_NEED_PULLUP

// #define HAS_SCREEN 0
// #define HAS_SDCARD
// #define USE_SPI1  

// Display (E-Ink)
#define USE_EINK
#define PIN_EINK_EN     (42)

#define PIN_EINK_MOSI   (15)
#define PIN_EINK_MISO   (-1)
#define PIN_EINK_SCLK   (14)
#define PIN_EINK_CS     (13)  

#define PIN_EINK_DC     (12)
#define PIN_EINK_RES    (11)
#define PIN_EINK_BUSY   (10)

// PCF8563 RTC Module
#define I2C_SDA     (18) // 1 // I2C pins for this board
#define I2C_SCL     (9)  // 2

#define PCF8563_RTC 0x51

/*LoRA*/
#define USE_SX1262
// #define USE_SX1280

#define LORA_DIO1   (5)
#define LORA_MISO   (6)
#define LORA_CS     (7)
#define LORA_SCK    (8)
#define LORA_MOSI   (17)
#define LORA_RESET (-1)

#ifdef USE_SX1262
#define SX126X_CS       LORA_CS
#define SX126X_DIO1     (5)
#define SX126X_BUSY     (16)
#define SX126X_RESET    LORA_RESET
#define SX126X_DIO3_TCXO_VOLTAGE    (1.8)
#endif

#ifdef USE_SX1280
#define SX128X_CS LORA_CS
#define SX128X_DIO1 LORA_DIO1
#define SX128X_BUSY 15
#define SX128X_RESET LORA_RESET
#endif
