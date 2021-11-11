#include <iostream>

#include "Peer.h"

namespace Client {
	Peer::Peer(ENetPeer* peer)
		: m_peer(peer) {}

	void Peer::SendPacket(eNetMessageType messageType, const std::string& data) {
		if (m_peer) {
			ENetPacket* packet = enet_packet_create(0, data.length() + 5, 1);
			*(eNetMessageType*)packet->data = messageType;
			memcpy(packet->data + 4, data.c_str(), data.length());

			if (enet_peer_send(m_peer, 0, packet) != 0) {
				enet_packet_destroy(packet);
			}
		}
		else {
			std::cout << "Bad peer" << std::endl;
		}
	}

	void Peer::SendPacketPacket(ENetPacket* packet) {
		if (m_peer) {
			ENetPacket* packetTwo = enet_packet_create(0, packet->dataLength, packet->flags);
			memcpy(packetTwo->data, packet->data, packet->dataLength);
            if (enet_peer_send(m_peer, 0, packetTwo) != 0) {
                enet_packet_destroy(packetTwo);
            }
			return;
		}
	}

    void Peer::SendPacketRaw(eNetMessageType messageType, GameUpdatePacket* gameUpdatePacket, size_t length, unsigned char* extendedData, enet_uint32 flags) {
        if (m_peer) {
            if (length > 0xf4240u) {
                std::cout << "Huge Packet Size " << length << std::endl;
                return;
            }

            ENetPacket* packet = enet_packet_create(0, length + 5, flags);
            *(eNetMessageType*)packet->data = messageType;
            memcpy(packet->data + 4, gameUpdatePacket, length);

            if (messageType == NET_MESSAGE_GAME_PACKET && (gameUpdatePacket->unk6 & 8) != 0) {
                memcpy(packet->data + length + 4, extendedData, gameUpdatePacket->unk16);
				*(uint32_t*)(packet->data + length + gameUpdatePacket->unk16 + 4) = 0x21402e40;
            }
			else {
				*(uint32_t*)(packet->data + length + 4) = 0x21402e40;
			}

            if (enet_peer_send(m_peer, 0, packet) != 0) {
                enet_packet_destroy(packet);
            }
        }
    }


} // namespace Client