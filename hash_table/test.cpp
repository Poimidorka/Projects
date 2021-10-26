#include <catch.hpp>

#include "hash_table.h"

namespace {
class BadHash {
public:
    size_t operator()(int) const {
        return 0;
    }
};
}  // namespace

TEST_CASE("HashTable") {
    HashTable<std::string, int> hash_table;
    hash_table["a"] = 1;
    hash_table["b"]++;

    REQUIRE(hash_table.find("a")->second == hash_table.at("b"));

    const HashTable<std::string, int> hash_table_copy = hash_table;
    for (const auto& [key, value] : hash_table_copy) {
        REQUIRE(!key.empty());
        REQUIRE(1 == value);
    }

    REQUIRE(2 == hash_table.size());
    hash_table.clear();
    REQUIRE(0 == hash_table.size());
    REQUIRE(hash_table.empty());

    {
        const auto& [it, inserted] = hash_table.emplace("c", 3);
        REQUIRE(inserted);
    }
    {
        const auto& [it, inserted] = hash_table.insert({"c", 3});
        REQUIRE(!inserted);
    }
}

TEST_CASE("HashTable_BadHash") {
    const size_t expected_size = 10000;
    HashTable<int, int, BadHash> hash_table;

    for (size_t i = 0; i < expected_size; ++i) {
        hash_table[i] = i;
    }

    REQUIRE(expected_size == hash_table.size());

    for (const auto& [key, value] : hash_table) {
        REQUIRE(key == value);
        REQUIRE(hash_table.at(key) == value);
        REQUIRE(hash_table.find(key)->second == value);
    }
}
