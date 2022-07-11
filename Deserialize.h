#ifndef __HEADER_DESERIALIZE_H_
#define __HEADER_DESERIALIZE_H_

#include "ByteStreams.h"

// Only specialization are allowed
template<typename T> T deserialize(InByteStream& ibs) = delete;

template<typename T> requires Arithmetic<T> T deserialize(InByteStream& ibs) {
    constexpr const std::size_t size = sizeof(T);
    uint8_t s[size] = {};
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
    T retval = {};
    const std::size_t size = deserialize<std::size_t>(ibs);
    for (std::size_t i = 0; i < size; i++) {
        retval.push_back(deserialize<B>(ibs));
    }
    return retval;
}
template<typename T> requires isSet<T> T deserialize(InByteStream& ibs) {
    using B = typename T::key_type;
    T retval = {};
    const std::size_t size = deserialize<std::size_t>(ibs);
    for (std::size_t i = 0; i < size; i++) {
        retval.insert(deserialize<B>(ibs));
    }
    return retval;
}
template<typename T> requires isMap<T> T deserialize(InByteStream& ibs) {
    using A = typename T::key_type;
    using B = typename T::mapped_type;
    T retval = {};
    const std::size_t size = deserialize<std::size_t>(ibs);
    for (std::size_t i = 0; i < size; i++) {
        retval.insert({ deserialize<A>(ibs), deserialize<B>(ibs) });
    }
    return retval;
}
template<typename T> requires Deserializable<T> T deserialize(InByteStream& ibs) {
    return T(ibs);
}

#endif // !__HEADER_DESERIALIZE_H_

