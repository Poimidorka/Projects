#pragma once

#include <vector>
#include <tuple>

template<typename K, typename V, typename Hash = std::hash<K>, typename KeyEqual = std::equal_to<K>>
class HashTable{
public:
    using ElemArray = std::vector<std::vector<std::pair<K, V>>>;
    struct Iterator{
        ElemArray& arr;
        size_t i, j;
        Iterator(ElemArray& arr, size_t i, size_t j) : arr(arr), i(i), j(j) {
        }
        bool operator!=(const Iterator& other) const {
            return std::tie(i, j) != std::tie(other.i, other.j);
        }
        void operator++() {
            if (j + 1 < arr[i].size()) {
                ++j;
            } else {
                j = 0;
                ++i;
                while (i != arr.size() && arr[i].empty()) {
                    ++i;
                }
            }
        }
        std::pair<K, V>& operator*() {
            return arr[i][j];
        };
        std::pair<K, V>* operator->() {
            return &arr[i][j];
        }
    };
    struct ConstIterator{
        const ElemArray& arr;
        size_t i, j;
        ConstIterator(const ElemArray& arr, size_t i, size_t j) : arr(arr), i(i), j(j) {
        }
        bool operator!=(const ConstIterator& other) const {
            return std::tie(i, j) != std::tie(other.i, other.j);
        }
        void operator++() {
            if (j + 1 < arr[i].size()) {
                ++j;
            } else {
                j = 0;
                ++i;
                while (i != arr.size() && arr[i].empty()) {
                    ++i;
                }
            }
        }
        const std::pair<K, V>& operator*() const {
            return arr[i][j];
        };
    };
    size_t size() const {
        return cnt_;
    }
    bool empty() const {
        return cnt_ == 0;
    }
    HashTable() {
        cnt_ = 0;
        arr_.resize(1);
    }
    template <typename T>
    HashTable(T list) {
        cnt_ = 0;
        arr_ = {{}};
        for (std::pair<K, V> &el : list) {
            insert(el);
        }
    }
    HashTable(const HashTable& a) {
        arr_ = a.arr_;
        cnt_ = a.cnt_;
    }
    void operator=(const HashTable& a) {
        arr_ = a.arr_;
        cnt_ = a.cnt_;
    }
    HashTable(HashTable&& a) {
        std::swap(arr_, a.arr_);
        std::swap(cnt_, a.cnt_);
    }
    void operator=(HashTable&& a) {
        std::swap(arr_, a.arr_);
        std::swap(cnt_, a.cnt_);
    }
    Iterator find(K key){
        size_t ind = Hash()(key) % arr_.size();
        size_t j = 0;
        for (auto &elem : arr_[j]) {
            if (KeyEqual()(elem.first, key)) {
                return Iterator(arr_, ind, j);
            }
            ++j;
        }
        return end();
    }
    std::pair<Iterator, bool> insert(const std::pair<K, V>& elem) {
        Iterator it = find(elem.first);
        if (it != end()) {
            return std::make_pair(it, false);
        }
        size_t ind = Hash()(elem.first) % arr_.size();
        arr_[ind].push_back(elem);
        ++cnt_;
        RebuildIf();
        return std::make_pair(find(elem.first), true);
    }
    std::pair<Iterator, bool> emplace(K first, V second) {
        auto elem = std::make_pair(first, second);
        return insert(elem);
    }
    void clear() {
        arr_ = {{}};
        cnt_ = 0;
    }
    Iterator begin() {
        return Iterator(arr_, 0, 0);
    }
    Iterator end() {
        return Iterator(arr_, arr_.size(), 0);
    }
    ConstIterator begin() const {
        return ConstIterator(arr_, 0, 0);
    }
    ConstIterator end() const {
        return ConstIterator(arr_, arr_.size(), 0);
    }
    V& operator[](K key) {
        auto it = find(key);
        if (it != end()) {
            return it->second;
        }
        emplace(key, V());
        return find(key)->second;
    }
    V at(K key) {
        auto it = find(key);
        if (it != end()) {
            return it->second;
        }
        exit(3);
    }

private:
    size_t cnt_;
    ElemArray arr_;
    void RebuildIf() {
        if (arr_.size() * 2 < cnt_) {
            auto new_arr = arr_;
            arr_.clear();
            arr_.resize(new_arr.size() * 5 / 2);
            cnt_ = 0;
            for (auto& el : new_arr) {
                for (auto &sub_el : el) {
                    insert(sub_el);
                }
            }
        }
    }
};
