/**
 **************************************************
 *
 * @file        singleSensor.ino
 * @brief       See how to init one sensor and make a simple reading of PPM of gas.
 *
 *              To successfully run the sketch:
 *              - Connect the breakout to your Dasduino board via easyC
 *              - Connect LMPEN pin to GND or a GPIO pin so the breakout can be configured
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

// Configurations for each of the sensor types are in sensorConfigData.h in the library's 'src' folder
// Create the sensor object with the according type
ElectrochemicalGasSensor sensor(SENSOR_CO);

// If you are using a custom I2C address (for multiple sensors) use:
// ElectrochemicalGasSensor sensor(SENSOR_SO2, 0x4B, 5);
// For more details on this, see customAddress.ino

void setup()
{
    Serial.begin(115200); // For debugging

    // Init the breakout
    if (!sensor.begin())
    {
        // Can't init? Notify the user and go to infinite loop
        Serial.println("ERROR: Can't init the sensor! Check connections!");
        while (true)
            delay(100);
    }

    Serial.println("Sensor initialized successfully!");
}

void loop()
{
    // Make the reading
    double reading = sensor.getPPM();

    // If PPB is more relevant for your sensor, you can use:
    // double reading = sensor.getPPB();

    // Print the reading with 5 digits of precision
    Serial.print("Sensor reading:");
    Serial.print(reading, 5);
    Serial.println(" PPM");

    // Wait a bit before reading again
    delay(2500);
}