# Copy-on-write string

Implement a `CowString` class that implements the concept of a copy-on-change string.

The idea behind copy-on-write is that the same strings can refer to the same
internal representation, thereby saving memory. When changing the line, it is transparent to
a copy of the internal view should be created for the user and the change should be applied to it.

## Class structure

Constructors:
- from `std :: string_view`
- copy- and move-constructors

Methods and Operators:
- copy- and move- `operator =`
- `GetData`, which returns a pointer to the internal` char * `buffer
- `begin` /` end` in constant and non-constant versions (and corresponding iterators)
- `operator []` and constant method `At`
- operators `+`, `+ =`, `==`, `! =` from `CowString` and` std :: string_view` in various combinations
- operator of implicit casting to `std :: string_view`

Don't forget the destructor.

## Implementation

You will need to implement an additional class - the internal string representation.
Internally, the view must contain a reference count.

When a new line is created, an internal representation with a reference count of 1 is created.

When copied, another line starts to reference the existing view.
Accordingly, the reference count is incremented.

When a row is deleted, the reference count in its internal representation is decremented.

The view should be removed when the last line referencing it is removed.

When modifying a row that alone owns its internal representation, create
no new presentation is needed.

If it was necessary to create a new string representation, all existing iterators
remain valid and "switch" to the new view.

To track the modification of a string through an iterator, the dereference operator
non-const operator must return not `char &`, but a special proxy object, implicitly
cast to `char` with an overloaded assignment operator. A similar technique should be applied
for `operator []`.
