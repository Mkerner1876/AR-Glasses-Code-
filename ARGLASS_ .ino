#include <Time.h>
#include <TimeLib.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;
#define  BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF
#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>
int seconds = 0;
int minutes = 0;
int hours = 0;
int F = 0;
Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);
int state;
int flag = 0;      //makes sure that the serial only prints once the state
int stateStop = 0;
#include <Servo.h>
long duration;
int distance;



void setup() {
  Serial.begin(9600);
  Serial.println(F("BME280 test"));

  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();
  display.begin();
  display.fillScreen(RED);
  delay(300);
  display.fillScreen(GREEN);
  delay(300);
  display.fillScreen(BLUE);
  delay(300);
  display.fillScreen(BLACK);
  delay(1000);
  while (!Serial) ; // Needed for Leonardo only
  pinMode(13, OUTPUT);
  setSyncProvider( requestSync);  //set function to call when sync required

  /*
     display.fillRect(10, 40, 75, 20, RED);

    display.setCursor(20, 47);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("I'm monky.");
  */



}
void loop() {
  delay(delayTime);
  other();
  digitalClockDisplay();
  void printDigits(int digits);
  blue();
  processSyncMessage();
  time_t requestSync();
  environment();
}
void other() {
  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus() != timeNotSet) {
    digitalClockDisplay();
  }
  if (timeStatus() == timeSet) {
    digitalWrite(13, HIGH); // LED on if synced
  } else {
    digitalWrite(13, LOW);  // LED off if needs refresh
  }
  delay(1000);


}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    if ( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
      setTime(pctime); // Sync Arduino clock to the time received on the serial port
    }
  }
}

time_t requestSync()
{

  Serial.print(TIME_REQUEST);
  return 0; // the time will be sent later in response to serial mesg
}


//__________________________________________________________________________
void blue() {

  if (Serial.available() > 0) {
    state = Serial.read();
    flag = 0;
  }
  if (state == 'Y') {

    display.fillRect(10, 40, 75, 20, RED);
    display.setCursor(20, 47);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print(hour());
    display.print(":");
    display.print(minute());
    display.print(":");
    display.print(second());
    Serial.println();
    if (flag == 0) {
      Serial.println("Time activated");
      flag = 1;
    }

    stateStop = 2;
  }
  if (state == 'F') {
    display.fillRect(10, 40, 75, 20, RED);

    display.setCursor(20, 47);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("I'm monky.");
    if (flag == 0) {
      Serial.println("Fun1 activated");
      flag = 1;
    }

    stateStop = 1;
  }
  if (state == 'C') {

    if (flag == 0) {
      Serial.println("Compass activated");
      flag = 1;
    }
    stateStop = 2;
  }
  if (state == 'G') {

    if (flag == 0) {
      Serial.println("GPS activated");
      flag = 1;
    }
    stateStop = 3;
  }

  if (state == 'D') {

    if (flag == 0) {
      Serial.println("Fun2 activated");
      flag = 1;
    }
    stateStop = 4;
  }

  if (state == 'S') {

    display.setCursor(20, 5);
    display.setTextColor(WHITE);
    display.setTextSize(1.5);
    display.print("Stopwatch:");

    display.fillRect(10, 40, 75, 20, BLACK);
    delay(1000);
    display.fillRect(10, 40, 75, 20, RED);
    display.setCursor(20, 47);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    seconds++;
    if (seconds >= 60) {
      seconds = 0;
      minutes++;
    }
    if (minutes >= 60) {
      seconds = 0;
      minutes = 0;
      hours++;
    }
    if (hours >= 13) {
      seconds = 0;
      minutes = 0;
      hours = 0;

    }
    display.print (hours);
    display.print("H, ");
    display.print (minutes);
    display.print("m, ");
    display.print(seconds);
    display.print("s ");
    if (flag == 0) {
      Serial.println("Stopwatch activated");
      flag = 1;
    }
    stateStop = 5;
  }

}
void environment() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");

  Serial.print("Pressure = ");

  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();

  if (state == 'I') {
    display.fillRect(10, 40, 75, 20, RED);

    display.setCursor(20, 47);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print(bme.readTemperature());
    display.print(" *C");
    if (flag == 0) {
      Serial.println("Temperature Activated");
      flag = 1;
    }

  }
  if (state == 'P') {
    display.fillRect(10, 40, 75, 20, RED);

    display.setCursor(20, 47);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print(bme.readPressure() / 100.0F);
    display.print(" hPa");
    if (flag == 0) {
      Serial.println("Pressure activated");
      flag = 1;
    }

  }

  if (state == 'A') {
    display.fillRect(10, 40, 75, 20, RED);

    display.setCursor(20, 47);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    display.print("m");
    if (flag == 0) {
      Serial.println("Altitude activated");
      flag = 1;
    }

  }
  if (state == 'H') {
    display.fillRect(10, 40, 75, 20, RED);

    display.setCursor(20, 47);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print(bme.readHumidity());
    display.print(" %");
    if (flag == 0) {
      Serial.println("Humidity activated");
      flag = 1;
    }

  }
}
//_________________________________________________________________




