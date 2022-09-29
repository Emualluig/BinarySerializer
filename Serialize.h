#ifndef __HEADER_SERIALIZE_H_
#define __HEADER_SERIALIZE_H_

#include "ByteStreams.h"

// Only specialization are allowed
template<typename T> void serialize(const T& data, OutByteStream& obs) noexcept = delete;

template<typename T> requires Arithmetic<T> void serialize(const T& data, OutByteStream& obs) noexcept {
    constexpr const std::size_t size = sizeof(T);
    const uint8_t* a = (uint8_t*)&data;
    for (std::size_t i = 0; i < size; i++) {
        obs.push(a[i]);
    }
}
template<> void serialize(const std::string& data, OutByteStream& obs) noexcept {
    serialize(data.size(), obs);
    for (const char c : data) {
        serialize(c, obs);
    }
}
template<typename T> void serialize(const std::vector<T>& data, OutByteStream& obs) noexcept {
    serialize(data.size(), obs);
    for (const T& elem : data) {
        serialize(elem, obs);
    }
}
template<typename T> void serialize(const std::set<T>& data, OutByteStream& obs) noexcept {
    serialize(data.size(), obs);
    for (const T& elem : data) {
        serialize(elem, obs);
    }
}
template<typename T> void serialize(const std::unordered_set<T>& data, OutByteStream& obs) noexcept {
    serialize(data.size(), obs);
    for (const T& elem : data) {
        serialize(elem, obs);
    }
}
template<typename T, typename U> void serialize(const std::map<T, U>& data, OutByteStream& obs) noexcept {
    serialize(data.size(), obs);
    for (const auto& [key, value] : data) {
        serialize(key, obs);
        serialize(value, obs);
    }
}
template<typename T, typename U> void serialize(const std::unordered_map<T, U>& data, OutByteStream& obs) noexcept {
    serialize(data.size(), obs);
    for (const auto& [key, value] : data) {
        serialize(key, obs);
        serialize(value, obs);
    }
}
template<typename T> requires Serializable<T> void serialize(const T& data, OutByteStream& obs) noexcept {
    T::serialize(data, obs);
}

#endif // !__HEADER_SERIALIZE_H_
