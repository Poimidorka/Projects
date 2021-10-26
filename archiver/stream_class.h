#pragma once

#include <fstream>
#include "cell.h"

class StreamClass {
public:
    void Put(BigCode code);
    BigCode Get(size_t cnt);
    StreamClass(std::ifstream* flow);
    StreamClass(std::ofstream* flow);
    StreamClass();
    void Change(unsigned char& c, signed char i, bool x) const;
    void CheckIn(std::istream* in);
    void CheckOut(std::ostream* out);

private:
    std::istream* in_flow_;
    std::ostream* out_flow_;
    unsigned char buffer_;
    signed char buffer_ptr_;
};
