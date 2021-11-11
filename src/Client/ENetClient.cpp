#include <iostream>

#include "ENetClient.h"
#include "../Packet/PacketDecoder.h"
#include "../Packet/PacketTextParse.h"
#include "../Utilities/Utils.h"
#include "../Utilities/Variant.h"

namespace Client {
	ENetClient::ENetClient()
		: m_uniqueId("dead") ,
		m_client(nullptr)
	{
		static int currentId = 0;
		static const char* hex = "0123456789ABCDEF";

		currentId++;
		int len = sizeof(currentId);
		m_uniqueId.resize(len);

		for (int i = 0; i < len; ++i) {
			m_uniqueId[i] = hex[(currentId >> (i << 2)) & 0x0f];
		}
	}

	ENetClient::~ENetClient() {
		m_uniqueId = "dead";
		m_client = nullptr;
		Kill();
		enet_deinitialize();
	}

	void ENetClient::Kill() {
		std::string data = "action|quit";
		m_peer->SendPacket(NET_MESSAGE_GAME_MESSAGE, data);
	}

	int ENetClient::OneTimeInit() {
		if (enet_initialize() != 0) {
			std::cout << "An error occurred while initializing ENet." << std::endl;
			return -1;
		}
		return 0;
	}

	int ENetClient::Init() {
		if (m_client) {
			enet_host_destroy(m_client);
			m_client = nullptr;
		}

		m_client = enet_host_create(NULL, 1, 2, 0, 0);
		if (m_client == NULL) {
			std::cout << "An error occurred while trying to create an ENet server host." << std::endl;
			return -1;
		}

		m_client->usingNewPacket = true;
		m_client->checksum = enet_crc32;
		if (enet_host_compress_with_range_coder(m_client) != 0) {
			std::cout << "An error occurred while trying compressing ENet server host with range coder." << std::endl;
			return -1;
		}

		ENetAddress address;
		enet_address_set_host(&address, "213.179.209.168");
		address.port = 17201;

		ENetPeer* peer = enet_host_connect(m_client, &address, 2, 0);
		if (m_peer == NULL) {
			std::cout << "No available peers for initiating an ENet connection." << std::endl;
			return -1;
		}
		else {
			m_peer = new Peer(peer);
		}

		enet_host_flush(m_client);
		return 0;
	}

	void ENetClient::DoReconnect() {
		Init();
	}

	void ENetClient::Disconnect() {
		if (m_peer) {
			enet_peer_disconnect(m_peer->GetPeer(), 0);
			m_peer = nullptr;
		}

		DoReconnect();
	}

	void ENetClient::Update() {
		while (true) {
			ENetEvent event;
			while (enet_host_service(m_client, &event, 1000) > 0) {
				switch (event.type) {
				case ENET_EVENT_TYPE_NONE:
					break;
				case ENET_EVENT_TYPE_CONNECT:
					std::cout << "Connected to growtopia server." << std::endl;
					event.peer->data = nullptr;
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					std::cout << "Disconnected from growtopia server trying to reconnecting.." << std::endl;
					enet_peer_reset(m_peer->GetPeer());
					Init();
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					ProcessPacket(event.packet);
					enet_packet_destroy(event.packet);
					break;
				default:
					break;
				}
			}
		}
	}

	void ENetClient::ProcessPacket(ENetPacket* packet) {
		int messageType = PacketDecoder::GetMessageTypeFromPacket(packet);
		std::cout << "Type: " << messageType << std::endl;
		switch (messageType) {
		case NET_MESSAGE_SERVER_HELLO: {
			PacketTextParse packetTextParse; 
			packetTextParse.InsertOrAssign("tankIDName", "LynxzTopia88");
			packetTextParse.InsertOrAssign("tankIDPass", "adhika");
			packetTextParse.InsertOrAssign("requestedName", "LaughCry"); // LaughCry, SmileZero 
			packetTextParse.InsertOrAssign("f", "0"); // Swear filter
			packetTextParse.InsertOrAssign("protocol", "147");
			packetTextParse.InsertOrAssign("game_version", "3.74");
			packetTextParse.InsertOrAssign("fz", "11535184");
			packetTextParse.InsertOrAssign("lmode", "0");
			packetTextParse.InsertOrAssign("cbits", "0");
			packetTextParse.InsertOrAssign("hash2", Utilities::Utils::GetDeviceSecondaryHash());
			packetTextParse.InsertOrAssign("meta", "ni.com");
			packetTextParse.InsertOrAssign("rid", Utilities::Utils::GenerateRandomHex(32, true));
			packetTextParse.InsertOrAssign("platformID", "0");
			packetTextParse.InsertOrAssign("deviceVersion", "0");
			packetTextParse.InsertOrAssign("country", "us");
			packetTextParse.InsertOrAssign("hash", Utilities::Utils::GetDeviceHash());
			packetTextParse.InsertOrAssign("reconnect", "1");
			packetTextParse.InsertOrAssign("mac", Utilities::Utils::GenerateRandomMac());
			packetTextParse.InsertOrAssign("wk", Utilities::Utils::GenerateRandomHex(32, true));
			packetTextParse.InsertOrAssign("fhash", "-716928004");
			packetTextParse.InsertOrAssign("GDPR", "3");
			packetTextParse.InsertOrAssign("player_age", "22");
			packetTextParse.InsertOrAssign("category", "_16");
			packetTextParse.InsertOrAssign("zf", "-311429722");

			std::string data{};
			packetTextParse.Serialize(data);
			m_peer->SendPacket(NET_MESSAGE_GENERIC_TEXT, data);

			std::cout << data << std::endl;
			break;
		}
		case NET_MESSAGE_GAME_MESSAGE:
			std::cout << "Data: " << PacketDecoder::GetTextPointerFromPacket(packet) << std::endl;
			break;
		case NET_MESSAGE_GAME_PACKET: {
			GameUpdatePacket* gameUpdatePacket = (GameUpdatePacket*)PacketDecoder::GetStructPointerFromTankPacket(packet);
			if (gameUpdatePacket) {
				ProcessTankUpdatePacket(gameUpdatePacket);
			}
			else {
				std::cout << "Got bad tank packet" << std::endl;
			}
			break;
		}
		case NET_MESSAGE_ERROR:
			break;
		case NET_MESSAGE_TRACK:
			std::cout << "Data: " << PacketDecoder::GetTextPointerFromPacket(packet) << std::endl;
			break;
		case NET_MESSAGE_CLIENT_LOG_REQUEST:
			std::cout << "NET_MESSAGE_CLIENT_LOG_REQUEST" << std::endl;
			break;
		default:
			std::cout << "Got unknown packet type : " << messageType << std::endl;
			break;
		}

		enet_host_flush(m_client);
	}

	void ENetClient::ProcessTankUpdatePacket(GameUpdatePacket* gameUpdatePacket) {
		std::cout << "gameUpdatePacket type: " << (int)gameUpdatePacket->packetType << std::endl;

		uint8_t* ExtendedDataPointerFromTankPacket = (uint8_t*)PacketDecoder::GetExtendedDataPointerFromTankPacket(gameUpdatePacket);
		switch ((int)gameUpdatePacket->packetType) {
		case PACKET_CALL_FUNCTION:
			VariantList variantList{};
			if (variantList.SerializeFromMem(ExtendedDataPointerFromTankPacket, gameUpdatePacket->unk16, 0)) {
				std::cout << variantList.Get(0).GetString() << std::endl;
				// TODO
			}
			else {
				std::cout << "Error reading function packet, ignoring" << std::endl;
			}
			break;
		}
	}
} // namespace Client