// BinarySerializer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cassert>

#include <type_traits>
#include <cstddef>
#include <concepts>

#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <array>

#include <queue>
#include <exception>

#include <fstream>

#include <iostream>

class OutByteStream;
class InByteStream;
// https://en.cppreference.com/w/cpp/language/types

template<typename T> concept isVector = std::same_as<T, std::vector<typename T::value_type, typename T::allocator_type>>;

// Implements isSet<T> for both set and unordered_set
template<typename T> concept isSet = std::same_as<T, std::set<typename T::key_type, typename T::key_compare, typename T::allocator_type>> ||
                                     std::same_as<T, std::unordered_set<typename T::key_type, typename T::key_compare, typename T::allocator_type>>;

// Implements isMap<T> for both map and unordered_map
template<typename T> concept isMap = std::same_as<T, std::map<typename T::key_type, typename T::mapped_type, typename T::key_compare, typename T::allocator_type>> ||
                                     std::same_as<T, std::unordered_map<typename T::key_type, typename T::mapped_type, typename T::hasher, typename T::key_equal, typename T::allocator_type>>;

// Works for ints, floats, bool
template<class T> concept Arithmetic = std::is_arithmetic<T>::value;

// Check if a type can be serialized (has method .serialize() -> )
template<class T> concept Serializable = requires (T object, OutByteStream& obsType) {
    {object.serialize(obsType)} -> std::same_as<void>;
};

// Check if a type can be deserialized
template<class T> concept Deserializable = requires (T object, InByteStream& ibsType) {
    {T(ibsType)} -> std::same_as<T>;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

constexpr const uint8_t BITS_PER_BYTE = 8;
constexpr const uint8_t BOTTOM_BYTE_MASK = 0xFF;
constexpr const std::size_t BUFFER_REFILL_SIZE = 4096;

class OutByteStream {
    friend class InByteStream;
    const bool bufferFlush;
    const std::string path;
    std::queue<uint8_t> bytesBuffer;
    std::ofstream fout;
public:
    explicit OutByteStream(const std::string& path, bool bufferFlush = true) noexcept : path{ path }, bufferFlush{ bufferFlush } {
        fout.open(path, std::ios::out | std::ios::binary);
    }
    void push(uint8_t byte) noexcept {
        bytesBuffer.push(byte);
        if (bufferFlush && bytesBuffer.size() >= BUFFER_REFILL_SIZE) {
            writeToFile();
        }
    }
    // Merges the unwriten bytes from another `byteStream` into this object
    void merge(OutByteStream& obs) noexcept {
        assert(&obs != this); // Check that they are different objects
        while (!obs.bytesBuffer.empty()) {
            bytesBuffer.push(obs.bytesBuffer.front());
            obs.bytesBuffer.pop();
        }
    }
    bool isBufferEmpty() const noexcept {
        return bytesBuffer.size() == 0;
    }
    // Writes the current byte buffer into the file
    void writeToFile() {
        // TODO: improve error handling, file reading
        while (!bytesBuffer.empty()) {
            uint8_t byte = bytesBuffer.front();
            bytesBuffer.pop();
            fout.write((const char*)&byte, sizeof(uint8_t));
        }
    }
    ~OutByteStream() {
        fout.close();
    }
};
class InByteStream {
    bool hasFile;
    std::ifstream file;
    std::queue<uint8_t> bytesBuffer;

    void readBufferUntilSize(const std::size_t bufferSize) {
        uint8_t byte = 0;
        std::size_t count = 0;
        while (file.read(reinterpret_cast<char*>(&byte), sizeof(uint8_t))) {
            bytesBuffer.push(byte);
            count++;
            if (count >= bufferSize) {
                break;
            }
        }
    }
public:
    explicit InByteStream(const std::string& path) : hasFile{ true }, file{ std::ifstream(path, std::ios::binary)} {
        readBufferUntilSize(BUFFER_REFILL_SIZE);
    }
    explicit InByteStream(const OutByteStream& byteStream) : hasFile{false}  {
        bytesBuffer = byteStream.bytesBuffer;
    }
    uint8_t getByte() {
        if (hasFile) {
            if (bytesBuffer.size() == 0) {
                // we need to refill bytesBuffer
                readBufferUntilSize(BUFFER_REFILL_SIZE);
            }
        }
        if (bytesBuffer.size() == 0) {
            assert(false);
        }
        uint8_t retval = bytesBuffer.front();
        bytesBuffer.pop();
        return retval;
    }
    bool isEmpty() const noexcept {
        if (hasFile) {
            return 0 == bytesBuffer.size() && file.eof();
        } else {
            return 0 == bytesBuffer.size();
        }
    }
};
class DeserializationError : public std::exception {
public:
    virtual const char* what() const {
        return "Deserialization Error";
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////

// Only specialization are allowed
template<typename T> void serialize(const T& data, OutByteStream& obs) = delete;

template<typename T> requires Arithmetic<T> void serialize(const T& data, OutByteStream& obs) {
    constexpr const std::size_t size = sizeof(T);
    const uint8_t* a = (uint8_t*)&data;
    for (std::size_t i = 0; i < size; i++) {
        obs.push(a[i]);
    }
}
template<> void serialize(const std::string& data, OutByteStream& obs) {
    serialize(data.size(), obs);
    for (const char c : data) {
        serialize(c, obs);
    }
}
template<typename T> void serialize(const std::vector<T>& data, OutByteStream& obs) {
    serialize(data.size(), obs);
    for (const T& elem : data) {
        serialize(elem, obs);
    }
}
template<typename T> void serialize(const std::set<T>& data, OutByteStream& obs) {
    serialize(data.size(), obs);
    for (const T& elem : data) {
        serialize(elem, obs);
    }
}
template<typename T, typename U> void serialize(const std::map<T, U>& data, OutByteStream& obs) {
    serialize(data.size(), obs);
    for (const auto& [ key, value ] : data) {
        serialize(key, obs);
        serialize(value, obs);
    }
}
template<typename T> requires Serializable<T> void serialize(const T& data, OutByteStream& obs) {
    data.serialize(obs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// Only specialization are allowed
template<typename T> T deserialize(InByteStream& ibs) = delete;

template<typename T> requires Arithmetic<T> T deserialize(InByteStream& ibs) {
    constexpr const std::size_t size = sizeof(T);
    uint8_t s[size];
    for (std::size_t i = 0; i < size; i++) {
        s[i] = ibs.getByte();
    }
    const T* b = (T*)s;
    const T c = *b;
    return c;
}
template<> std::string deserialize<std::string>(InByteStream& ibs) {
    std::string retval;
    const std::size_t size = deserialize<std::size_t>(ibs);
    for (std::size_t i = 0; i < size; i++) {
        char c = deserialize<char>(ibs);
        retval += c;
    }
    return retval;
}
template<typename T> requires isVector<T> T deserialize(InByteStream& ibs) {
    using B = typename T::value_type;
    T retval;
    const std::size_t size = deserialize<std::size_t>(ibs);
    for (std::size_t i = 0; i < size; i++) {
        retval.push_back(deserialize<B>(ibs));
    }
    return retval;
}
template<typename T> requires isSet<T> T deserialize(InByteStream& ibs) {
    using B = typename T::key_type;
    T retval;
    const std::size_t size = deserialize<std::size_t>(ibs);
    for (std::size_t i = 0; i < size; i++) {
        retval.insert(deserialize<B>(ibs));
    }
    return retval;
}
template<typename T> requires isMap<T> T deserialize(InByteStream& ibs) {
    using A = typename T::key_type;
    using B = typename T::mapped_type;
    T retval;
    const std::size_t size = deserialize<std::size_t>(ibs);
    for (std::size_t i = 0; i < size; i++) {
        retval.insert({ deserialize<A>(ibs), deserialize<B>(ibs) });
    }
    return retval;
}
template<typename T> requires Deserializable<T> T deserialize(InByteStream& ibs) {
    return T(ibs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void testIntegral_Serialize_Deserialize() {
    // TESTS FOR UINT8_T
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const uint8_t value = 0;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<uint8_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const uint8_t value = std::numeric_limits<uint8_t>::min();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<uint8_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const uint8_t value = std::numeric_limits<uint8_t>::max();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<uint8_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    // END UINT8_T
    // TESTS FOR UINT16_T
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const uint16_t value = 0;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<uint16_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const uint16_t value = std::numeric_limits<uint16_t>::min();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<uint16_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const uint16_t value = std::numeric_limits<uint16_t>::max();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<uint16_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    // END UINT16_T
    // TESTS FOR UINT32_T
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const uint32_t value = 0;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<uint32_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const uint32_t value = std::numeric_limits<uint32_t>::min();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<uint32_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const uint32_t value = std::numeric_limits<uint32_t>::max();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<uint32_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    // END UINT32_T
    // TESTS FOR UINT64_T
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const uint64_t value = 0;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<uint64_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const uint64_t value = std::numeric_limits<uint64_t>::min();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<uint64_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const uint64_t value = std::numeric_limits<uint64_t>::max();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<uint64_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    // END UINT64_T
    // TESTS FOR INT8_T
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int8_t value = 0;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int8_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int8_t value = std::numeric_limits<int8_t>::min();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int8_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int8_t value = std::numeric_limits<int8_t>::max();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int8_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    // END INT8_T
    // TESTS FOR INT16_T
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int16_t value = 0;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int16_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int16_t value = std::numeric_limits<int16_t>::min();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int16_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int16_t value = std::numeric_limits<int16_t>::max();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int16_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    // END INT16_T
    // TESTS FOR INT32_T
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int32_t value = 0;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int32_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int32_t value = std::numeric_limits<int32_t>::min();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int32_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int32_t value = std::numeric_limits<int32_t>::max();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int32_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    // END INT32_T
    // TESTS FOR INT64_T
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int64_t value = 0;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int64_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int64_t value = std::numeric_limits<int64_t>::min();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int64_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int64_t value = std::numeric_limits<int64_t>::max();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int64_t>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    // END INT64_T
    // TESTS FOR CHAR
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const char value = 0;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<char>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const char value = std::numeric_limits<char>::min();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<char>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const char value = std::numeric_limits<char>::max();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<char>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    // END CHAR
    // TESTS FOR UNSIGNED CHAR
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const unsigned char value = 0;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<char>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const unsigned char value = std::numeric_limits<unsigned char>::min();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<char>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const unsigned char value = std::numeric_limits<unsigned char>::max();
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<unsigned char>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    // END UNSIGNED CHAR
    // TEST FOR INT
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int value = 0;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int value = std::numeric_limits<int>::min();;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const int value = std::numeric_limits<int>::max();;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<int>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    // END INT
    // TEST FOR SHORT INT
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const short int value = 0;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<short int>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const short int value = std::numeric_limits<short int>::min();;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<short int>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    { 
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const short int value = std::numeric_limits<short int>::max();;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<short int>(ibs);
        assert(i == value); assert(ibs.isEmpty());
    }
    // END SHORT INT
}

void testFloat_Serialize_Deserialize() {
    // TESTS FOR FLOAT
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const float value = std::numeric_limits<float>::min();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<float>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const float value = std::numeric_limits<float>::max();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<float>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const float value = std::numeric_limits<float>::denorm_min();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<float>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const float value = std::numeric_limits<float>::epsilon();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<float>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const float value = std::numeric_limits<float>::infinity();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<float>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const float value = std::numeric_limits<float>::lowest();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<float>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const float value = std::numeric_limits<float>::max();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<float>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    // END FLOAT
    // TESTS FOR DOUBLE
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const double value = std::numeric_limits<double>::min();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<double>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const double value = std::numeric_limits<double>::max();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<double>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const double value = std::numeric_limits<double>::denorm_min();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<double>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const double value = std::numeric_limits<double>::epsilon();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<double>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const double value = std::numeric_limits<double>::infinity();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<double>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const double value = std::numeric_limits<double>::lowest();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<double>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const double value = std::numeric_limits<double>::max();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<double>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    // END DOUBLE
    // TESTS FOR LONG DOUBLE
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const long double value = std::numeric_limits<long double>::min();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<long double>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const long double value = std::numeric_limits<long double>::max();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<long double>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const long double value = std::numeric_limits<long double>::denorm_min();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<long double>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const long double value = std::numeric_limits<long double>::epsilon();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<long double>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const long double value = std::numeric_limits<long double>::infinity();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<long double>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const long double value = std::numeric_limits<long double>::lowest();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<long double>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    {
        OutByteStream obs = OutByteStream("./testFloat.bin");
        constexpr const long double value = std::numeric_limits<long double>::max();
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<long double>(ibs);
        assert(i == value);
        assert(ibs.isEmpty());
    }
    // END LONG DOUBLE
}

void testVector_Serialize_Deserialize() {
    // TODO: implement more tests
    {
        OutByteStream obs = OutByteStream("./testVector.bin");
        std::vector<std::vector<int>> value = { { 1,2,3,4,5 }, { 1,2,3,4,5 } };
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        std::vector<std::vector<int>> i = deserialize<std::vector<std::vector<int>>>(ibs);
        assert(value == i);
    }
}

void testString_Serialize_Deserialize() {
    // TODO: implement more tests
    {
        OutByteStream obs = OutByteStream("./testString.bin");
        std::string value = "Hello, World!";
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        auto i = deserialize<std::string>(ibs);
        assert(value == i);
    }
    {
        OutByteStream obs = OutByteStream("./testString.bin");
        std::string value = "a";
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        auto i = deserialize<std::string>(ibs);
        assert(value == i);
    }
    {
        OutByteStream obs = OutByteStream("./testString.bin");
        std::string value = "";
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        auto i = deserialize<std::string>(ibs);
        assert(value == i);
    }
    {
        OutByteStream obs = OutByteStream("./testString.bin");
        std::string value = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        auto i = deserialize<std::string>(ibs);
        assert(value == i);
    }
    {
        OutByteStream obs = OutByteStream("./testString.bin");
        std::string value = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        auto i = deserialize<std::string>(ibs);
        assert(value == i);
    }
}

void testSet_Serialize_Deserialize() {
    // TODO: implement more tests (test both set and unordered_set)
    {
        OutByteStream obs = OutByteStream("./testSet.bin");
        const std::set<int> value = { 1, 2 };
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<std::set<int>>(ibs);
        assert(value == i);
    }
}

void testMap_Serialize_Deserialize() {
    // TODO: implement more tests (test both map and unordered_map)
    {
        OutByteStream obs = OutByteStream("./testMap.bin");
        const std::map<int, int> value = { {1,1}, {2,2} };
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<std::map<int, int>>(ibs);
    }
}

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
    // This is the serialization method
    void serialize(OutByteStream& obs) {
        // Use the global scope
        ::serialize(a, obs);
        ::serialize(b, obs);
        ::serialize(c, obs);
    }
};

int main()
{
#define testing false
#if testing
    // This is Arithmetic
    testIntegral_Serialize_Deserialize();
    testFloat_Serialize_Deserialize();

    testVector_Serialize_Deserialize();
    testString_Serialize_Deserialize();

    testSet_Serialize_Deserialize();

    testMap_Serialize_Deserialize();
#endif

#if false
    {
        OutByteStream obs = OutByteStream("./testVector.bin");
        std::vector<int> value = {};
        for (int i = 0; i < 2000000; i++) {
            value.push_back(i);
        }
        serialize(value, obs);
        obs.writeToFile();
    }

    {
        InByteStream ibs = InByteStream("./testVector.bin");
        const auto i = deserialize<std::vector<int>>(ibs);
    }
#endif

    return 0;
}
