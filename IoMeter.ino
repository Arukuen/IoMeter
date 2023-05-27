#include <PZEM004Tv30.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Initialize LCD with address 0x27
// Use pins 21 and 22
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Iniatialize PZEM to pins 16 and 17
PZEM004Tv30 pzem(Serial2, 16, 17);

void setup() {
    // Establish serial communication with baud rate 115200
    Serial.begin(115200);
    // Initialize the LCD
    lcd.init();
    lcd.backlight();

    // Display an introduction 
    lcd.setCursor(0, 0);
    lcd.print("IoMeter Demo");
    lcd.setCursor(0, 1);
    lcd.print("by Team 4");
    delay(2000);
}

void loop() {
    // Get all the measurements
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();

    float cost = energy * 10;

    // Check if the measurements are valid
    if(isnan(voltage)) {
        Serial.println("Error reading voltage");
    } else if (isnan(current)) {
        Serial.println("Error reading current");
    } else if (isnan(power)) {
        Serial.println("Error reading power");
    } else if (isnan(energy)) {
        Serial.println("Error reading energy");
    } else if (isnan(frequency)) {
        Serial.println("Error reading frequency");
    } else if (isnan(pf)) {
        Serial.println("Error reading power factor");
    } else {
        // Print the measurements to the Serial monitor
        Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
        Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
        Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
        Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
        Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
        Serial.print("PF: ");           Serial.println(pf);
        Serial.println();

        // Display Power and Energy
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("P: ");
        lcd.print(power);
        lcd.print(" E: ");
        lcd.print(energy, 3);
        lcd.print("kWh");
        // Display the cost
        lcd.setCursor(0, 1);
        lcd.print("Cost: Php");
        lcd.print(cost);
        delay(2000);
    }
}
