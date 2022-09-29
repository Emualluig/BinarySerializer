#ifndef __HEADER_BYTESTREAMS_H_
#define __HEADER_BYTESTREAMS_H_

#include <type_traits>
#include <concepts>

#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

#include <filesystem>
#include <cassert>
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

    const std::string path;
    std::ofstream fout;

    std::vector<uint8_t> bytes;
public:
    explicit OutByteStream(const std::string& path, bool deletePath = true) noexcept : path{ path } {
        if (deletePath) {
            // Delete the file (will not fail if doesn't exist)
            auto result = std::filesystem::remove(path);
        }
        fout.open(path, std::ios::out | std::ios::binary);
    }
    ~OutByteStream() {
        fout.close();
    }
    void push(uint8_t byte) noexcept {
        bytes.push_back(byte);
        if (bytes.size() >= BUFFER_REFILL_SIZE) {
            writeToFile();
        }
    }
    void writeToFile() {
        for (auto byte : bytes) {
            fout.write((const char*)&byte, sizeof(uint8_t));
        }
        bytes.clear();
    }
};
class InByteStream {
    bool hasFile;
    std::ifstream file;

    std::size_t front = 0;
    std::vector<uint8_t> bytes;

    void readBufferUntilSize(const std::size_t bufferSize) {
        uint8_t byte = 0;
        std::size_t count = 0;
        while (file.read(reinterpret_cast<char*>(&byte), sizeof(uint8_t))) {
            bytes.push_back(byte);
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
    // Invalidates the byteStream object
    explicit InByteStream(OutByteStream& byteStream) : hasFile{ false } {
        bytes = std::move(byteStream.bytes);
    }
    uint8_t getByte() {
        if (hasFile) {
            if (front == bytes.size()) {
                // we need to refill bytesBuffer
                front = 0;
                bytes.clear();
                readBufferUntilSize(BUFFER_REFILL_SIZE);
            }
        }
        if (bytes.size() == 0 && front == 0) {
            assert(false);
        }
        uint8_t retval = bytes.at(front);
        front += 1;
        return retval;
    }
    bool isEmpty() const noexcept {
        if (hasFile) {
            return file.eof() && (0 == bytes.size() || (front == bytes.size() - 1));
        }
        else {
            return front == bytes.size();
        }
    }
};

#endif // !__HEADER_BYTESTREAMS_H_

