//
// Created by srfas on 3/26/2026.
//

#ifndef SOEM_CPP_PDO_H
#define SOEM_CPP_PDO_H

#include <vector>
#include "BufferOffsetHolder.h"


struct PDO
{
    int address;

    explicit PDO(int address);

    void linkBuffers(const std::vector<std::byte>& masterIOMap, BufferOffsetHolder& bufferOffsetHolder);

    [[nodiscard]] int getAddress() const { return address; }

    virtual ~PDO() = default;
};

#endif //SOEM_CPP_PDO_H
