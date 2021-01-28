//-------- DEMO OLED ---------------------

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//-------- DEFINICION DE OLEDS -----------
#define SCREEN_WIDTH          128   // OLED display anchura (px).
#define SCREEN_HEIGHT         64    // OLED display altura (px).

#define OLED_RESET            -1    // Reset pin # (or -1 if sharing Arduino reset pin).

#define ledPin                2

#define ANIMATION_NUMBER      2     // Cant. de frames que cambiará cada vez que se llama la función.
#define ANIMATION_TIME        500   // Tiempo de muestreo entre cada frame.
#define OLED_ADDRESS          0x3C  // Direccion I2C de la oled, también puede ser 0x3D.

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins).
//----------------------------------------

//-------- PANTALLAS OLED ----------------
#define HORA                  1
#define MOVIMIENTO_CORRECTO   2
#define MOVIMIENTO_INCORRECTO 3
#define BATERIA_BAJA          4
#define DESCONFIGURADO        5

#define BUFFER_LLENO          1
#define ACTIVANDO_IMU         2
#define ACTIVANDO_IMU_FALLA   3

#define BATT_ZERO             0
#define BATT_EMPTY            5
//----------------------------------------

int y = 19;                           // Offset para oled 128x64, Debe ser 0 para oled 128x32.

//----------------------- VARIABLES MODO OPERACION -------------------
int pasos;                            // Pasos.
int pc;                               // Operaciones Correctas.
int par;                              // Posiciones Alto Riesgo.
int hi;                               // Movimientos de Alta Intensidad.
int hv;                               // Operaciones Alta Velocidad.

int hour = 12;
int minute = 00;
char *amPm = "am";
//--------------------------------------------------------------------

//----------------------- VARIABLES AUX ------------------------------
int curr_time;
int tt = 4;
int xx = 59;
int yy = y + 9;
//--------------------------------------------------------------------

//----------------------- LOGOS MODO OPERACION -----------------------------------------------------------------------------------------

static const uint8_t steps[16]        = {0x03, 0x00, 0x33, 0x80, 0x73, 0x80, 0x70, 0x80, 0x41, 0x80, 0x63, 0x80, 0x73, 0x00, 0x30, 0x00};
static const uint8_t check[16]        = {0x00, 0x00, 0x01, 0x80, 0x03, 0x80, 0x67, 0x00, 0x7e, 0x00, 0x3c, 0x00, 0x18, 0x00, 0x00, 0x00};
static const uint8_t alert[16]        = {0x00, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0c, 0x00};
static const uint8_t acceleration[16] = {0x1c, 0x00, 0x0c, 0x00, 0x14, 0x40, 0x20, 0x80, 0x41, 0x00, 0x8a, 0x00, 0x0c, 0x00, 0x0e, 0x00};
static const uint8_t stepsV2[16]      = {0x70, 0x00, 0x73, 0x80, 0x73, 0x80, 0x03, 0x80, 0x70, 0x00, 0x73, 0x80, 0x03, 0x80, 0x00, 0x00};
static const uint8_t stepsV3[16]      = {0x03, 0x00, 0x33, 0x80, 0x73, 0x80, 0x71, 0x80, 0x60, 0x00, 0x03, 0x80, 0x73, 0x00, 0x30, 0x00};
static const uint8_t velocity[16]     = {0x00, 0x00, 0x8f, 0xc0, 0x68, 0x40, 0x08, 0x40, 0xd8, 0x40, 0x08, 0x40, 0x6f, 0xc0, 0x00, 0x00};
static const uint8_t velocityV2[16]   = {0xf0, 0xc0, 0x06, 0xc0, 0xe9, 0x00, 0x03, 0xc0, 0x66, 0x00, 0x09, 0x80, 0x10, 0x80, 0x61, 0x00};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//---------------------- SPRITES LOAD ANIMATION ------------------------------------------------

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
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//---------------------- FRAME FAIL IMU --------------------------------------------------------
const unsigned char PROGMEM error_imu[] = {0x03, 0xc0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x03, 0xc0, 0x01, 0x80};
//----------------------------------------------------------------------------------------------

//---------------------- battery sprites -------------------------------------------------------
/*
  Batt_frames definition:
  0: batt_0
  1: batt_25
  2: batt_50
  3: batt_75
  4: batt_100
  5: batt_empty
  6: batt_empty_small
*/
const unsigned char PROGMEM batt_frames[7][48]    = {
  {0x3F, 0xFF, 0xF0, 0x7F, 0xFF, 0xF8, 0xC0, 0x00, 0x0C, 0xC0, 0x00, 0x0C, 0xC0, 0x00, 0x0C, 0xC0, 0x00, 0x0C, 0xC0, 0x00, 0x0F, 0xC0, 0x00, 0x0F, 0xC0, 0x00, 0x0F, 0xC0, 0x00, 0x0F, 0xC0, 0x00, 0x0F, 0xC0, 0x00, 0x0C, 0xC0, 0x00, 0x0C, 0xC0, 0x00, 0x0C, 0x7F, 0xFF, 0xF8, 0x3F, 0xFF, 0xF0},
  {0x3F, 0xFF, 0xF0, 0x7F, 0xFF, 0xF8, 0xC0, 0x00, 0x0C, 0xDE, 0x00, 0x0C, 0xDE, 0x00, 0x0C, 0xDE, 0x00, 0x0C, 0xDE, 0x00, 0x0F, 0xDE, 0x00, 0x0F, 0xDE, 0x00, 0x0F, 0xDE, 0x00, 0x0F, 0xDE, 0x00, 0x0F, 0xDE, 0x00, 0x0C, 0xDE, 0x00, 0x0C, 0xC0, 0x00, 0x0C, 0x7F, 0xFF, 0xF8, 0x3F, 0xFF, 0xF0},
  {0x3F, 0xFF, 0xF0, 0x7F, 0xFF, 0xF8, 0xC0, 0x00, 0x0C, 0xDF, 0xE0, 0x0C, 0xDF, 0xE0, 0x0C, 0xDF, 0xE0, 0x0C, 0xDF, 0xE0, 0x0F, 0xDF, 0xE0, 0x0F, 0xDF, 0xE0, 0x0F, 0xDF, 0xE0, 0x0F, 0xDF, 0xE0, 0x0F, 0xDF, 0xE0, 0x0C, 0xDF, 0xE0, 0x0C, 0xC0, 0x00, 0x0C, 0x7F, 0xFF, 0xF8, 0x3F, 0xFF, 0xF0},
  {0x3F, 0xFF, 0xF0, 0x7F, 0xFF, 0xF8, 0xC0, 0x00, 0x0C, 0xDF, 0xFE, 0x0C, 0xDF, 0xFE, 0x0C, 0xDF, 0xFE, 0x0C, 0xDF, 0xFE, 0x0F, 0xDF, 0xFE, 0x0F, 0xDF, 0xFE, 0x0F, 0xDF, 0xFE, 0x0F, 0xDF, 0xFE, 0x0F, 0xDF, 0xFE, 0x0C, 0xDF, 0xFE, 0x0C, 0xC0, 0x00, 0x0C, 0x7F, 0xFF, 0xF8, 0x3F, 0xFF, 0xF0},
  {0x3F, 0xFF, 0xF0, 0x7F, 0xFF, 0xF8, 0xC0, 0x00, 0x0C, 0xDF, 0xFF, 0xEC, 0xDF, 0xFF, 0xEC, 0xDF, 0xFF, 0xEC, 0xDF, 0xFF, 0xEF, 0xDF, 0xFF, 0xEF, 0xDF, 0xFF, 0xEF, 0xDF, 0xFF, 0xEF, 0xDF, 0xFF, 0xEF, 0xDF, 0xFF, 0xEC, 0xDF, 0xFF, 0xEC, 0xC0, 0x00, 0x0C, 0x7F, 0xFF, 0xF8, 0x3F, 0xFF, 0xF0},
  {0x3F, 0xFF, 0xF0, 0x7F, 0xFF, 0xF8, 0xC0, 0x00, 0x0C, 0xC0, 0x78, 0x0C, 0xC0, 0xFC, 0x0C, 0xC0, 0xFC, 0x0C, 0xC0, 0xFC, 0x0F, 0xC0, 0xFC, 0x0F, 0xC0, 0x78, 0x0F, 0xC0, 0x30, 0x0F, 0xC0, 0x00, 0x0F, 0xC0, 0x30, 0x0C, 0xC0, 0x30, 0x0C, 0xC0, 0x00, 0x0C, 0x7F, 0xFF, 0xF8, 0x3F, 0xFF, 0xF0},
  {0x7f, 0xfc, 0xff, 0xfe, 0xc0, 0x06, 0xd0, 0x07, 0xd0, 0x07, 0xd0, 0x07, 0xd0, 0x07, 0xc0, 0x06, 0xff, 0xfe, 0x7f, 0xfc}
};
//----------------------------------------------------------------------------------------------

//---------------------- MUESTREO DE PANTALLAS -------------------------------------------------
//---------------------- MODO OPERACION --------------------------------------------------------
/*
  Modo Operacion tiene distintos msj de muestra, a continuación los posibles mensajes

  1: HORA                   -> Pantalla predeterminada, muestra valores y la hora
  2: MOVIMIENTO_CORRECTO    -> Muestra valores y msj "MOVIMIENTO CORRECTO"
  3: MOVIMIENTO_INCORRECTO  -> Muestra valores y msj "MOVIMIENTO INCORRECTO"
  4: BATERIA_BAJA           -> Muestra valores y msj "BATERIA BAJA"
  5: DESCONFIGURADO         -> Muestra valores y msj "EQUIPO DESCONFIGURADO"

*/
void modo_operacion(int msj) {

  char buf[9];                                               // Char auxiliar

  display.clearDisplay();                                     // Limpiamos gráficos actuales en la OLED
  display.setTextSize(1);                                     // Establecemos el tamaño del texto a 1
  display.setTextColor(WHITE);                                // Y de color blanco/azul

  display.drawBitmap(10, y, stepsV3, 10, 8, 1);               // Logo stepsV3.
  display.drawBitmap(43, y, check, 10, 8, 1);                 // Logo check.
  display.drawBitmap(64, y, alert, 10, 8, 1);                 // Logo alert.
  display.drawBitmap(87, y, acceleration, 10, 8, 1);          // Logo acceleration.
  display.drawBitmap(115, y, velocityV2, 10, 8, 1);           // Logo velocityV2.

  display.setCursor(0, y + 9);                                // Establecemos la posición X,Y.
  sprintf(buf, "%05d", pasos);                                // Agregamos 0's iniciales. Conteo de pasos
  display.println(F(buf));                                    // Mostramos en pantalla.

  display.setCursor(38, y + 9);
  sprintf(buf, "%03d", pc);                                   // Operaciones Correcta
  display.println(F(buf));

  display.setCursor(64, y + 9);
  sprintf(buf, "%02d", par);                                  // Operacion Alto Riesgo
  display.println(F(buf));

  display.setCursor(87, y + 9);
  sprintf(buf, "%02d", hi);                                   // Movimientos de Alta Intensidad
  display.println(F(buf));

  display.setCursor(115, y + 9);
  sprintf(buf, "%02d", hv);                                   // Operaciones Alta Velocidad
  display.println(F(buf));

  switch (msj) {
    case 1: // HORA
      display.setTextSize(2);
      display.setCursor(20, y + 18);

      sprintf(buf, "%02d:%02d %s", hour, minute, amPm);       // Formato a la hora EJ: 12:00 AM->  where char *amPm = "AM"; IT WORKS

      //display.println(F("12:25 pm"));
      display.println(F(buf));

      break;

    case 2: // MOVIMIENTO_CORRECTO
      display.setTextSize(1);
      display.setCursor(36, y + 18);
      display.println(F("MOVIMIENTO"));
      display.setCursor(36, y + 27);
      display.println(F(" CORRECTO "));
      break;

    case 3: // MOVIMIENTO_INCORRECTO
      display.setTextSize(1);
      display.setCursor(36, y + 18);
      display.println(F("MOVIMIENTO"));
      display.setCursor(36, y + 27);
      display.println(F("INCORRECTO"));
      break;

    case 4: // BATERIA_BAJA
      display.setTextSize(1);
      display.setCursor(20 , y + 23);
      display.println(F("BATERIA BAJA"));
      display.drawBitmap(xx + 38, y + 21, batt_frames[6], 16, 10, 1);    // Modo operacion -> batería baja
      break;

    case 5: // DESCONFIGURADO
      display.setTextSize(1);
      display.setCursor(2 , y + 23);
      display.println(F("EQUIPO DESCONFIGURADO"));
      break;
  }
  display.display();
}
//----------------------------------------------------------------------------------------------

//---------------------- MODO CARGA DE BATERIA -------------------------------------------------
void modo_carga_bateria(int porc_carga) {

  char buff[10];

  int frames_batt_sel;                                        // Aquí guardaremos el indice donde está guardado el sprite de la bateria a mostrar

  if (porc_carga <= 25) {                                     // Dependiendo de la cantidad de carga en la batería se le asignará una animación diferente
    frames_batt_sel = 1;                                      // batt_25
  }
  else if (porc_carga <= 50) {
    frames_batt_sel = 2;                                      // batt_50
  }
  else if (porc_carga <= 75) {
    frames_batt_sel = 3;                                      // batt_75
  }
  else if (porc_carga <= 100) {
    frames_batt_sel = 4;                                      // batt_100
  }

  for (int i = 0; i < ANIMATION_NUMBER; i++) {                // i -> para cambiar el frame en la animacion

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(2, y);
    display.print(F("    CARGANDO "));

    sprintf(buff, "%01d", porc_carga);                          // Llevamos el entero del porcentaje a un array auxiliar buff.
    strcat(buff, "%");                                          // Concatenamos el simbolo de % al array auxiliar buff.

    display.print(F(buff));                                     // Mostramos porcentaje de carga.

    if (porc_carga == 100) {                                    // Si el porcentaje es igual a 100, no mostramos animación.
      display.drawBitmap(xx - 4, yy, batt_frames[4], 24, 16, 1);                    // Seleccionamos el frame de bateria completa.
    } else {
      if (i == 1) {
        display.drawBitmap(xx - 4, yy, batt_frames[frames_batt_sel], 24, 16, 1);    // Seleccionamos el frame correspondiente al porcentaje de carga actual.
      } else {
        display.drawBitmap(xx - 4, yy, batt_frames[0], 24, 16, 1);                  // Seleccionamos el frame de batería vacio.
      }
    }

    display.display();                                          // Muestreo de la pantalla.
    curr_time = millis();
    while (millis() - curr_time <= ANIMATION_TIME);             // Delay sin bloquear interrupciones.
  }
}
//----------------------------------------------------------------------------------------------

//---------------------- MODO ACTIVACION -------------------------------------------------------
/*
  Modo Activacion tiene distintas pantallas, a continuación las posibles pantallas

  1: BUFFER_LLENO         -> Muestra pantalla indicando de descargar la data
  2: ACTIVANDO_IMU        -> Muestra pantalla indicando que se está activando el equipo
  3: ACTIVANDO_IMU_FALLA  -> Muestra pantalla indicando del status del IMU es incorrecto y debera seguir intentando/reiniciar el equipo
  4: BATERIA_BAJA         -> Muestra pantalla indicando que la bateria esta baja
*/
void modo_activacion(int msj) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  for (int i = 0; i < ANIMATION_NUMBER; i++) {              // i -> para cambiar el frame en la animacion
    switch (msj) {
      case 1:                                               // BUFFER LLENO - BLINK
        if (i == 1) {
          display.drawBitmap(xx, y, error_imu, 16, 16, 1);
        }
        display.setCursor(2, y + 23);
        display.println(F("  DESCARGAR LA DATA  "));
        break;

      case 2:                                               // ACTIVANDO IMU - SPRITE ANIMATION LOADING
        for (int i = 0; i < 8; i++) {                       // 8 frames per rotation
          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(WHITE);

          display.setCursor(2, y);
          display.println(F("  ACTIVANDO EQUIPO   "));
          display.drawBitmap(xx, yy, frames[i], 16, 16, 1);
          display.display();
          delay(5);
        }
        break;

      case 3:                                               // ACTIVANDO IMU - ERROR WHILE CONNECTING BLINK
        display.setCursor(2, y);
        display.println(F("  ACTIVANDO EQUIPO   "));
        if (i == 1) {
          display.drawBitmap(xx, yy, error_imu, 16, 16, 1);
        }
        break;

      case 4:                                               // CARGAR BATERIA - BLINK
        if (i == 1) {
          display.drawBitmap(xx - 4, y, batt_frames[BATT_EMPTY], 24, 16, 1);
          display.drawBitmap(xx - 4, y, batt_frames[BATT_ZERO], 24, 16, 1);
        }
        display.setCursor(2, y + 23);
        display.println(F("    CARGAR BATERIA   "));
        break;
    }

    if (msj != 2) {                                         // Si MSJ es ACTIVANDO_IMU (animacion de tuerca) salta este delay para no detener la fluidez de la animacion
      display.display();
      curr_time = millis();
      while (millis() - curr_time <= ANIMATION_TIME);       // Delay sin bloquear interrupciones
    }
  }
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();

  // Draw bitmap orbittas logo on the screen
  //display.drawBitmap(0, 0, ORBITTAS_BMP_LOGO, 128, 64, 1);
  //display.display();

  digitalWrite(ledPin, HIGH);
  delay(300);
  digitalWrite(ledPin, LOW);
}

void loop() {
  int c_t, pcarga;
  int tiempo_muestreo = 4000;

  //-------- VALORES ALEATORIOS PARA DEMOSTRACION --------
  hour = random(1, 12);
  minute = random(0, 59);

  pcarga = random(0, 100);
  
  pasos = random(0, 99999);
  pc = random(0, 999);
  par = random(0, 99);
  hi = random(0, 99);
  hv = random(0, 99);
  //------------------------------------------------------
  
  c_t = millis();
  do {
    modo_operacion(HORA);                       // Muestreo modo operacion pantalla por defecto
  } while (millis() - c_t <= tiempo_muestreo);

  c_t = millis();
  do {
    modo_operacion(MOVIMIENTO_CORRECTO);        // Muestreo modo operacion pantalla movimiento correcto
  } while (millis() - c_t <= tiempo_muestreo);

  c_t = millis();
  do {
    modo_operacion(MOVIMIENTO_INCORRECTO);      // Muestreo modo operacion pantalla movimiento incorrecto
  } while (millis() - c_t <= tiempo_muestreo);

  c_t = millis();
  do {
    modo_operacion(BATERIA_BAJA);               // Muestreo modo operacion pantalla bateria baja
  } while (millis() - c_t <= tiempo_muestreo);

  c_t = millis();
  do {
    modo_operacion(DESCONFIGURADO);             // Muestreo modo operacion pantalla desconfigurado
  } while (millis() - c_t <= tiempo_muestreo);


  c_t = millis();
  do {
    modo_carga_bateria(pcarga);                 // Muestreo modo carga bateria pantalla porcentaje de carga
  } while (millis() - c_t <= tiempo_muestreo);

  
  c_t = millis();
  do {
    modo_activacion(ACTIVANDO_IMU);             // Muestreo modo activacion pantalla activando equipo
  } while (millis() - c_t <= tiempo_muestreo);

  c_t = millis();
  do {
    modo_activacion(ACTIVANDO_IMU_FALLA);       // Muestreo modo activacion pantalla activando equipo con falla
  } while (millis() - c_t <= tiempo_muestreo);

  c_t = millis();
  do {
    modo_activacion(BUFFER_LLENO);              // Muestreo modo activacion pantalla buffer de datos lleno
  } while (millis() - c_t <= tiempo_muestreo);

  c_t = millis();
  do {
    modo_activacion(BATERIA_BAJA);              // Muestreo modo activacion pantalla cargar bateria
  } while (millis() - c_t <= tiempo_muestreo);
}
