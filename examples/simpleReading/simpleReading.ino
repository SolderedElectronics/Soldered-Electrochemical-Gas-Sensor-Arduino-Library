/**
 **************************************************
 *
 * @file        simpleReading.ino
 * @brief       See how to make a simple reading of PPM of the gas your sensor is meant for
 * 
 *              To successfully run the sketch:
 *              -Connect the breakout to your Dasduino board via easyC
 *              -Run the sketch and open serial monitor at 115200 baud!
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

// The different types are seen on the front of the breakout, the type which you have will be marked
// Configurations for each of the sensor types are in sensorConfigData.h in the library's 'src' folder
// Create the sensor object with the according type
ElectrochemicalGasSensor sensor(SENSOR_NO2);

// If you are using a custom I2C address, create the object like this:
// ElectrochemicalGasSensor sensor(SENSOR_SO2, 0x4A);
// For more info, see customAddress.ino

void setup()
{
    Serial.begin(115200); // For debugging

    // Init the sensor
    if (!sensor.begin())
    {
        // Can't init? Notify the user and go to infinite loop
        Serial.println("ERROR: Can't init the sensor! Check connections!");
        while (true)
        {
            delay(100);
        }
    }

    Serial.println("Sensor initialized successfully!");
}

void loop()
{
    // Make the reading
    double reading = sensor.getPPM();
    
    // Print the reading with 10 digits of precision
    Serial.print("Sensor reading: ");
    Serial.print(reading, 10);
    Serial.println("ppm");
    
    // Wait a bit before reading again
    delay(5000);
}