// modbus_rtu_8bit.h
#ifndef MODBUS_RTU_8BIT_H
#define MODBUS_RTU_8BIT_H

#include <stdint.h>
#include <stdbool.h>

// Maximum Modbus frame size
#define MODBUS_MAX_FRAME_SIZE 256

// Structure to hold Modbus frame information
typedef struct {
    bool isValid;                // Indicates if the frame is valid
    uint8_t deviceAddress;       // Device address
    uint8_t functionCode;        // Function code
    uint8_t data[MODBUS_MAX_FRAME_SIZE - 4]; // Data array (excluding address, function code, and CRC)
    uint8_t dataLength;          // Length of the data
} ModbusFrame;

// Function prototypes
void Modbus_SendFrame(uint8_t deviceAddress, uint8_t functionCode, uint8_t *data, uint8_t dataLength);
ModbusFrame Modbus_ReceiveFrame(uint8_t *buffer, uint8_t length);
uint16_t Modbus_CalculateCRC(uint8_t *buffer, uint8_t length);

#endif // MODBUS_RTU_8BIT_H
