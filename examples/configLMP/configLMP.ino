/**
 **************************************************
 *
 * @file        configLMP.ino
 * @brief       Configure the LMP91000 analog frontend. This only has to be done once, during production and testing.
 *              Your sensor will come preconfigured for the type of sensor you have.
 *
 *              This sketch is in case you want to reconfigure your device for a different type of sensor.
 *
 *              NOTE: For the LMP91000 to be accessible via I2C, you must connect LMPEN to GND!
 *
 *              To successfully run the sketch:
 *              -Connect LMPEN to GND
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

// Create the sensor object with the according type
ElectrochemicalGasSensor sensor(SENSOR_SO2, 0x49);

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

    // Configure LMP91000
    if (!sensor.configureLMP())
    {
        // Error? Notify the user and go to infinite loop
        Serial.println("ERROR: Can't configure LMP91000!");
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
    float reading = sensor.getPPM();
    
    // Print the reading with 10 digits of precision
    Serial.print("Sensor reading: ");
    Serial.print(reading, 10);
    Serial.println("ppm");
    
    // Wait a bit before reading again
    delay(5000);
}