/**
 **************************************************
 *
 * @file        attiny_firmware.ino
 * @brief       ATtiny404 I2C bridge firmware for the Electrochemical Gas Sensor board.
 *
 *              Presents a configurable I2C address to the host over the qwiic/easyC
 *              connector (SCL1/SDA1, wired to PB0/PB1 - megaTinyCore's default hardware
 *              Wire pins) and relays LMP91000 configuration writes + ADS1x15 raw ADC reads
 *              to/from the onboard chips (fixed addresses 0x48/0x49, on the SCL2/SDA2 net,
 *              wired to PA2/PA1) over a software bit-banged local bus - the ATtiny404 has
 *              only one hardware TWI peripheral, and that one is reserved for the
 *              host-facing slave role since slave-mode timing is far less forgiving to
 *              bit-bang than master.
 *
 *              LMPEN (ATtiny PA6, net "LMPEN" to the LMP91000's MENB pin) is manually
 *              grounded on this board revision instead - this firmware never drives it.
 *
 * @authors     @ soldered.com
 ***************************************************/

#include "easyC.h"
#include "BridgeProtocol.h"
#include "SoftI2C.h"
#include "LmpBridge.h"
#include "AdsBridge.h"
#include <Wire.h>

// Local bus pins to the onboard LMP91000 + ADS chip - per schematic, the SCL2/SDA2 net
// is wired to PA2/PA1 (megaTinyCore's TWI0 "pinswap" pins, used here as plain bit-banged
// GPIO rather than via the hardware peripheral - see file header). PA1/PA2 are already
// defined as macros in easyC.h.
#define LOCAL_SDA_PIN PA1
#define LOCAL_SCL_PIN PA2

int addr = DEFAULT_ADDRESS;

SoftI2C localBus(LOCAL_SDA_PIN, LOCAL_SCL_PIN);
LmpBridge lmp(&localBus);
AdsBridge ads(&localBus);

// Command buffer: [cmd, payload0, payload1, payload2] - 4 bytes covers the
// largest command (CMD_CONFIGURE_LMP: tiacn, refcn, modecn).
volatile uint8_t cmdBuffer[4];
volatile bool cmdPending = false;

// Always exactly 3 bytes: [status, resultHigh, resultLow]. Read by onRequest(),
// written only from loop() - never from an ISR - so a transaction is never
// caught half-updated as long as the host follows the one-command-at-a-time protocol.
uint8_t responseFrame[BRIDGE_RESPONSE_LEN] = {BRIDGE_STATUS_NONE, 0, 0};

void setup()
{
    initDefault();
    addr = getI2CAddress();

    localBus.begin();

    Wire.begin(addr);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
}

void loop()
{
    if (!cmdPending)
        return;

    uint8_t cmd = cmdBuffer[0];
    uint8_t p0 = cmdBuffer[1];
    uint8_t p1 = cmdBuffer[2];
    uint8_t p2 = cmdBuffer[3];

    switch (cmd)
    {
    case CMD_PING:
        responseFrame[0] = BRIDGE_STATUS_OK;
        responseFrame[1] = (uint8_t)(BRIDGE_PING_MAGIC >> 8);
        responseFrame[2] = (uint8_t)(BRIDGE_PING_MAGIC & 0xFF);
        break;

    case CMD_CONFIGURE_ADC:
        ads.setGain(p0);
        ads.setDataRate(p1);
        responseFrame[0] = BRIDGE_STATUS_OK;
        responseFrame[1] = 0;
        responseFrame[2] = 0;
        break;

    case CMD_CONFIGURE_LMP:
    {
        bool ok = lmp.configure(p0, p1, p2);
        responseFrame[0] = ok ? BRIDGE_STATUS_OK : BRIDGE_STATUS_ERROR;
        responseFrame[1] = 0;
        responseFrame[2] = 0;
        break;
    }

    case CMD_TRIGGER_ADC:
    {
        ads.triggerConversion();
        while (ads.isBusy())
        {
            // Local bus is self-paced - blocking loop() here is fine, it doesn't
            // stall the host bus (onReceive/onRequest ISRs still fire independently).
        }
        int16_t raw = ads.readValue();
        responseFrame[0] = BRIDGE_STATUS_OK;
        responseFrame[1] = (uint8_t)((raw >> 8) & 0xFF);
        responseFrame[2] = (uint8_t)(raw & 0xFF);
        break;
    }

    default:
        responseFrame[0] = BRIDGE_STATUS_ERROR;
        break;
    }

    cmdPending = false;
}

void receiveEvent(int howMany)
{
    uint8_t i = 0;
    while (Wire.available() && i < sizeof(cmdBuffer))
        cmdBuffer[i++] = Wire.read();
    while (Wire.available())
        Wire.read(); // discard anything beyond the buffer

    if (i > 0)
    {
        responseFrame[0] = BRIDGE_STATUS_BUSY; // host will poll until loop() updates this
        cmdPending = true;
    }
}

void requestEvent()
{
    Wire.write((const uint8_t *)responseFrame, BRIDGE_RESPONSE_LEN);
}
