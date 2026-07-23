/**
 **************************************************
 *
 * @file        Electrochemical-Gas-Sensor-SOLDERED.h
 * @brief       Header file for sensor specific code.
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     Robert @ soldered.com
 ***************************************************/

#ifndef __ELECTROCHEMICAL_GAS_SENSOR_SOLDERED__
#define __ELECTROCHEMICAL_GAS_SENSOR_SOLDERED__

// This define will cause debug messages to print in our library
// Useful when calibrating the sensor
//#define ELECTROCHEMICAL_SENSOR_DEBUG

#include "Arduino.h"
#include "libs/ADS1X15/ADS1X15.h"
#include "libs/LMP91000/LMP91000.h"
#include "sensorConfigData.h"

#define DEFAULT_LMP_ADDR 0x48
#define DEFAULT_ADC_ADDR 0x49
#define REF_VOLTAGE      2.5F

// --- ATtiny bridge protocol constants ---
// NOTE: keep these in sync with extras/attiny_firmware/BridgeProtocol.h - the two
// are compiled by separate toolchains (host sketch vs. ATtiny404 firmware) and
// can't share one physical header file.
#define CMD_PING          0x01
#define CMD_CONFIGURE_ADC 0x02
#define CMD_CONFIGURE_LMP 0x03
#define CMD_TRIGGER_ADC   0x04

#define BRIDGE_STATUS_BUSY  0x00
#define BRIDGE_STATUS_OK    0x01
#define BRIDGE_STATUS_ERROR 0x02
#define BRIDGE_STATUS_NONE  0xFF

// Address range reserved for ATtiny bridge boards (easyC jumper scheme). Legacy
// direct-wired boards use the ADS1115's own address range (0x48-0x4B) instead -
// begin() dispatches transport mode based on which range _adcAddr falls in.
#define BRIDGE_ADDR_MIN   0x30
#define BRIDGE_ADDR_MAX   0x37
#define BRIDGE_TIMEOUT_MS 500

// LEGACY_DIRECT: direct-wired board, talk to LMP91000/ADS1115 over Wire as before.
// BRIDGE: new ATtiny404 board revision, talk to the ATtiny's command protocol instead.
enum class TransportMode
{
    LEGACY_DIRECT,
    BRIDGE
};

class ElectrochemicalGasSensor
{
  public:
    // _adcAddr meaning depends on the board revision:
    //  - legacy direct-wired boards: the ADS1115's own I2C address (0x48-0x4B)
    //  - ATtiny bridge boards: the bridge's easyC jumper address (0x30-0x37)
    // begin() auto-detects which one applies from the address range.
    ElectrochemicalGasSensor(sensorType _t, uint8_t _adcAddr = DEFAULT_ADC_ADDR, int _configPin = -1);
    bool begin();
    bool configureLMP();
    double getVoltage();
    double getPPM();
    double getPPB();
    double getAveragedPPM(uint8_t _numMeasurements = 5, uint8_t _secondsDelay = 2);
    double getAveragedPPB(uint8_t _numMeasurements = 5, uint8_t _secondsDelay = 2);
    void setCustomTiaGain(float _tiaGain);
    void setCustomZeroCalibration(double calibration);

  private:
    LMP91000 *lmp;
    ADS1115 *ads;
    uint8_t adcAddr;
    int configPin; // unused when mode == TransportMode::BRIDGE - LMPEN is grounded on that board
    sensorType type;
    TransportMode mode;
    float tiaGainInKOHms;
    float internalZeroPercent;
    float getTiaGain();
    float getInternalZeroPercent();

    // ATtiny bridge transport helpers - only used when mode == TransportMode::BRIDGE
    bool bridgeTransaction(uint8_t cmd, const uint8_t *payload, uint8_t payloadLen, uint8_t *resultHigh,
                            uint8_t *resultLow);
    bool pingBridge();
    bool sendConfigureAdc(uint8_t gain, uint8_t dataRate);
    bool sendConfigureLmp(uint8_t tiacn, uint8_t refcn, uint8_t modecn);
    bool triggerAndReadAdc(int16_t &rawOut);
};

#endif
