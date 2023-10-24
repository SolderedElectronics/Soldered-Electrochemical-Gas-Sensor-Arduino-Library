/**
 **************************************************
 *
 * @file        twoSensors.ino
 * @brief       See how to init two sensors and make two  readings of PPM of gas.
 *
 *              To successfully run the sketch:
 *              - Set different addresses using the jumpers on the back of the breakout
 *              - Connect the breakouts to your Dasduino board via easyC
 *              - Connect LMPEN pins to a GPIO pin so the breakouts can be configured
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

// Create two different sensor objects
ElectrochemicalGasSensor sensorCO(SENSOR_CO, 0x4A, 25);
ElectrochemicalGasSensor sensorNO2(SENSOR_NO2, 0x49, 32);
// To see how the custom addresses are determined, check customAddress.ino

void setup()
{
    Serial.begin(115200); // For debugging

    // Init breakout #1
    if (!sensorCO.begin())
    {
        // Can't init? Notify the user and go to infinite loop
        Serial.println("ERROR: Can't init the sensor! Check connections!");
        while (true)
            delay(100);
    }

    Serial.println("CO sensor initialized successfully!");

    // Init breakout #2
    if (!sensorNO2.begin())
    {
        // Can't init? Notify the user and go to infinite loop
        Serial.println("ERROR: Can't init the sensor! Check connections!");
        while (true)
            delay(100);
    }

    Serial.println("NO2 sensor initialized successfully!");
}

void loop()
{
    // Make the reading of CO and print it
    double COreading = sensorCO.getPPM();

    // Print the reading with 5 digits of precision
    Serial.print("CO sensor reading: ");
    Serial.print(COreading, 5);
    Serial.println(" PPM");

    // Make the reading of NO2 and print it
    double NO2reading = sensorNO2.getPPB();

    // Print the reading with 5 digits of precision
    Serial.print("NO2 sensor reading: ");
    Serial.print(NO2reading, 5);
    Serial.println(" PPB");

    // Wait a bit before reading again
    delay(2500);
}