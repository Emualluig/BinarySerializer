#ifndef __HEADER_BYTESTREAMS_H_
#define __HEADER_BYTESTREAMS_H_

#include <type_traits>
#include <concepts>

#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

#include <cassert>
#include <queue>
#include <exception>
#include <fstream>

class OutByteStream;
class InByteStream;

template<typename T> concept isVector = std::same_as<T, std::vector<typename T::value_type, typename T::allocator_type>>;

template<typename T> concept isSet =
    std::same_as<T, std::set<typename T::key_type, typename T::key_compare, typename T::allocator_type>> ||
    std::same_as<T, std::unordered_set<typename T::key_type, typename T::hasher, typename T::key_equal, typename T::allocator_type>>;

// Implements isMap<T> for both map and unordered_map
template<typename T> concept isMap =
    std::same_as<T, std::map<typename T::key_type, typename T::mapped_type, typename T::key_compare, typename T::allocator_type>> ||
    std::same_as<T, std::unordered_map<typename T::key_type, typename T::mapped_type, typename T::hasher, typename T::key_equal, typename T::allocator_type>>;

// Works for ints, floats, bool
template<class T> concept Arithmetic = std::is_arithmetic<T>::value;

// Check if a type can be serialized (has method .serialize() -> )
template<class T> concept Serializable = requires (T object, OutByteStream & obsType) {
    {T::serialize(object, obsType)} -> std::same_as<void>;
};

// Check if a type can be deserialized
template<class T> concept Deserializable = requires (T object, InByteStream & ibsType) {
    {T(ibsType)} -> std::same_as<T>;
};

constexpr const uint8_t BITS_PER_BYTE = 8;
constexpr const uint8_t BOTTOM_BYTE_MASK = 0xFF;
constexpr const std::size_t BUFFER_REFILL_SIZE = 4096;

class OutByteStream {
    friend class InByteStream;
    const bool writeFile;
    const bool bufferFlush;
    const std::string path;
    std::queue<uint8_t> bytesBuffer;
    std::ofstream fout;
public:
    // WriteFile can be turned on to write the file to the specified path, bufferFlush automatically flushed the buffer to the file after a certain number of bytes
    explicit OutByteStream(const std::string& path, bool writeFile = false, bool bufferFlush = false) noexcept : path{ path }, writeFile{ writeFile }, bufferFlush{ bufferFlush } {
        if (writeFile) {
            fout.open(path, std::ios::out | std::ios::binary);
        }
    }
    void push(uint8_t byte) noexcept {
        bytesBuffer.push(byte);
        if (writeFile && bufferFlush && bytesBuffer.size() >= BUFFER_REFILL_SIZE) {
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
        if (!writeFile) {
            return;
        }
        // TODO: improve error handling, file reading
        while (!bytesBuffer.empty()) {
            uint8_t byte = bytesBuffer.front();
            bytesBuffer.pop();
            fout.write((const char*)&byte, sizeof(uint8_t));
        }
    }
    ~OutByteStream() {
        if (writeFile) {
            fout.close();
        }
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
    explicit InByteStream(const std::string& path) : hasFile{ true }, file{ std::ifstream(path, std::ios::binary) } {
        readBufferUntilSize(BUFFER_REFILL_SIZE);
    }
    explicit InByteStream(const OutByteStream& byteStream) : hasFile{ false } {
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
        }
        else {
            return 0 == bytesBuffer.size();
        }
    }
};
class DeserializationError : public std::exception {
    // TODO: implement exceptions
public:
    virtual const char* what() const {
        return "Deserialization Error";
    }
};

#endif // !__HEADER_BYTESTREAMS_H_

