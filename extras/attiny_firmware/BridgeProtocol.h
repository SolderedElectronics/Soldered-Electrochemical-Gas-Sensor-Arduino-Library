#ifndef BRIDGE_PROTOCOL_H
#define BRIDGE_PROTOCOL_H

// Protocol between the host MCU and the ATtiny404 I2C bridge.
// This header exists in two copies - here, and mirrored as #defines inside
// src/Electrochemical-Gas-Sensor-SOLDERED.h - since the firmware and the host
// library are compiled by two separate toolchains and can't share one file.
// Keep both copies in sync manually whenever the protocol changes.

// Command bytes (host -> bridge, write phase)
#define CMD_PING          0x01 // no payload; response data = BRIDGE_PING_MAGIC
#define CMD_CONFIGURE_ADC 0x02 // payload: [gainCode, dataRate]
#define CMD_CONFIGURE_LMP 0x03 // payload: [tiacn, refcn, modecn]
#define CMD_TRIGGER_ADC   0x04 // no payload; response data = raw int16_t ADC reading, big-endian

// Response status byte - always the first byte of every 3-byte read
#define BRIDGE_STATUS_BUSY  0x00 // command still processing, host should retry
#define BRIDGE_STATUS_OK    0x01
#define BRIDGE_STATUS_ERROR 0x02
#define BRIDGE_STATUS_NONE  0xFF // no command received yet

#define BRIDGE_RESPONSE_LEN 3

#define BRIDGE_PING_MAGIC 0xC0FE

// Address range reserved for ATtiny bridge boards (easyC jumper scheme, see easyC.h).
// Legacy direct-wired boards use the ADS1115's own address range (0x48-0x4B) instead -
// the host library dispatches transport mode based on which range the address falls in.
#define BRIDGE_ADDR_MIN 0x30
#define BRIDGE_ADDR_MAX 0x37

#endif
