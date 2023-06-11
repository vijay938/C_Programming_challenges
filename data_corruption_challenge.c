#include <stdint.h>
#include <stdbool.h>

#define MAX_PACKET_DATA_LENGTH (50)

typedef struct data_packet_t {
    uint8_t id;
    uint8_t data_length;
    uint8_t data[MAX_PACKET_DATA_LENGTH];
    uint16_t crc;
} data_packet_t;

bool isPacketCorrupted(const data_packet_t* packet) {
    // Calculate the CRC checksum of the packet data
    uint16_t calculated_crc = calculateCRC(packet->data, packet->data_length);

    // Compare the calculated CRC with the received CRC
    if (calculated_crc == packet->crc) {
        // Packet is not corrupted
        return false;
    }

    // Packet is corrupted
    return true;
}

uint16_t calculateCRC(const uint8_t* data, uint8_t length) {
    uint16_t crc = 0xFFFF; // Initial CRC value
    uint16_t polynomial = 0x1021; // CRC polynomial

    for (int i = 0; i < length; i++) {
        crc ^= (data[i] << 8); // XOR the current data byte with the CRC

        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) { // Check if the highest bit is set (MSB)
                crc = (crc << 1) ^ polynomial; // Shift CRC left and XOR with polynomial
            } else {
                crc <<= 1; // Shift CRC left
            }
        }
    }

    return crc;
}