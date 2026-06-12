#ifndef BUFFER_HEADER
#define BUFFER_HEADER

#include <stddef.h>
#include <stdint.h>

typedef struct BufferCDT * BufferADT;

/**
 * @brief Creates a new Buffer.
 * 
 * @return Pointer to the new Buffer, or NULL on alloc fail.
 */
BufferADT bufferNew(void);

/**
 * @brief Appends a single byte.
 * 
 * @param buffer Target buffer.
 * @param byte Byte to append.
 */
void bufferAppendByte(BufferADT buffer, uint8_t byte);

/**
 * @brief Appends length bytes from data.
 * 
 * @param buffer Target buffer.
 * @param data Source data.
 * @param length Number of bytes to append.
 */
void bufferAppendData(BufferADT buffer, const uint8_t * data, uint32_t length);

/**
 * @brief Appends a 16-bit value in big-endian.
 * 
 * @param buffer Target buffer.
 * @param value Value to append.
 */
void bufferAppendU16BE(BufferADT buffer, uint16_t value);

/**
 * @brief Appends a 32-bit value in big-endian.
 * 
 * @param buffer Target buffer.
 * @param value Value to append.
 */
void bufferAppendU32BE(BufferADT buffer, uint32_t value);

/**
 * @brief Encodes value as a MIDI variable-length quantity and appends it (1-4 .
 * 
 * @param buffer Target buffer.
 * @param value Value to encode.
 * @return Number of bytes appended.
 */
uint32_t bufferAppendVariableLength(BufferADT buffer, uint32_t value);

/**
 * @brief Returns the current number of bytes in the buffer.
 * 
 * @param buffer Buffer to query.
 * @return Current length.
 */
uint32_t bufferGetLength(const BufferADT buffer);

/**
 * @brief Returns a pointer to the internal buffer data.
 * 
 * @param buffer Buffer to query.
 * @return Pointer to data.
 */
const uint8_t * bufferGetData(const BufferADT buffer);

/**
 * @brief Frees the buffer and internal data.
 * 
 * @param buffer Buffer to free.
 */
void bufferFree(BufferADT buffer);

#endif
