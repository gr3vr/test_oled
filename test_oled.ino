#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define ledPin        2

#define LOGO_HEIGHT   32
#define LOGO_WIDTH    128

int y = 16;

static const uint8_t steps[16]        = {0x03, 0x00, 0x33, 0x80, 0x73, 0x80, 0x70, 0x80, 0x41, 0x80, 0x63, 0x80, 0x73, 0x00, 0x30, 0x00};
static const uint8_t check[16]        = {0x00, 0x00, 0x01, 0x80, 0x03, 0x80, 0x67, 0x00, 0x7e, 0x00, 0x3c, 0x00, 0x18, 0x00, 0x00, 0x00};
static const uint8_t alert[16]        = {0x00, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0c, 0x00};
static const uint8_t acceleration[16] = {0x1c, 0x00, 0x0c, 0x00, 0x14, 0x40, 0x20, 0x80, 0x41, 0x00, 0x8a, 0x00, 0x0c, 0x00, 0x0e, 0x00};
static const uint8_t stepsV2[16]      = {0x70, 0x00, 0x73, 0x80, 0x73, 0x80, 0x03, 0x80, 0x70, 0x00, 0x73, 0x80, 0x03, 0x80, 0x00, 0x00};
static const uint8_t stepsV3[16]      = {0x03, 0x00, 0x33, 0x80, 0x73, 0x80, 0x71, 0x80, 0x60, 0x00, 0x03, 0x80, 0x73, 0x00, 0x30, 0x00};
static const uint8_t velocity[16]     = {0x00, 0x00, 0x8f, 0xc0, 0x68, 0x40, 0x08, 0x40, 0xd8, 0x40, 0x08, 0x40, 0x6f, 0xc0, 0x00, 0x00};
static const uint8_t velocityV2[16]   = {0xf0, 0xc0, 0x06, 0xc0, 0xe9, 0x00, 0x03, 0xc0, 0x66, 0x00, 0x09, 0x80, 0x10, 0x80, 0x61, 0x00};

//---------------------- sprites animation load ------------------------------------------------

const unsigned char PROGMEM frames [9][33] = {
  {0x00, 0x00, 0x01, 0x80, 0x03, 0xC0, 0x02, 0x48, 0x3C, 0x3C, 0x20, 0x84, 0x33, 0xCC, 0x12, 0x48, 0x16, 0x68, 0x33, 0xCC, 0x20, 0x84, 0x3E, 0x3C, 0x02, 0x40, 0x03, 0xC0, 0x00, 0x80, 0x00, 0x00},
  {0x00, 0x00, 0x01, 0x80, 0x01, 0xC0, 0x1B, 0x60, 0x3C, 0x3C, 0x31, 0x1C, 0x13, 0xC4, 0x12, 0x4C, 0x36, 0x68, 0x23, 0xC8, 0x28, 0x8C, 0x3C, 0x3C, 0x06, 0xD8, 0x03, 0x80, 0x02, 0x80, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x80, 0x01, 0xE0, 0x1F, 0x20, 0x34, 0x30, 0x10, 0x9C, 0x13, 0xC6, 0x12, 0x4C, 0x76, 0x68, 0x23, 0xC8, 0x38, 0x88, 0x0C, 0x2C, 0x04, 0xF8, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x40, 0x0C, 0xE0, 0x1F, 0xA0, 0x10, 0x30, 0x19, 0x1C, 0x1B, 0xCC, 0x32, 0x46, 0x66, 0x6C, 0x23, 0xC8, 0x39, 0x18, 0x0C, 0x48, 0x0D, 0xF8, 0x07, 0x10, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x00, 0x0C, 0x70, 0x1B, 0x90, 0x18, 0x30, 0x08, 0x90, 0x13, 0xCC, 0x72, 0x44, 0x66, 0x6E, 0x33, 0xC8, 0x08, 0x90, 0x08, 0x10, 0x0B, 0xD8, 0x0E, 0x30, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x00, 0x0E, 0x70, 0x0B, 0xD8, 0x08, 0x10, 0x08, 0x90, 0x33, 0xD8, 0x26, 0x46, 0x62, 0x66, 0x1B, 0xCC, 0x08, 0x90, 0x08, 0x10, 0x1B, 0xD0, 0x06, 0x70, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x02, 0x00, 0x07, 0x30, 0x0D, 0xF8, 0x0C, 0x18, 0x39, 0x10, 0x33, 0xD8, 0x62, 0x4C, 0x36, 0x66, 0x13, 0xCE, 0x19, 0x98, 0x10, 0x30, 0x1F, 0xB0, 0x0C, 0xF0, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x01, 0x00, 0x07, 0x00, 0x05, 0xF8, 0x0C, 0x08, 0x38, 0x18, 0x23, 0xC8, 0x72, 0x48, 0x16, 0x66, 0x13, 0xC4, 0x10, 0x9C, 0x10, 0x30, 0x1F, 0x20, 0x00, 0xE0, 0x00, 0x80, 0x00, 0x00},
  {0x00, 0x00, 0x03, 0x00, 0x03, 0x80, 0x06, 0xF8, 0x1C, 0x6C, 0x39, 0x0C, 0x63, 0xC8, 0x36, 0x48, 0x16, 0x6C, 0x13, 0xC6, 0x30, 0x9C, 0x34, 0x38, 0x1F, 0x60, 0x01, 0xE0, 0x00, 0x80, 0x00, 0x00}
};

const unsigned char PROGMEM error_imu[] = {0x03, 0xc0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x03, 0xc0, 0x01, 0x80};

//----------------------------------------------------------------------------------------------

//---------------------- battery sprites -------------------------------------------------------
const unsigned char PROGMEM batt_0 []           = {0x3F, 0xFF, 0xF0, 0x7F, 0xFF, 0xF8, 0xC0, 0x00, 0x0C, 0xC0, 0x00, 0x0C, 0xC0, 0x00, 0x0C, 0xC0, 0x00, 0x0C, 0xC0, 0x00, 0x0F, 0xC0, 0x00, 0x0F, 0xC0, 0x00, 0x0F, 0xC0, 0x00, 0x0F, 0xC0, 0x00, 0x0F, 0xC0, 0x00, 0x0C, 0xC0, 0x00, 0x0C, 0xC0, 0x00, 0x0C, 0x7F, 0xFF, 0xF8, 0x3F, 0xFF, 0xF0};
const unsigned char PROGMEM batt_25 []          = {0x3F, 0xFF, 0xF0, 0x7F, 0xFF, 0xF8, 0xC0, 0x00, 0x0C, 0xDE, 0x00, 0x0C, 0xDE, 0x00, 0x0C, 0xDE, 0x00, 0x0C, 0xDE, 0x00, 0x0F, 0xDE, 0x00, 0x0F, 0xDE, 0x00, 0x0F, 0xDE, 0x00, 0x0F, 0xDE, 0x00, 0x0F, 0xDE, 0x00, 0x0C, 0xDE, 0x00, 0x0C, 0xC0, 0x00, 0x0C, 0x7F, 0xFF, 0xF8, 0x3F, 0xFF, 0xF0};
const unsigned char PROGMEM batt_50 []          = {0x3F, 0xFF, 0xF0, 0x7F, 0xFF, 0xF8, 0xC0, 0x00, 0x0C, 0xDF, 0xE0, 0x0C, 0xDF, 0xE0, 0x0C, 0xDF, 0xE0, 0x0C, 0xDF, 0xE0, 0x0F, 0xDF, 0xE0, 0x0F, 0xDF, 0xE0, 0x0F, 0xDF, 0xE0, 0x0F, 0xDF, 0xE0, 0x0F, 0xDF, 0xE0, 0x0C, 0xDF, 0xE0, 0x0C, 0xC0, 0x00, 0x0C, 0x7F, 0xFF, 0xF8, 0x3F, 0xFF, 0xF0};
const unsigned char PROGMEM batt_75 []          = {0x3F, 0xFF, 0xF0, 0x7F, 0xFF, 0xF8, 0xC0, 0x00, 0x0C, 0xDF, 0xFE, 0x0C, 0xDF, 0xFE, 0x0C, 0xDF, 0xFE, 0x0C, 0xDF, 0xFE, 0x0F, 0xDF, 0xFE, 0x0F, 0xDF, 0xFE, 0x0F, 0xDF, 0xFE, 0x0F, 0xDF, 0xFE, 0x0F, 0xDF, 0xFE, 0x0C, 0xDF, 0xFE, 0x0C, 0xC0, 0x00, 0x0C, 0x7F, 0xFF, 0xF8, 0x3F, 0xFF, 0xF0};
const unsigned char PROGMEM batt_100 []         = {0x3F, 0xFF, 0xF0, 0x7F, 0xFF, 0xF8, 0xC0, 0x00, 0x0C, 0xDF, 0xFF, 0xEC, 0xDF, 0xFF, 0xEC, 0xDF, 0xFF, 0xEC, 0xDF, 0xFF, 0xEF, 0xDF, 0xFF, 0xEF, 0xDF, 0xFF, 0xEF, 0xDF, 0xFF, 0xEF, 0xDF, 0xFF, 0xEF, 0xDF, 0xFF, 0xEC, 0xDF, 0xFF, 0xEC, 0xC0, 0x00, 0x0C, 0x7F, 0xFF, 0xF8, 0x3F, 0xFF, 0xF0};
const unsigned char PROGMEM batt_empty []       = {0x3F, 0xFF, 0xF0, 0x7F, 0xFF, 0xF8, 0xC0, 0x00, 0x0C, 0xC0, 0x78, 0x0C, 0xC0, 0xFC, 0x0C, 0xC0, 0xFC, 0x0C, 0xC0, 0xFC, 0x0F, 0xC0, 0xFC, 0x0F, 0xC0, 0x78, 0x0F, 0xC0, 0x30, 0x0F, 0xC0, 0x00, 0x0F, 0xC0, 0x30, 0x0C, 0xC0, 0x30, 0x0C, 0xC0, 0x00, 0x0C, 0x7F, 0xFF, 0xF8, 0x3F, 0xFF, 0xF0};
const unsigned char PROGMEM batt_empty_short [] = {0x7f, 0xfc, 0xff, 0xfe, 0xc0, 0x06, 0xd0, 0x07, 0xd0, 0x07, 0xd0, 0x07, 0xd0, 0x07, 0xc0, 0x06, 0xff, 0xfe, 0x7f, 0xfc};

//----------------------------------------------------------------------------------------------

void header(int pasos, int pc, int par, int hi, int hv) {
  
  
  
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  digitalWrite(ledPin, HIGH);
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //display.display();
  //delay(1000); // Pause for 1 second

  // Clear the buffer
  display.clearDisplay();

  // Draw bitmap orbittas logo on the screen
  //display.drawBitmap(0, 0, ORBITTAS_BMP_LOGO, 128, 64, 1);
  //display.display();
  delay(300);

  digitalWrite(ledPin, LOW);
}

void loop() {
  int tt = 4;
  int xx = 59;
  int yy = y + 9;
  tt = 500;
  int current_time = millis();

  while (millis() - current_time <= 2500) {                   // Modo operacion, hora
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    //display.drawRect(0, y, 128, 32, WHITE);                 // Para saber el tamaño de la pantalla a usar

    display.drawBitmap(10, y, stepsV3, 10, 8, 1);              // Logo stepsV3
    display.drawBitmap(43, y, check, 10, 8, 1);               // Logo check
    display.drawBitmap(64, y, alert, 10, 8, 1);               // Logo alert
    display.drawBitmap(87, y, acceleration, 10, 8, 1);        // Logo acceleration
    display.drawBitmap(115, y, velocityV2, 10, 8, 1);         // Logo velocityV2

    display.setCursor(0, y + 9);
    display.println(F("00000"));
    display.setCursor(38, y + 9);
    display.println(F("000"));
    display.setCursor(64, y + 9);
    display.println(F("00"));
    display.setCursor(87, y + 9);
    display.println(F("00"));
    display.setCursor(115, y + 9);
    display.println(F("00"));

    display.setTextSize(2);
    display.setCursor(20, y + 18);
    display.println(F("10:03 am"));

    display.display();
  }

  current_time = millis();
  while (millis() - current_time <= 2500) {                   // Modo operacion, bateria baja
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    //display.drawRect(0, y, 128, 32, WHITE);                 // Para saber el tamaño de la pantalla a usar

    display.drawBitmap(10, y, stepsV3, 10, 8, 1);              // Logo stepsV3
    display.drawBitmap(43, y, check, 10, 8, 1);               // Logo check
    display.drawBitmap(64, y, alert, 10, 8, 1);               // Logo alert
    display.drawBitmap(87, y, acceleration, 10, 8, 1);        // Logo acceleration
    display.drawBitmap(115, y, velocityV2, 10, 8, 1);         // Logo velocityV2

    display.setCursor(0, y + 9);
    display.println(F("00010"));
    display.setCursor(38, y + 9);
    display.println(F("003"));
    display.setCursor(64, y + 9);
    display.println(F("02"));
    display.setCursor(87, y + 9);
    display.println(F("99"));
    display.setCursor(115, y + 9);
    display.println(F("87"));

    display.setTextSize(1);
    display.setCursor(20 , y + 23);
    display.println(F("BATERIA BAJA"));
    display.drawBitmap(xx + 38, y + 21, batt_empty_short, 16, 10, 1);    // Modo operacion -> batería baja

    display.display();
  }

  current_time = millis();
  while (millis() - current_time <= 2500) {                   // Modo operacion, Desconfigurado
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    //display.drawRect(0, y, 128, 32, WHITE);                 // Para saber el tamaño de la pantalla a usar

    display.drawBitmap(10, y, stepsV3, 10, 8, 1);              // Logo stepsV3
    display.drawBitmap(43, y, check, 10, 8, 1);               // Logo check
    display.drawBitmap(64, y, alert, 10, 8, 1);               // Logo alert
    display.drawBitmap(87, y, acceleration, 10, 8, 1);        // Logo acceleration
    display.drawBitmap(115, y, velocityV2, 10, 8, 1);         // Logo velocityV2

    display.setCursor(0, y + 9);
    display.println(F("20022"));
    display.setCursor(38, y + 9);
    display.println(F("108"));
    display.setCursor(64, y + 9);
    display.println(F("35"));
    display.setCursor(87, y + 9);
    display.println(F("67"));
    display.setCursor(115, y + 9);
    display.println(F("12"));

    display.setTextSize(1);
    display.setCursor(2 , y + 23);
    display.println(F("EQUIPO DESCONFIGURADO"));
    //display.drawBitmap(xx + 43, y + 21, batt_empty_short, 16, 10, 1);

    display.display();
  }

  current_time = millis();
  while (millis() - current_time <= 2500) {                   // Modo operacion, Movimiento incorrecto
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    //display.drawRect(0, y, 128, 32, WHITE);                 // Para saber el tamaño de la pantalla a usar

    display.drawBitmap(10, y, stepsV3, 10, 8, 1);              // Logo stepsV3
    display.drawBitmap(43, y, check, 10, 8, 1);               // Logo check
    display.drawBitmap(64, y, alert, 10, 8, 1);               // Logo alert
    display.drawBitmap(87, y, acceleration, 10, 8, 1);        // Logo acceleration
    display.drawBitmap(115, y, velocityV2, 10, 8, 1);         // Logo velocityV2

    display.setCursor(0, y + 9);
    display.println(F("30333"));
    display.setCursor(38, y + 9);
    display.println(F("022"));
    display.setCursor(64, y + 9);
    display.println(F("00"));
    display.setCursor(87, y + 9);
    display.println(F("43"));
    display.setCursor(115, y + 9);
    display.println(F("01"));

    display.setTextSize(1);
    display.setCursor(36, y + 18);
    display.println(F("MOVIMIENTO"));
    display.setCursor(36, y + 27);
    display.println(F("INCORRECTO"));

    display.display();
  }

  current_time = millis();
  while (millis() - current_time <= 2500) {                   // Modo operacion, Movimiento correcto
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    //display.drawRect(0, y, 128, 32, WHITE);                 // Para saber el tamaño de la pantalla a usar

    display.drawBitmap(10, y, stepsV3, 10, 8, 1);              // Logo stepsV3
    display.drawBitmap(43, y, check, 10, 8, 1);               // Logo check
    display.drawBitmap(64, y, alert, 10, 8, 1);               // Logo alert
    display.drawBitmap(87, y, acceleration, 10, 8, 1);        // Logo acceleration
    display.drawBitmap(115, y, velocityV2, 10, 8, 1);         // Logo velocityV2

    display.setCursor(0, y + 9);
    display.println(F("44444"));
    display.setCursor(38, y + 9);
    display.println(F("999"));
    display.setCursor(64, y + 9);
    display.println(F("00"));
    display.setCursor(87, y + 9);
    display.println(F("77"));
    display.setCursor(115, y + 9);
    display.println(F("56"));

    display.setTextSize(1);
    display.setCursor(36, y + 18);
    display.println(F("MOVIMIENTO"));
    display.setCursor(36, y + 27);
    display.println(F(" CORRECTO "));

    display.display();
  }


  current_time = millis();
  while (millis() - current_time <= 3000) {
    int ttt = 4;
    for (int i = 0; i < 8; i++) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(2, y);
      display.println(F("  ACTIVANDO EQUIPO   "));
      display.drawBitmap(xx, yy, frames[i], 16, 16, 1);
      display.display();
      delay(ttt);
    }
  }

  current_time = millis();
  while (millis() - current_time <= 2500) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(2, y);
    display.println(F("  ACTIVANDO EQUIPO   "));
    display.drawBitmap(xx, yy, error_imu, 16, 16, 1);
    display.display();
    delay(tt);

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(2, y);
    display.println(F("  ACTIVANDO EQUIPO   "));
    display.display();
    delay(tt);
  }
  current_time = millis();
  while (millis() - current_time <= 2500) {   // Batt: 0 - 25;
    tt = 500;

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(2, y);
    display.println(F("    CARGANDO 25%     "));
    display.drawBitmap(xx - 4, yy, batt_0, 24, 16, 1);
    display.display();
    delay(tt);

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(2, y);
    display.println(F("    CARGANDO 25%     "));
    display.drawBitmap(xx - 4, yy, batt_25, 24, 16, 1);
    display.display();
    delay(tt);
  }

  current_time = millis();
  while (millis() - current_time <= 2500) {   // Batt: 0 - 50
    tt = 500;

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(2, y);
    display.println(F("    CARGANDO 50%     "));
    display.drawBitmap(xx - 4, yy, batt_0, 24, 16, 1);
    display.display();
    delay(tt);

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(2, y);
    display.println(F("    CARGANDO 50%     "));
    display.drawBitmap(xx - 4, yy, batt_50, 24, 16, 1);
    display.display();
    delay(tt);
  }

  current_time = millis();
  while (millis() - current_time <= 2500) {   // Batt: 0 - 75
    tt = 500;

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(2, y);
    display.println(F("    CARGANDO 75%     "));
    display.drawBitmap(xx - 4, yy, batt_0, 24, 16, 1);
    display.display();
    delay(tt);

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(2, y);
    display.println(F("    CARGANDO 75%     "));
    display.drawBitmap(xx - 4, yy, batt_75, 24, 16, 1);
    display.display();
    delay(tt);
  }

  current_time = millis();
  while (millis() - current_time <= 2500) {   // Batt: 0 - 100
    tt = 500;

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(2, y);
    display.println(F("    CARGANDO 99%     "));
    display.drawBitmap(xx - 4, yy, batt_0, 24, 16, 1);
    display.display();
    delay(tt);

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(2, y);
    display.println(F("    CARGANDO 99%     "));
    display.drawBitmap(xx - 4, yy, batt_100, 24, 16, 1);
    display.display();
    delay(tt);
  }

  current_time = millis();
  while (millis() - current_time <= 2500) {   // Batt: cargada

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(2, y);
    display.println(F("    CARGADA 100%     "));
    display.drawBitmap(xx - 4, yy, batt_100, 24, 16, 1);
    display.display();
  }

  current_time = millis();
  while (millis() - current_time <= 2500) {   // Batt: EMPTY
    display.clearDisplay();
    display.drawBitmap(xx - 4, y, batt_empty, 24, 16, 1);
    display.drawBitmap(xx - 4, y, batt_0, 24, 16, 1);
    display.setTextSize(1);
    display.setCursor(2, y + 23);
    display.println(F("    CARGAR BATERIA   "));
    display.display();
    delay(tt);

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(2, y + 23);
    display.println(F("    CARGAR BATERIA   "));
    display.display();
    delay(tt);
  }

  current_time = millis();
  while (millis() - current_time <= 2500) {
    display.clearDisplay();
    display.drawBitmap(xx, y, error_imu, 16, 16, 1);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(2, y + 23);
    display.println(F("  DESCARGAR LA DATA  "));
    display.display();
    delay(tt);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(2, y + 23);
    display.println(F("  DESCARGAR LA DATA  "));
    display.display();
    delay(tt);
  }

}
