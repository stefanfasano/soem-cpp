//
// Created by srfas on 1/24/2026.
//

#ifndef H4_SDK_BUFFEROFFSETHOLDER_H
#define H4_SDK_BUFFEROFFSETHOLDER_H
#include <cstdint>


class BufferOffsetHolder
{
    const int bitSize; //The entire bit size of a slave's output or input buffer, plus all bits before that in the master buffer
    int byteOffset{0};
    int bitOffset{0};

    public:
    BufferOffsetHolder(uint32_t byteOffset, uint8_t bitOffset, uint16_t bitSize);

    /**
    * Align the offsets on a byte boundary if bytes > 0
    *
    *
    * @param bytes
    * @param bits
    */
    void align(int bytes, int bits);

    /**
    * Increase the offsets, wrapping bits into bytes.
    *
    * @param bytes
    * @param bits
    * @return true if space is available, false if there is not enough space for the increase
    */
    bool increase(int bytes, int bits);

    const int getByteOffset() const;

    const int getBitOffset() const;

    /**
    * @return the number of bits available in this buffer
    */
    int getAvailableBits() const;
};


#endif //H4_SDK_BUFFEROFFSETHOLDER_H