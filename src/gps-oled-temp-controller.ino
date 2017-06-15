#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#include <DHT.h>
#include <Adafruit_MLX90614.h>
#include "Switch.h"

#define PIN_RESET 255  // MicroOLED
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D - OLED Display

#define DHTPIN D4
#define DHTTYPE DHT22

const byte buttonPin = D3; // Button Shield
const int ledPin = BUILTIN_LED; // Wemos D1 Built-in LED

int i =0;

/////////////////////////////////////////////////////////////
// Hardware Initialization
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
DHT dht(DHTPIN, DHTTYPE);
MicroOLED oled(PIN_RESET, DC_JUMPER);
Switch button = Switch(buttonPin);
/////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mlx.begin();
  dht.begin();
  // oled.begin();     // Initialize the OLED
  
  // oled.clear(PAGE); // Clear the display's internal memory
  // oled.clear(ALL);  // Clear the library's display buffer
  // oled.display();   // Display what's in the buffer (splashscreen)
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  button.poll();
  // textExamples();  // Finally the text example
  // printInitialScreen();  
  // scanI2CDevices();

  if(button.switched()) {
    Serial.print("SWITHCED");
    digitalWrite(ledPin, HIGH);
  }
  if (button.pushed()) {
    Serial.print("pushed ");
    Serial.print(++i);
    Serial.print(" ");
  }

  if (button.longPress()) {
    Serial.print("longPress ");
    digitalWrite(ledPin, HIGH);
    getTemp();
    getTempMLX();
   // printInitialScreen();
  }

  if (button.doubleClick()) {
    Serial.print("doubleClick ");
    // getTemp();
  }

  if (button.released()) {
    Serial.print("released\n");
    digitalWrite(ledPin, LOW);
  }
}

void getTemp() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
}

void scanI2CDevices() {
  byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error == 4) {
      Serial.print("Unknow error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
 
  delay(3000);           // wait 5 seconds for next scan
}

void getTempMLX() {
  Serial.print("MLX Ambient = "); 
  Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tMLX Object = "); 
  Serial.print(mlx.readObjectTempC()); 
  Serial.println("*C");
  Serial.print("MLX Ambient = "); 
  Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tMLX Object = "); 
  Serial.print(mlx.readObjectTempF()); 
  Serial.println("*F");
 
  Serial.println();
  delay(2000);
}

void printInitialScreen()
{
  String title = "PPSmart";

  int middleX = oled.getLCDWidth() / 2;
  int middleY = oled.getLCDHeight() / 2;
  
  oled.clear(PAGE);
  oled.setFontType(1);
  // oled.setCursor(middleX - (oled.getFontWidth() * (title.length() / 2)), middleY - (oled.getFontWidth() / 2));
  oled.setCursor(0, middleY - (oled.getFontWidth() / 2));
  oled.print(title);
  oled.display();


}