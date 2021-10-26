#pragma once

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <vector>
#include <tuple>

class Poly {
public:
    struct Member {
        int64_t a;
        size_t x;
        Member* l;
        Member* r;
        Member();
        Member(size_t x, int64_t a);
    };
    Poly();
    Poly(std::vector<int64_t> coeff);
    Poly(std::vector<Member> coeff);
    Member* Remove(Member* ptr) const;
    int64_t operator()(int64_t value) const;
    bool operator==(const Poly& b) const;
    bool operator!=(const Poly& b) const;
    void operator=(const Poly& b);
    ~Poly();
    Member* head;
    void Clear();
    void ToBegin();
    friend std::ostream& operator<<(std::ostream& out, const Poly& p);
};

void operator+=(Poly& a, const Poly& b);
void operator-=(Poly& a, const Poly& b);
Poly operator*(const Poly& a, const Poly& b);
Poly operator-(const Poly& a, const Poly& b);
Poly operator-(const Poly& b);
Poly operator+(const Poly& a, const Poly& b);
