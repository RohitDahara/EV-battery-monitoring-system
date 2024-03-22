#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN PA0 
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int currentSensorPin = A1;     
const int batteryVoltagePin = A2;    
const float voltageDividerRatio = 5.0 / (5.0 + 220.0); 
const int relayPin = A4; 

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();       
  lcd.backlight(); 
  pinMode(relayPin, OUTPUT); 
}

void loop() {
  float temperature = dht.readTemperature();

  int currentRaw = analogRead(currentSensorPin);

  float current_mA = (currentRaw - 590) * 0.185; 

  int batteryVoltageRaw = analogRead(batteryVoltagePin);
  float batteryVoltage = ((batteryVoltageRaw * voltageDividerRatio));


    Serial.print("  Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C,  Current: ");
    Serial.print(current_mA);
    Serial.print(" mA,  Battery Voltage: ");
    Serial.print(batteryVoltage);
    Serial.println(" V  ");

    if (current_mA > 600.0) {
    digitalWrite(relayPin, HIGH);
    Serial.print("  high current detected");
    lcd.clear();        
    lcd.setCursor(0, 0);
    lcd.print("curr not in range");
  } 
  else {
    digitalWrite(relayPin, LOW);
    Serial.print("  current is in safe range  ");
    lcd.clear();        
    lcd.setCursor(0, 0);
    lcd.print("curr in range");
  }

  delay(1000);
  
  if (temperature > 80) {
    digitalWrite(relayPin, LOW);
    Serial.print("high temperature detected  ");
    lcd.clear();        
    lcd.setCursor(0, 0);
    lcd.print("Temp not in range");
  } 
  else {
    digitalWrite(relayPin, HIGH);
    Serial.print("temperature is in safe range");
    lcd.clear();        
    lcd.setCursor(0, 0);
    lcd.print("Temp in range");
  }
  delay(1000);
  
    lcd.clear();        
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Curr: ");
    lcd.print(current_mA);
    lcd.print(" mA");

    delay(5000);

    lcd.setCursor(0, 1);
    lcd.print("Voltage: ");
    lcd.print(batteryVoltage);
    lcd.print(" V");

    delay(5000);
  
}
