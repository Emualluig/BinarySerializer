# BinarySerializer
### Serialize and de-serialize from a binary file

This is a small library to provide serialization and deserialization capabilities.

It can serialize/deserialize most basic types and the most commonly used STL containers.

It is also easily extendable to your own custom types and classes.

# Requires:

C++20 and concepts

# How to use

Here is a small sample for its basic usage

```C++
#include "ByteStreams.h"
#include "Serialize.h"
#include "Deserialize.h"

auto myvec = std::vector<int>{1,2,3,4,5};

// Create OutByteStream to write out data to file "vector.bin"
OutByteStream obs = OutByteStream("vector.bin", true); // True here means that the bytes will be written to the file occasionally without calling .writeToFile

// Serialize the data
serialize(myvec, obs);

// Write the data to the file
obs.writeToFile();

// Create InByteStream to read in data from file "vector.bin"
InByteStream ibs = InByteStream("vector.bin");

// Deserialize back into original container
auto deserializedVec = deserialize<std::vector<int>>(ibs);

```

In addition to all arithmetic types being implemented, here are all of the STL containers that are implemented:
```C++
std::vector
std::string
std::set
std::unordered_set
std::map
std::unordered_map
```

# Extending

In order for your class to be serializable/deserializable you must implement these methods
```C++
void T::serialize(const T& data, OutByteStream& obs);
T::T(InByteStream& ibs); // This is the deserialization constructor
```

In these methods you must implement what class fields should be serialized/deserialized.

Here is an example implementation:

```C++
class TestClass {
    int a;
    int b;
    int c;
public:
    // This is the deserialization constructor
    explicit TestClass(InByteStream& ibs) {
        a = deserialize<int>(ibs);
        b = deserialize<int>(ibs);
        c = deserialize<int>(ibs);
    }
    static void serialize(const TestClass& tc, OutByteStream& obs) {
        // Use the global scope
        ::serialize(tc.a, obs);
        ::serialize(tc.b, obs);
        ::serialize(tc.c, obs);
    }
};
```

Typically the serializations/deserializations are implemented recursively, since most types are aggregations of other more fundamental types. 

# Limitations
It does not type check. So if you are deserializing to the wrong type there will be an error.

The serialize/deserialize does not handle pointers. Cast pointers to std::size_t for it to be serialized. Deserialize as std::size_t then cast to pointer type.

Serializing/deserializing a graph like structure will not work since the objects will not be placed back in the original memory positions. This means that pointers are invalidated. To properly serialize/deserialize a graph consider using a system where each node has a unique ID, and then simply connecting the correct IDs together after deserializing.

# TODO
- [ ] Improve file handling
- [ ] Implement exceptions
