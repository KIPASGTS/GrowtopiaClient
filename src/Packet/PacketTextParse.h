#pragma once
#include <string>
#include <unordered_map>

namespace Client {
    class PacketTextParse {
    public:
        PacketTextParse() {};
        ~PacketTextParse() {};

        static PacketTextParse Parse(const std::string& data);
        int Get(const std::string& key, std::string& value);
        void InsertOrAssign(const std::string& key, const std::string& value);
        void InsertOrAssign(const std::string& key, const int& value);
        void Serialize(std::string& data);

    private:
        std::unordered_map<std::string, std::string> m_data;
    };
} // namespace Client