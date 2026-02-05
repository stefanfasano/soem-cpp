//
// Created by srfas on 1/6/2026.
//

#ifndef H4_SDK_SLOVE_H
#define H4_SDK_SLOVE_H
#include <cstddef>
#include <vector>

#include "BufferOffsetHolder.h"

class Slove
{
    protected:
    const int vendorID;
    const int productCode;
    const int alias;
    const int position;
    const std::size_t ioMapSize;

    std::vector<std::byte> IOMap;

    public:
    Slove(const int &vendorID, const int &productCode, const int &alias, const int &position, const std::size_t ioMapSize) : vendorID(vendorID),
        productCode(productCode), alias(alias), position(position), ioMapSize(ioMapSize), IOMap{ioMapSize}
    {
    }

    virtual void update() = 0;

    // virtual void configure(const std::vector<std::byte>& masterIOMap, ) = 0;

    virtual void linkBuffers(const BufferOffsetHolder& inputOffsetHolder, const BufferOffsetHolder& outputOffsetHolder)
    {

    }

    virtual int getVendorID()
    {
        return vendorID;
    }

    virtual int getProductCode()
    {
        return productCode;
    }

    virtual int getALias()
    {
        return alias;
    }

    virtual int getPosition()
    {
        return position;
    }

    virtual ~Slove() = default;
};

#endif //H4_SDK_SLOVE_H
