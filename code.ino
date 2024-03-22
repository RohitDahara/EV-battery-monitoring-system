#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the pin for the DHT sensor and the type of DHT sensor being used
#define DHTPIN PA0
#define DHTTYPE DHT11

// Initialize the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Initialize the LCD with its address and dimensions
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define pins for current sensor, battery voltage sensor, and relay
const int currentSensorPin = A1;
const int batteryVoltagePin = A2;
const int relayPin = A4;

// Define the voltage divider ratio for battery voltage measurement
const float voltageDividerRatio = 5.0 / (5.0 + 220.0);

void setup() {
  // Begin serial communication
  Serial.begin(9600);
  
  // Initialize the DHT sensor
  dht.begin();
  
  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  
  // Set relay pin as an output
  pinMode(relayPin, OUTPUT);
}

void loop() {
  // Read temperature from DHT sensor
  float temperature = dht.readTemperature();

  // Read current from current sensor
  int currentRaw = analogRead(currentSensorPin);
  float current_mA = (currentRaw - 590) * 0.185;

  // Read battery voltage using voltage divider
  int batteryVoltageRaw = analogRead(batteryVoltagePin);
  float batteryVoltage = ((batteryVoltageRaw * voltageDividerRatio));

  // Print readings to serial monitor
  Serial.print("  Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C,  Current: ");
  Serial.print(current_mA);
  Serial.print(" mA,  Battery Voltage: ");
  Serial.print(batteryVoltage);
  Serial.println(" V  ");

  // Check if current exceeds safe limit
  if (current_mA > 600.0) {
    digitalWrite(relayPin, HIGH); // Activate relay to cut off power
    Serial.print("  High current detected");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Current not in range");
  } 
  else {
    digitalWrite(relayPin, LOW); // Deactivate relay
    Serial.print("  Current is in safe range  ");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Current in range");
  }

  delay(1000);

  // Check if temperature exceeds safe limit
  if (temperature > 80) {
    digitalWrite(relayPin, LOW); // Activate relay to cut off power
    Serial.print("High temperature detected  ");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperature not in range");
  } 
  else {
    digitalWrite(relayPin, HIGH); // Deactivate relay
    Serial.print("Temperature is in safe range");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperature in range");
  }
  delay(1000);

  // Display temperature and current readings on LCD
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

  // Display battery voltage reading on LCD
  lcd.setCursor(0, 1);
  lcd.print("Voltage: ");
  lcd.print(batteryVoltage);
  lcd.print(" V");

  delay(5000);
}
