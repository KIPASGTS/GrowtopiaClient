#pragma once
#include <vector>

namespace Utilities {
    namespace Utils {
        std::string GenerateRandomNumber(size_t length);
        std::string GenerateRandomHex(size_t length, bool uppercase = false);
        std::string GenerateRandomMac();
        int HashString(const char* data, int length);
        int GetDeviceHash();
        int GetDeviceSecondaryHash();
    } // namespace Utils
} // namespace Utilities