#include <string>

#include "Utils.h"
#include "Random.h"

namespace Utilities {
    namespace Utils {
        std::string GenerateRandomNumber(size_t length) {
            std::string string{};
            while (length-- > 0) {
                string += std::to_string(Utilities::Random::RandomGen<int>(0, 9));
            }
            return string;
        }

        std::string GenerateRandomHex(size_t length, bool uppercase) {
            static const char* hex = "0123456789abcdef";

            std::string string{};
            while (length-- > 0) {
                string += hex[Utilities::Random::RandomGen<int>(0, 15) & 0x0f];
            }

            if (uppercase) {
                for (auto& c : string) {
                    c = std::toupper(c);
                }
            }

            return string;
        }

        std::string GenerateRandomMac() {
            static const char* hex = "0123456789abcdef";

            std::string mac{};
            for (int i = 0; i < 6; i++) {
                mac += hex[Utilities::Random::RandomGen<int>(0, 15) & 0x0f];
                mac += hex[Utilities::Random::RandomGen<int>(0, 15) & 0x0f];
                if (i != 5) {
                    mac += ":";
                }
            }
            return mac;
        }

        int HashString(const char* data, int length) {
            int hash = 0x55555555;
            if (data) {
                if (length >= 1) {
                    while (length) {
                        hash = (hash >> 27) + (hash << 5) + *data++;
                        length--;
                    }
                }
                else {
                    while (*data) {
                        hash = (hash >> 27) + (hash << 5) + *data++;
                    }
                }
            }
            return hash;
        }

        int GetDeviceHash() {
            std::string deviceId = GenerateRandomNumber(8);
            if (!deviceId.empty()) {
                deviceId.append("RT");
                return HashString(deviceId.c_str(), 0);
            }
            return 0;
        }

        int GetDeviceSecondaryHash() {
            std::string mac = GenerateRandomMac();
            if (!mac.empty()) {
                mac.append("RT");
                return HashString(mac.c_str(), 0);
            }
            return 0;
        }
    } // namespace Utils
} // namespace Utilities