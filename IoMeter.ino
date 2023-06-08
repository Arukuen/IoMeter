#include <PZEM004Tv30.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "RGB.h"
#include "Buzzer.h"

#define PIN_BUTTON 15
#define PIN_RED 32
#define PIN_GREEN 33
#define PIN_BLUE 27
#define PIN_BUZZER 23

// Initialize LCD with address 0x27
// Use pins 21 and 22
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Iniatialize PZEM to pins 16 and 17
PZEM004Tv30 pzem(Serial2, 17, 16);

// Create instance of WiFiClientSecure and HTTPClient
WiFiClientSecure *client = new WiFiClientSecure;
HTTPClient https; 

// Initialize RGB LED and Buzzer
RGB rgb(PIN_RED, PIN_GREEN, PIN_BLUE);
Buzzer buzzer(PIN_BUZZER);

// Network credentials
const char* ssid = "iometer";
const char* password = "iometer1234";

const char* post_url = "https://smart-meter-iot-server.onrender.com/api/device_response";

// Certificate from the web server necessary for HTTPS connection
const char* certificate = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n" \
"RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n" \
"VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n" \
"DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n" \
"ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n" \
"VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n" \
"mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n" \
"IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n" \
"mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n" \
"XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n" \
"dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n" \
"jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n" \
"BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n" \
"DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n" \
"9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n" \
"jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n" \
"Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n" \
"ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n" \
"R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n" \
"-----END CERTIFICATE-----\n";

//Simlating power and energy
// double power = 0;
// double energy = 0;

double cost = 0;
Status led_status = Low;
Status buzzer_status = Low;

// Set the JSON capacity to 2 members and declare a buffer to hold the serialized json
const size_t post_request_capacity = JSON_OBJECT_SIZE(3);
const size_t post_response_capacity = JSON_OBJECT_SIZE(6);
char json_output[128];
char json_output_pretty[128];

void setup() {
    // Establish serial communication with baud rate 115200
    Serial.begin(115200);
    pinMode(PIN_BUTTON, INPUT);

    // Initialize the LCD
    lcd.init();
    lcd.backlight();

    // Display an introduction 
    lcd.setCursor(0, 0);
    lcd.print("IoMeter Demo");
    lcd.setCursor(0, 1);
    lcd.print("by Team 4");

    // Connect to the WiFi network
    Serial.print("Attempting to connect to SSID: ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println();

    // Set the secure client with certificate
    client->setCACert(certificate);
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

    // Simulating the power and energy
    // power = 600;
    // energy += ((power / (3600/2)) / 1000);
    // float cost = energy * 10;

    // Check if the measurements are valid
    if(isnan(voltage)) {
        Serial.println("Error reading voltage");
    } 
    else if (isnan(current)) {
        Serial.println("Error reading current");
    } 
    else if (isnan(power)) {
        Serial.println("Error reading power");
    } 
    else if (isnan(energy)) {
        Serial.println("Error reading energy");
    } 
    else if (isnan(frequency)) {
        Serial.println("Error reading frequency");
    } 
    else if (isnan(pf)) {
        Serial.println("Error reading power factor");
    } 
    
    // Print the measurements to the Serial monitor
    Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
    Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
    Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
    Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
    Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
    Serial.print("PF: ");           Serial.println(pf);

    // Check if the device is still connected 
    if (WiFi.status() == WL_CONNECTED){
        // Starting the https connection
        Serial.println("[HTTPS] begin");
        if (https.begin(*client, post_url)) {
            // Add the necessary header
            https.addHeader("Content-Type", "application/json");

            // Create a StaticJsonDocument to hold memory representation of the object for POST request
            StaticJsonDocument<post_request_capacity> doc;
            JsonObject object = doc.to<JsonObject>();

            // Adding members to the JsonDocument automatically converts it to object
            object["kwh"] = energy;
            object["power"] = power;
            object["device_id"] = "A";

            // Serialize the object to produce a JSON document
            serializeJson(doc, json_output);

            // Serialize a pretty version for debugging
            serializeJsonPretty(doc, json_output_pretty);
            Serial.printf("Data to send to the server:%s\n", json_output_pretty);

            // HTTPS Post of the JSON ouput
            int http_code = https.POST(String(json_output));

            // HTTP code is negative on error
            if (http_code > 0 && http_code == HTTP_CODE_OK) {
                Serial.printf("[HTTPS] POST successful. Code: %d\nResponse:\n", http_code);
                String payload = https.getString();
                Serial.println(payload);

                // Create a StaticJsonDocument to hold memory representation of the object
                StaticJsonDocument<post_response_capacity> doc;

                // Deserialize the JSON to extract the data
                DeserializationError err = deserializeJson(doc, payload);

                // If no error occured in parsing
                if (!err) {
                    // Store the deserialized value form the JSON
                    cost = doc["cost"];
                    led_status = doc["led_status"];
                    buzzer_status = doc["buzzer_status"];

                    // Turn on the actuators accordingly
                    rgb.on(led_status);
                    buzzer.on(buzzer_status);

                    Serial.printf("Deserialized data: \nCost: %f\nLED: %d\nBuzzer: %d\n", cost, led_status,buzzer_status);

                }
                else {
                    Serial.printf("Deserializing failed with code %s\n", err.f_str());
                }

            }
            else {
                Serial.printf("[HTTPS] POST unsuccessful. Error: %s\n", https.errorToString(http_code).c_str());
            }
            https.end();
        }
    }
    else {
        Serial.println("Unable to connect to the network");
    }
    Serial.println("__________________________________________");

    // Display Power and Energy
    lcd.clear();
    lcd.home();
    lcd.print("P:");
    lcd.print(power);
    lcd.print("  E:");
    lcd.print(energy, 3);
    // Display the cost
    lcd.setCursor(0, 1);
    lcd.print("Cost: Php");
    lcd.print(cost);

    // Logic for resetting
    int button = digitalRead(PIN_BUTTON);
    if (button) {
        lcd.clear();
        lcd.home();
        lcd.print("Resetting...");
        pzem.resetEnergy();
    }
    delay(5000);
}
