# HashTable

Implement a templated `HashTable` class similar to` std :: unordered_map`.

It is not necessary to completely repeat the `std :: unordered_map` interface, implement only the necessary elements.

Template class parameters:
- `K` - key type
- `V` - value type
- `Hash` - a type describing a hash function, by default` std :: hash <K> `
- `KeyEqual` - a type describing the key comparison function, by default` std :: equal_to <K> `

Constructors:
- default
- from collection `std :: pair <K, V>` (templated)
- `copy` and` move`

Methods:
- `copy` and` move` operators `=`
- `[]`, `at`
- `find`
- `begin` /` end`
- `empty`,` size`
- `clear`
- `insert` - in a variant that accepts` const std :: pair <K, V> & `
- `emplace`

Helper classes:
- constant and non-constant iterator

## Implementation

Ideas for implementing value storage, collision resolution and hashing can be taken from
[from Wikipedia article] (https://en.wikipedia.org/wiki/Hash_table).

Make constant and non-constant variants of methods where applicable.

The use of standard associative containers is prohibited.
