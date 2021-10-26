#include "poly.h"

Poly::Member::Member(size_t x, int64_t a) {
    this->a = a;
    this->x = x;
    l = r = nullptr;
}

Poly::Member::Member() {
    x = a = 0;
    l = r = nullptr;
}

Poly::Poly() {
    head = nullptr;
}

Poly::Poly(std::vector<int64_t> coeff) {
    head = nullptr;
    for (size_t x = 0; x < coeff.size(); ++x) {
        if (coeff[x] != 0) {
            Member* next = new Member(x, coeff[x]);
            if (head) {
                head->r = next;
            }
            next->l = head;
            head = next;
        }
    }
    ToBegin();
}

Poly::Poly(std::vector<Poly::Member> coeff) {
    head = nullptr;
    for (auto& memb : coeff) {
        if (memb.a != 0) {
            Member* next = new Member(memb.x, memb.a);
            if (head) {
                head->r = next;
            }
            next->l = head;
            head = next;
        }
    }
    ToBegin();
}

Poly::Member* Poly::Remove(Poly::Member* ptr) const {
    Member* left = ptr->l;
    Member* right = ptr->r;
    Member* ans = nullptr;
    if (left) {
        left->r = nullptr;
    }
    if (right) {
        right->l = nullptr;
        ans = right;
    }
    if (left && right) {
        left->r = right;
        right->l = left;
    }
    delete ptr;
    return ans;
}

void Insert(Poly::Member* left, Poly::Member* ptr, Poly::Member* right) {
    ptr->l = ptr->r = nullptr;
    if (left) {
        ptr->l = left;
        left->r = ptr;
    }
    if (right) {
        ptr->r = right;
        right->l = ptr;
    }
}

int64_t Poly::operator()(int64_t value) const {
    assert(head == nullptr || head->l == nullptr);
    int64_t ans = 0;
    Member* current = head;
    int64_t current_value = 1;
    size_t current_pow = 0;
    while (current) {
        while (current_pow < current->x) {
            ++current_pow;
            current_value *= value;
        }
        ans += current->a * current_value;
        current = current->r;
    }
    return ans;
}

bool Poly::operator==(const Poly& b) const {
    Member* heada = head;
    Member* headb = b.head;
    while (true) {
        if (!heada && !headb) {
            break;
        }
        if (!heada || !headb || std::tie(heada->x, heada->a) != std::tie(headb->x, headb->a)) {
            return false;
        }
        heada = heada->r;
        headb = headb->r;
    }
    return true;
}

bool Poly::operator!=(const Poly& b) const {
    Member* heada = head;
    Member* headb = b.head;
    while (true) {
        if (!heada && !headb) {
            break;
        }
        if (!heada || !headb || std::tie(heada->x, heada->a) != std::tie(headb->x, headb->a)) {
            return true;
        }
        heada = heada->r;
        headb = headb->r;
    }
    return false;
}

void Poly::operator=(const Poly& b) {
    ToBegin();
    Clear();
    Member* current = b.head;
    while (current) {
        Poly::Member* new_memb = new Poly::Member(current->x, current->a);
        Insert(head, new_memb, nullptr);
        head = new_memb;
        current = current->r;
    }
    ToBegin();
}

void operator+=(Poly& a, const Poly& b) {
    a.ToBegin();
    Poly::Member* heada = a.head;
    Poly::Member* prev = nullptr;
    Poly::Member* headb = b.head;
    if (!a.head) {
        a = b;
        return;
    }
    while (headb) {
        Poly::Member* new_memb = new Poly::Member(headb->x, headb->a);
        while (heada && heada->x < headb->x) {
            prev = heada;
            heada = heada->r;
        }
        if (heada && heada->x == headb->x) {
            heada->a += headb->a;
            delete new_memb;
            if (heada->a == 0) {
                prev = heada->l;
                heada = a.Remove(heada);
            }
        } else {
            Insert(prev, new_memb, heada);
            prev = new_memb;
        }
        headb = headb->r;
    }
    a.head = nullptr;
    if (heada) {
        a.head = heada;
    } else if (prev) {
        a.head = prev;
    }
    a.ToBegin();
}

void operator-=(Poly& a, const Poly& b) {
    a.ToBegin();
    Poly fake;
    fake = b;
    while (fake.head && fake.head->r) {
        fake.head->a *= -1;
        fake.head = fake.head->r;
    }
    if (fake.head) {
        fake.head->a *= -1;
    }
    fake.ToBegin();
    a += fake;
}

std::ostream& operator<<(std::ostream& out, const Poly& p) {
    std::string answer = "y = ";
    if (p.head) {
        Poly::Member* current = p.head;
        while (current->r) {
            current = current->r;
        }
        while (current) {
            answer += std::to_string(current->a);
            if (current->x) {
                answer += "x^";
                answer += std::to_string(current->x);
            }
            if (current->l) {
                answer += " + ";
            }
            current = current->l;
        }
    } else {
        answer += '0';
    }
    out << answer;
    return out;
}

Poly operator+(const Poly& a, const Poly& b) {
    Poly fake;
    fake += a;
    fake += b;
    return fake;
}

Poly operator-(const Poly& a, const Poly& b) {
    Poly fake;
    fake += a;
    fake -= b;
    return fake;
}

Poly operator-(const Poly& b) {
    Poly fake;
    fake -= b;
    return fake;
}

Poly operator*(const Poly& a, const Poly& b) {
    Poly ans;
    Poly::Member* current = b.head;
    while (current) {
        Poly fake;
        fake = a;
        while (fake.head && fake.head->r) {
            fake.head->x += current->x;
            fake.head->a *= current->a;
            fake.head = fake.head->r;
        }
        if (fake.head) {
            fake.head->x += current->x;
            fake.head->a *= current->a;
        }
        fake.ToBegin();
        ans += fake;
        current = current->r;
    }
    return ans;
}

void Poly::ToBegin() {
    while (head && head->l) {
        head = head->l;
    }
}

void Poly::Clear() {
    ToBegin();
    while (head) {
        head = Remove(head);
    }
}

Poly::~Poly() {
    ToBegin();
    Clear();
}