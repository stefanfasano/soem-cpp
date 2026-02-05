//
// Created by srfas on 1/24/2026.
//

#include "BufferOffsetHolder.h"

#include "Slove.h"


BufferOffsetHolder::BufferOffsetHolder(uint32_t byteOffset, uint8_t bitOffset, uint16_t bitSize) : bitSize((byteOffset << 3) + bitOffset + bitSize)
{
    increase(byteOffset, bitOffset);
}

/**
* Align the offsets on a byte boundary if bytes > 0
*
*
* @param bytes
* @param bits
*/
void BufferOffsetHolder::align(int bytes, int bits)
{
    bytes += (bits >> 3);
    if (bytes > 0 && bitOffset > 0)
    {
        byteOffset++;
        bitOffset = 0;
    }
}

/**
* Increase the offsets, wrapping bits into bytes.
*
* @param bytes
* @param bits
* @return true if space is available, false if there is not enough space for the increase
*/
bool BufferOffsetHolder::increase(int bytes, int bits)
{
    int newBitOffset = bitOffset + bits;
    int newByteOffset = byteOffset + bytes + (newBitOffset >> 3);
    newBitOffset &= 7;

    if ((newByteOffset << 3) + newBitOffset > bitSize)
    {
        return false;
    }

    bitOffset = newBitOffset;
    byteOffset = newByteOffset;
    return true;
}

const int BufferOffsetHolder::getByteOffset() const { return byteOffset; }

const int BufferOffsetHolder::getBitOffset() const { return bitOffset; }

/**
* @return the number of bits available in this buffer
*/
int BufferOffsetHolder::getAvailableBits() const { return bitSize - ((byteOffset << 3) + bitOffset); }
