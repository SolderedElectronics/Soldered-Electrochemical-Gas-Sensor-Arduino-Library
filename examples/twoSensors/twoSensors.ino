/**
 **************************************************
 *
 * @file        twoSensors.ino
 * @brief       How to configure and read from two different electrochemical sensors
 *              connected to one Dasduino.
 *
 *              To successfully run the sketch:
 *              - Connect LMPEN pins of the breakouts to pins set below
 *              - Connect the breakouts to your Dasduino board via easyC
 *              - Run the sketch and open serial monitor at 115200 baud!
 *
 *              Electrochemical Gas Sensor Breakout: solde.red/333218
 *              Dasduino Core: www.solde.red/333037
 *              Dasduino Connect: www.solde.red/333034
 *              Dasduino ConnectPlus: www.solde.red/333033
 *
 * @authors     Robert @ soldered.com
 ***************************************************/

// Include the required library
#include "Electrochemical-Gas-Sensor-SOLDERED.h"

// Set pins which connect to LMPEN on the breakouts
// To enable configuring the LMP91000 analog frontends, these have to be pulled LOW
// The library does this for you, just set the pins!
#define LMPEN1_PIN 7
#define LMPEN2_PIN 8

// Create the sensor object with the according types, addresses and pins
// See how to set the custom addresses in customAddress.ino
ElectrochemicalGasSensor sensorNO(SENSOR_NO, 0x49, LMPEN1_PIN);
ElectrochemicalGasSensor sensorCO(SENSOR_CO, 0x4A, LMPEN2_PIN);

void setup()
{
    Serial.begin(115200); // For debugging

    // Init sensorNO
    if (!sensorNO.begin())
    {
        // Can't init? Notify the user and go to infinite loop
        Serial.println("ERROR: Can't init NO sensor! Check connections!");
        while (true)
        {
            delay(100);
        }
    }
    Serial.println("NO sensor initialized successfully!");

    // Configure LMP91000 for sensorNO
    if (!sensorNO.configureLMP())
    {
        // Error? Notify the user and go to infinite loop
        Serial.println("ERROR: Can't configure LMP91000 for NO sensor!");
        while (true)
        {
            delay(100);
        }
    }
    Serial.println("NO sensor configured successfully!");

    // Init SENSOR_CO
    if (!sensorCO.begin())
    {
        // Can't init? Notify the user and go to infinite loop
        Serial.println("ERROR: Can't init CO sensor! Check connections!");
        while (true)
        {
            delay(100);
        }
    }
    Serial.println("CO sensor initialized successfully!");

    // Configure LMP91000 for SENSOR_CO
    if (!sensorCO.configureLMP())
    {
        // Error? Notify the user and go to infinite loop
        Serial.println("ERROR: Can't configure LMP91000 for CO sensor!");
        while (true)
        {
            delay(100);
        }
    }
    Serial.println("CO sensor configured successfully!");
}

void loop()
{
    // Make the readings
    double readingNO = sensorNO.getPPM();
    double readingCO = sensorCO.getPPM();
    
    // Print the readings with 10 digits of precision
    Serial.print("NO sensor reading: ");
    Serial.print(readingNO, 10);
    Serial.println("ppm");

    Serial.print("CO sensor reading: ");
    Serial.print(readingCO, 10);
    Serial.println("ppm");
    
    // Wait a bit before reading again
    delay(5000);
}