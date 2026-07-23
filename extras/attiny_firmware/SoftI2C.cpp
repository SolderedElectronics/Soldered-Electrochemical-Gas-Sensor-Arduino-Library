#include "SoftI2C.h"

// Conservative bit period - keeps this comfortably under 100kHz once call
// overhead is included. Bench-verify against the real LMP91000/ADS timing.
#define I2C_DELAY_US 5

SoftI2C::SoftI2C(uint8_t sdaPin, uint8_t sclPin) : _sda(sdaPin), _scl(sclPin)
{
}

void SoftI2C::begin()
{
    sdaHigh();
    sclHigh();
}

// Open-drain emulation: "high" releases the line to its pull-up, "low" drives it.
void SoftI2C::sdaHigh()
{
    pinMode(_sda, INPUT_PULLUP);
}

void SoftI2C::sdaLow()
{
    pinMode(_sda, OUTPUT);
    digitalWrite(_sda, LOW);
}

void SoftI2C::sclHigh()
{
    pinMode(_scl, INPUT_PULLUP);
    while (digitalRead(_scl) == LOW)
    {
        // clock stretching: wait for the slave chip to release SCL
    }
}

void SoftI2C::sclLow()
{
    pinMode(_scl, OUTPUT);
    digitalWrite(_scl, LOW);
}

void SoftI2C::start()
{
    sdaHigh();
    sclHigh();
    delayMicroseconds(I2C_DELAY_US);
    sdaLow();
    delayMicroseconds(I2C_DELAY_US);
    sclLow();
}

void SoftI2C::stop()
{
    sdaLow();
    delayMicroseconds(I2C_DELAY_US);
    sclHigh();
    delayMicroseconds(I2C_DELAY_US);
    sdaHigh();
    delayMicroseconds(I2C_DELAY_US);
}

bool SoftI2C::writeByte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & 0x80)
            sdaHigh();
        else
            sdaLow();
        delayMicroseconds(I2C_DELAY_US);
        sclHigh();
        delayMicroseconds(I2C_DELAY_US);
        sclLow();
        data <<= 1;
    }

    // 9th clock: read the ACK bit (slave pulls SDA low to ACK)
    sdaHigh();
    delayMicroseconds(I2C_DELAY_US);
    sclHigh();
    bool ack = (digitalRead(_sda) == LOW);
    delayMicroseconds(I2C_DELAY_US);
    sclLow();
    return ack;
}

uint8_t SoftI2C::readByte(bool ack)
{
    uint8_t data = 0;
    sdaHigh(); // release SDA so the slave can drive it

    for (uint8_t i = 0; i < 8; i++)
    {
        sclHigh();
        data <<= 1;
        if (digitalRead(_sda))
            data |= 0x01;
        delayMicroseconds(I2C_DELAY_US);
        sclLow();
        delayMicroseconds(I2C_DELAY_US);
    }

    // 9th clock: drive the ACK/NACK bit ourselves as master-receiver
    if (ack)
        sdaLow();
    else
        sdaHigh();
    delayMicroseconds(I2C_DELAY_US);
    sclHigh();
    delayMicroseconds(I2C_DELAY_US);
    sclLow();
    sdaHigh();

    return data;
}

bool SoftI2C::beginTransmission(uint8_t address)
{
    start();
    return writeByte((address << 1) | 0x00); // R/W = write
}

bool SoftI2C::write(uint8_t data)
{
    return writeByte(data);
}

void SoftI2C::endTransmission()
{
    stop();
}

bool SoftI2C::requestFrom(uint8_t address, uint8_t *buffer, uint8_t length)
{
    start();
    if (!writeByte((address << 1) | 0x01)) // R/W = read
    {
        stop();
        return false;
    }

    for (uint8_t i = 0; i < length; i++)
    {
        bool isLastByte = (i == length - 1);
        buffer[i] = readByte(!isLastByte); // ACK every byte except the last
    }

    stop();
    return true;
}
