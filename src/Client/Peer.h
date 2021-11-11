#pragma once
#include <string>
#include <enet/enet.h>

#include "../Packet/PacketCommon.h"

namespace Client {
	class Peer {
	public:
		Peer(ENetPeer* peer);
		~Peer() {};

		ENetPeer* GetPeer() { return m_peer; };

		void SendPacket(eNetMessageType messageType, const std::string& data);
		void SendPacketPacket(ENetPacket* packet);
		void SendPacketRaw(eNetMessageType messageType, GameUpdatePacket* gameUpdatePacket, size_t length, unsigned char* extendedData, enet_uint32 flags);

	private:
		ENetPeer* m_peer;
	};
}