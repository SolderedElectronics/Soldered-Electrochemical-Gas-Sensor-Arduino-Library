/**
 **************************************************
 *
 * @file        bridgeBoard.ino
 * @brief       See how to init a sensor on the new ATtiny404 bridge board revision,
 *              which allows daisy-chaining multiple boards on one easyC/I2C bus.
 *
 *              To successfully run the sketch:
 *              - Set the desired address using the jumpers on the back of the board
 *                (address = 0x30 + jumper offset, giving a range of 0x30-0x37)
 *              - Connect the breakout to your Dasduino board via easyC
 *              - Run the sketch and open serial monitor at 115200 baud!
 *
 *              Note: on this board revision, LMPEN is hardwired to GND, so there's
 *              no configPin argument to pass - the constructor's third argument
 *              is ignored whenever the address falls in the bridge range (0x30-0x37).
 *
 *              Electrochemical Gas Sensor Breakout: solde.red/333218
 *              Dasduino Core: www.solde.red/333037
 *              Dasduino Connect: www.solde.red/333034
 *              Dasduino ConnectPlus: www.solde.red/333033
 *
 * @authors     @ soldered.com
 ***************************************************/

// Include the required library
#include "Electrochemical-Gas-Sensor-SOLDERED.h"

// Create the sensor object, addressed via the ATtiny bridge's easyC jumpers.
// Any additional boards on the same bus just need a different jumper setting
// (0x30-0x37), same as twoSensors.ino does for legacy boards with 0x49/0x4A/0x4B.
ElectrochemicalGasSensor sensor(SENSOR_CO, 0x30);

void setup()
{
    Serial.begin(115200); // For debugging

    // Init the breakout
    if (!sensor.begin())
    {
        // Can't init? Notify the user and go to infinite loop
        Serial.println("ERROR: Can't init the sensor! Check connections and jumper address!");
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
    Serial.print("Sensor reading: ");
    Serial.print(reading, 5);
    Serial.println(" PPM");

    // Wait a bit before reading again
    delay(2500);
}
