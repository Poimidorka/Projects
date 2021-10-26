#include "cow_string.h"

CowString::FakeChar::operator char() const {
    return *ptr;
}

CowString::FakeChar::FakeChar(size_t ind, CowString& par) : parent(par) {
    ptr = par.own->begin() + ind;
}

void CowString::FakeChar::operator=(char c) {
    if (*(parent.cnt) == 1) {
        *ptr = c;
    }
    else if (*ptr != c) {
        size_t ind = ptr - parent.own->begin();
        parent.own = std::shared_ptr<std::string>(new std::string(*(parent.own)));
        --(*(parent.cnt));
        parent.cnt = std::shared_ptr<size_t>(new size_t(1));
        (*(parent.own))[ind] = c;
        ptr = parent.own->begin() + ind;
    }
}

void CowString::Iterator::operator++() {
    ++ind;
}

bool CowString::Iterator::operator!=(const Iterator& other) {
    return ind != other.ind || &parent != &other.parent;
}

CowString::FakeChar CowString::Iterator::operator*() {
    return FakeChar(ind, parent);
}

void CowString::ConstIterator::operator++() {
    ++ind;
}

bool CowString::ConstIterator::operator!=(const ConstIterator& other) {
    return ind != other.ind || &parent != &other.parent;
}

char CowString::ConstIterator::operator*() {
    return (*parent.own)[ind];
}

CowString::Iterator::Iterator(CowString& parent, size_t ind) : parent(parent), ind(ind) {
}

CowString::ConstIterator::ConstIterator(const CowString& parent, size_t ind) : parent(parent), ind(ind) {
}

CowString::Iterator CowString::begin() {
    return CowString::Iterator(*this, 0);
}

CowString::Iterator CowString::end() {
    return CowString::Iterator(*this, this->size());
}

CowString::ConstIterator CowString::begin() const {
    return CowString::ConstIterator(*this, 0);
}

CowString::ConstIterator CowString::end() const {
    return CowString::ConstIterator(*this, this->size());
}

CowString::FakeChar CowString::operator[](size_t ind) {
    return FakeChar(ind, *this);
}

char CowString::operator[](size_t ind) const {
    return (*own)[ind];
}

char CowString::At(size_t ind) const {
    return (*own)[ind];
}

const char* CowString::GetData() const {
    return own->data();
}

CowString::operator std::string_view() const {
    return std::string_view(*own);
}

CowString::CowString(const std::string_view& s) {
    own = std::shared_ptr<std::string> (new std::string());
    cnt = std::shared_ptr<size_t> (new size_t(1));
    for (char el : s) {
        *own += el;
    }
}

CowString::CowString(const CowString& other) {
    own = other.own;
    cnt = other.cnt;
    ++(*cnt);
}

void CowString::operator=(const CowString& other) {
    --(*cnt);
    own = other.own;
    cnt = other.cnt;
    ++(*cnt);
}

size_t CowString::size() const {
    return own->size();
}