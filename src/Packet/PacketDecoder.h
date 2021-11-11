#pragma once
#include <iostream>
#include <string>
#include <enet/enet.h>

#include "PacketCommon.h"

namespace Client {
	namespace PacketDecoder {
		inline int GetMessageTypeFromPacket(ENetPacket* packet) {
			if (packet->dataLength > 3) {
				return *(packet->data);
			}
			
			std::cout << "Bad packet length, ignoring message" << std::endl;
			return 0;
		}

		inline char* GetTextPointerFromPacket(ENetPacket* packet) {
			memset(packet->data + packet->dataLength - 1, 0, 1);
			return (char*)(packet->data + 4);
		}

		inline char* GetStructPointerFromPacket(ENetPacket* packet, int a2) {
			if (packet->dataLength >= a2 + 4) {
				return (char*)(packet->data + 4);
			}
			return 0;
		}

		inline GameUpdatePacket* GetStructPointerFromTankPacket(ENetPacket* packet) {
			if (packet->dataLength >= sizeof(GameUpdatePacket)) {
				GameUpdatePacket* gameUpdatePacket = (GameUpdatePacket*)(packet->data + 4);
				if ((gameUpdatePacket->unk6 & 8) == 0) {
					gameUpdatePacket->unk16 = 0;
				}
				else {
					if (packet->dataLength < gameUpdatePacket->unk16 + sizeof(GameUpdatePacket)) {
						std::cout << "Packet too small for extended packet to be valid" << std::endl;
						std::cout << "Sizeof float is 4.  TankUpdatePacket size: 56" << std::endl;
						return nullptr;
					}
				}

				return gameUpdatePacket;
			}
			return nullptr;
		}

		inline uint8_t* GetExtendedDataPointerFromTankPacket(GameUpdatePacket* gameUpdatePacket) {
			return reinterpret_cast<uint8_t*>(&gameUpdatePacket->unk17);
		}
	} // namespace PacketDecodee
} // namespace Client