//
// Created by srfas on 3/26/2026.
//

#include "../include/PDO.h"

PDO::PDO(int address) : address(address) {}

void PDO::linkBuffers(const std::vector<std::byte>& masterIOMap, BufferOffsetHolder& bufferOffsetHolder)
{
    size_t size = sizeof(this);
}
