#include "stream_class.h"
#include <iostream>

StreamClass::StreamClass(std::ifstream* flow) {
    buffer_ = 0;
    in_flow_ = dynamic_cast<std::istream*>(flow);
    buffer_ptr_ = 8;
    out_flow_ = nullptr;
}

StreamClass::StreamClass(std::ofstream* flow) {
    buffer_ptr_ = 0;
    buffer_ = 0;
    out_flow_ = dynamic_cast<std::ostream*>(flow);
    in_flow_ = nullptr;
}

void StreamClass::Change(unsigned char& c, signed char i, bool x) const {
    if (x == 1) {
        c |= static_cast<unsigned char>(1 << i);
    }
    else {
        c &= static_cast<unsigned char>((1 << 8) - 1 - (1 << i));
    }
}

void StreamClass::Put(BigCode code) {
    if (code.empty()) {
        if (buffer_ptr_ != 0) {
            CheckOut(out_flow_);
            (*out_flow_).put(buffer_);
            buffer_ = 0;
            buffer_ptr_ = 0;
        }
    }
    for (bool p : code) {
        if (buffer_ptr_ == 8) {
            (*out_flow_).put(buffer_);
            buffer_ptr_ = 0;
            buffer_ = 0;
        }
        Change(buffer_, buffer_ptr_, p);
        ++buffer_ptr_;
    }
}

void StreamClass::CheckIn(std::istream* in) {
    if (!(*in) || (*in).eof() || (*in).bad()) {
        throw std::runtime_error("Something wrong with input file.\n"
                                 "Might it's damaged or something is interrupting my filestream ;(");
    }
}

void StreamClass::CheckOut(std::ostream* out) {
    if (!(*out) || (*out).bad()) {
        throw std::runtime_error("Something wrong with output file.\n"
                                 "Might something is interrupting my filestream ;(");
    }
}

StreamClass::StreamClass() {
    in_flow_ = nullptr;
    out_flow_ = nullptr;
}

BigCode StreamClass::Get(size_t cnt) {
    BigCode ans(cnt);
    for (size_t i = 0; i < cnt; ++i) {
        if (buffer_ptr_ == 8) {
            CheckIn(in_flow_);
            buffer_ = (*in_flow_).get();
            buffer_ptr_ = 0;
        }
        ans[i] = (buffer_ >> buffer_ptr_) % 2;
        ++buffer_ptr_;
    }
    return ans;
}