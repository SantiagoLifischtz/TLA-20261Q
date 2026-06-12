#include "Buffer.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BLOCK_SIZE 64

typedef struct BufferCDT {
	uint8_t * data;
	uint32_t length;
	uint32_t capacity;
} BufferCDT;

/** PRIVATE FUNCTIONS */

/**
 * @brief Writes a 16-bit unsigned in big-endian.
 * 
 * @param buffer Output buffer, must be at least 2 bytes.
 * @param value Value to write.
 */
static void _writeU16toBE(uint8_t buffer[2], uint16_t value) {
	buffer[0] = (value >> 8) & 0xFF;
	buffer[1] = value & 0xFF;
}

/**
 * @brief Writes a 32-bit unsigned in big-endian.
 * 
 * @param buffer Output buffer, must be at least 4 bytes.
 * @param value Value to write.
 */
static void _writeU32toBE(uint8_t buffer[4], uint32_t value) {
	buffer[0] = (value >> 24) & 0xFF;
	buffer[1] = (value >> 16) & 0xFF;
	buffer[2] = (value >> 8) & 0xFF;
	buffer[3] = value & 0xFF;
}

/**
 * @brief Encodes an unsigned values as a MIDI Variable Length.
 * 
 * @param buffer Output buffer, must be at least 4 bytes.
 * @param value Value to encode.
 * @return Number of bytes written.
 */
static uint32_t _writeVariableLength(uint8_t * buffer, uint32_t value) {
	uint8_t temp[4];
	uint32_t i = 0;
	uint32_t j;
	temp[0] = value & 0x7F;

    // 7-bit groups, set continuation 8th bit in 1
	while (value > 0x7F) {
		value >>= 7;
		i++;
		temp[i] = (value & 0x7F) | 0x80;
	}

    // write in reverse order
	for (j = 0; j <= i; j++) {
		buffer[j] = temp[i - j];
	}

	return i + 1;
}

/**
 * @brief Returns the byte size needed to encode a value as a MIDI Variable Length.
 * 
 * @param value Value to measure.
 * @return Number of bytes required.
 */
static uint32_t _variableLengthSizeCount(uint32_t value) {
	uint32_t size = 1;

	while (value > 0x7F) {
		size++;
		value >>= 7;
	}

	return size;
}

/** PUBLIC FUNCTIONS */

BufferADT bufferNew(void) {
	BufferADT buffer = calloc(1, sizeof(BufferCDT));
    
	return buffer;
}

void bufferAppendByte(BufferADT buffer, uint8_t byte) {
	if (buffer->length + 1 > buffer->capacity) {
		uint32_t newCap = buffer->capacity == 0 ? BLOCK_SIZE : buffer->capacity + BLOCK_SIZE;
		uint8_t * newData = realloc(buffer->data, newCap);
		buffer->data = newData;
		buffer->capacity = newCap;
	}
    
	buffer->data[buffer->length++] = byte;
}

void bufferAppendData(BufferADT buffer, const uint8_t * data, uint32_t length) {
	uint32_t needed = buffer->length + length;

	if (needed > buffer->capacity) {
		uint32_t newCap = buffer->capacity == 0 ? BLOCK_SIZE : buffer->capacity;

		while (newCap < needed) {
			newCap += BLOCK_SIZE;
		}

		uint8_t * newData = realloc(buffer->data, newCap);
		buffer->data = newData;
		buffer->capacity = newCap;
	}
	memcpy(buffer->data + buffer->length, data, length);
	buffer->length = needed;
}

void bufferAppendU16BE(BufferADT buffer, uint16_t value) {
	uint8_t tmp[2];
	_writeU16toBE(tmp, value);
	bufferAppendData(buffer, tmp, 2);
}

void bufferAppendU32BE(BufferADT buffer, uint32_t value) {
	uint8_t tmp[4];
	_writeU32toBE(tmp, value);
	bufferAppendData(buffer, tmp, 4);
}

uint32_t bufferAppendVariableLength(BufferADT buffer, uint32_t value) {
	uint8_t tmp[4];
	uint32_t n = _writeVariableLength(tmp, value);
	bufferAppendData(buffer, tmp, n);
	return n;
}

uint32_t bufferGetLength(const BufferADT buffer) {
	return buffer->length;
}

const uint8_t * bufferGetData(const BufferADT buffer) {
	return buffer->data;
}

void bufferFree(BufferADT buffer) {
	if (buffer != NULL) {
		free(buffer->data);
		free(buffer);
	}
}
