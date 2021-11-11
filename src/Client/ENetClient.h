#pragma once
#include <cstdint>
#include <thread>
#include <enet/enet.h>

#include "Peer.h"
#include "../Packet/PacketCommon.h"

namespace Client {
	class ENetClient {
	public:
		ENetClient();
		~ENetClient();

		void Kill();
		static int OneTimeInit();
		int Init();
		void DoReconnect();
		void Disconnect();
		void Update();
		void ProcessPacket(ENetPacket* packet);
		void ProcessTankUpdatePacket(GameUpdatePacket* gameUpdatePacket);

		void Execute() {
			std::thread thread(&ENetClient::Update, this);
			m_thread = std::move(thread);
		}

	private:
		std::thread m_thread;
		std::string m_uniqueId;
		ENetHost* m_client;
		Peer* m_peer;
	};
} // namespace Client