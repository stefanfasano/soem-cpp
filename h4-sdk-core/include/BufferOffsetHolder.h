//
// Created by srfas on 1/24/2026.
//

#ifndef H4_SDK_BUFFEROFFSETHOLDER_H
#define H4_SDK_BUFFEROFFSETHOLDER_H


class BufferOffsetHolder
{
    const int bitSize;
    int byteOffset{0};
    int bitOffset{0};

    public:
    BufferOffsetHolder(int bitSize, int byteOffset, int bitOffset) : bitSize((byteOffset << 3) + bitOffset + bitSize)
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
    void align(int bytes, int bits)
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
    bool increase(int bytes, int bits)
    {
        int newBitOffset = bitOffset + bits;
        int newByteOffset = byteOffset + bytes + (newBitOffset >> 3);
        newBitOffset &= 7;

        if((newByteOffset << 3) + newBitOffset > bitSize)
        {
            return false;
        }

        bitOffset = newBitOffset;
        byteOffset = newByteOffset;
        return true;
    }

    const int getByteOffset() const { return byteOffset; }

    const int getBitOffset() const { return bitOffset; }

    /**
    * @return the number of bits available in this buffer
    */
    int getAvailableBits() const{return bitSize - ((byteOffset << 3) + bitOffset);}
};


#endif //H4_SDK_BUFFEROFFSETHOLDER_H