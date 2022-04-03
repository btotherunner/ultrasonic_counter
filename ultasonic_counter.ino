int triggerdistance = 20;     // Distanz in CM Trigger für den Counter

#define PIN_TRIGGER 14   // PIN D5
#define PIN_ECHO    12   // PIN D6

const int SENSOR_MAX_RANGE = 300; // in cm
unsigned long duration;
unsigned int distance;

#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
u8g2_uint_t offset;
u8g2_uint_t width;

// Zähler
int counter = 0;
int resetdistance = 0;
int hold = 0;

void setup()
{
  Serial.begin(9600);

  // INIT SSD1306
  u8g2.begin();
  u8g2.setFontMode(0);

  // INIT HC-SR04
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

}

void loop()
{
  digitalWrite(PIN_TRIGGER, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIGGER, HIGH);
  delayMicroseconds(10);

  duration = pulseIn(PIN_ECHO, HIGH);
  distance = duration / 58;

  if (distance > SENSOR_MAX_RANGE || distance <= 0) {
    Serial.println("Out of sensor range!");
  } else {
    Serial.println("Distance to object: " + String(distance) + " cm");
  }

  if (resetdistance == 0) {
    if (distance < triggerdistance) {
      counter = counter + 1;
      resetdistance = 1;
      hold = 1;
      showdisplay();

      delay(5000);          // Kleine Pause von 5 Sekunden damit eine Palette nicht 2x gezählt wird...
    }
  }
  if (distance > triggerdistance) {
    resetdistance = 0;
    hold = 0;
    showdisplay();
  }

  delay(1000);
}

void showdisplay() {
  // DISPLAY Ausgabe
  u8g2.firstPage();
  do
  {
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_logisoso54_tf);
    u8g2.setCursor(0, 60);
    u8g2.print(counter);

    if (hold == 1) {
      u8g2.setFont(u8g2_font_helvB08_te);
      u8g2.setCursor(90, 50);
      u8g2.print("HOLD");
    }

    u8g2.setFont(u8g2_font_helvB08_te);
    u8g2.setCursor(90, 60);
    u8g2.print(distance);
    u8g2.print(" cm");
  }
  while (u8g2.nextPage());

}
