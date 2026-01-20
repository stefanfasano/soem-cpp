//
// Created by srfas on 1/6/2026.
//

#ifndef H4_SDK_SLOVE_H
#define H4_SDK_SLOVE_H

class Slove
{
    const int vendorID;
    const int productCode;
    const int alias;
    const int position;

    public:

    Slove(const int& vendorID, const int& productCode, const int& alias, const int& position) : vendorID(vendorID), productCode(productCode), alias(alias), position(position) {}

    virtual void update() = 0;

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
