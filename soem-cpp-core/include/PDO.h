//
// Created by srfas on 3/26/2026.
//

#ifndef SOEM_CPP_PDO_H
#define SOEM_CPP_PDO_H


class PDO {

    int address;

    public:
    explicit PDO(int address);

    [[nodiscard]] int getAddress() const { return address; }

    virtual ~PDO() = default;
};

#endif //SOEM_CPP_PDO_H