#include <sstream>

#include "PacketTextParse.h"

namespace Client {
    PacketTextParse PacketTextParse::Parse(const std::string& data) {
        PacketTextParse param{};

        std::string::size_type key_pos, value_pos = 0;
        std::string key, value;

        while (value_pos != std::string::npos) {
            key_pos = data.find('|', value_pos + 1);
            if (key_pos == std::string::npos) {
                break;
            }

            key = data.substr(value_pos == 0 ? 0 : value_pos + 1, key_pos - value_pos - (value_pos == 0 ? 0 : 1));

            value_pos = data.find('\n', key_pos + 1);
            if (value_pos == std::string::npos) {
                break;
            }

            value = data.substr(key_pos + 1, value_pos - key_pos - 1);

            param.InsertOrAssign(key, value);
        }

        return param;
    }

    int PacketTextParse::Get(const std::string& key, std::string& value) {
        if (auto data = m_data.find(key); data != m_data.end()) {
            value = data->second;
            return 0;
        }
        return -1;
    }

    void PacketTextParse::InsertOrAssign(const std::string& key, const std::string& value) {
        m_data.insert_or_assign(key, value);
    }

    void PacketTextParse::InsertOrAssign(const std::string& key, const int& value) {
        m_data.insert_or_assign(key, std::to_string(value));
    }

    void PacketTextParse::Serialize(std::string& data) {
        std::string tempData{};
        for (auto& data : m_data) {
            tempData += data.first;
            tempData += "|";
            tempData += data.second;
            tempData += "\n";
        }

        data = tempData;
    }
} // namespace Client