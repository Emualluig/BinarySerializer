#include "Tests.h"

#include "ByteStreams.h"
#include "Serialize.h"
#include "Deserialize.h"


void testIntegral_Serialize_Deserialize_2() {
    // TESTS FOR UINT8_T
    {
        OutByteStream obs = OutByteStream("./testIntegral.bin");
        constexpr const uint8_t value = 0;
        serialize(value, obs); InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<uint8_t>(ibs);
        assert(i == value); 
        assert(ibs.isEmpty());
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
}

void testIntegral_Serialize_Deserialize() {
    testIntegral_Serialize_Deserialize_2();
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

void testString_Serialize_Deserialize() {
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

void testVector_Serialize_Deserialize() {
    {
        OutByteStream obs = OutByteStream("./testVector.bin");
        std::vector<int> value = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 };
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<std::vector<int>>(ibs);
        assert(value == i);
    }
    {
        OutByteStream obs = OutByteStream("./testVector.bin");
        std::vector<float> value = {};
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<std::vector<float>>(ibs);
        assert(value == i);
    }
    {
        OutByteStream obs = OutByteStream("./testVector.bin");
        std::vector<float> value = { 0.09f, 0.31f, 3.14f, 4.5f };
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<std::vector<float>>(ibs);
        assert(value == i);
    }
    {
        OutByteStream obs = OutByteStream("./testVector.bin");
        std::vector<std::vector<int>> value = { { 1,2,3,4,5 }, { 1,2,3,4,5 } };
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<std::vector<std::vector<int>>>(ibs);
        assert(value == i);
    }
    {
        OutByteStream obs = OutByteStream("./testVector.bin");
        std::vector<std::vector<std::vector<int>>> value = { {{}, {}, {}}, {{}, {}, {}}, {{}, {}, {}} };
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<std::vector<std::vector<std::vector<int>>>>(ibs);
        assert(value == i);
    }
    {
        OutByteStream obs = OutByteStream("./testVector.bin");
        std::vector<std::string> value = {
            {"Hello there!"}, {"1234567890-=`~"}, {"~!@#$%^&*()_+"},
            {"qwertyuiop[]\\"}, {"QWERTYUIOP{}|"}, {"Hello there!"} };
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<std::vector<std::string>>(ibs);
        assert(value == i);
    }
}

void testSet_Serialize_Deserialize() {
    {
        OutByteStream obs = OutByteStream("./testSet.bin");
        const std::set<int> value = { 1, 2 };
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<std::set<int>>(ibs);
        assert(value == i);
    }
    {
        OutByteStream obs = OutByteStream("./testSet.bin");
        const std::unordered_set<int> value = { 1, 2 };
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<std::unordered_set<int>>(ibs); // <- there is an error but it is wrong
        assert(value == i);
    }
    {
        OutByteStream obs = OutByteStream("./testSet.bin");
        const std::unordered_set<int> value = { 1, 2,3,4,5,6,7,8,9,10,11,12,13,14,999,-9999 };
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<std::unordered_set<int>>(ibs); // <- there is an error but it is wrong
        assert(value == i);
    }
    {
        OutByteStream obs = OutByteStream("./testSet.bin");
        const std::unordered_set<std::string> value = { "`1234567890-=", "~!@#$%^&*()_+", "qwertyuiop[]\\", "QWERTYUIOP{}|" };
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<std::unordered_set<std::string>>(ibs); // <- there is an error but it is wrong
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
        assert(i == value);
    }
    {
        OutByteStream obs = OutByteStream("./testMap.bin");
        const std::unordered_map<int, int> value = { {1,1}, {2,2} };
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<std::unordered_map<int, int>>(ibs);
        assert(i == value);
    }
}

void testLarge_Serialization_Deserialization() {
    constexpr int limit = 20'000'000;
    std::vector<int> value = {};
    for (int i = 0; i < limit; i++) {
        value.push_back(i);
    }
    {
        {
            OutByteStream obs = OutByteStream("./testVector.bin");
            serialize(value, obs);
            obs.writeToFile();
        }

        {
            InByteStream ibs = InByteStream("./testVector.bin");
            const auto i = deserialize<std::vector<int>>(ibs);
            for (int index = 0; index < limit; index++) {
                assert(value[index] == i[index]);
            }
        }
    }
}

class TestClass {
    int a;
    int b;
    int c;
public:
    explicit TestClass(int a, int b, int c) noexcept : a{ a }, b{ b }, c{ c } {}
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

    bool operator==(const TestClass& rhs) const noexcept {
        return (this->a == rhs.a) && (this->b == rhs.b) && (this->c == rhs.c);
    }
};

void testClass_Serialize_Deserialize() {
    // TODO: more tests for classes
    {
        OutByteStream obs = OutByteStream("./testMap.bin");
        const auto value = TestClass(1, 2, 3);
        serialize(value, obs);
        InByteStream ibs = InByteStream(obs);
        const auto i = deserialize<TestClass>(ibs);
        assert(i == value);
    }
}

void runTests() {
    testIntegral_Serialize_Deserialize();
    testFloat_Serialize_Deserialize();

    testVector_Serialize_Deserialize();
    testString_Serialize_Deserialize();

    testSet_Serialize_Deserialize();

    testMap_Serialize_Deserialize();

    testLarge_Serialization_Deserialization();

    testClass_Serialize_Deserialize();
}