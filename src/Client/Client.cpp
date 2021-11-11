#include <thread>
#include <future>

#include "Client.h"
#include "../Utilities/Random.h"

namespace Client {
	Client::Client()
		: m_enetClient(nullptr)
	{
		m_enetClient = new ENetClient;
		if (m_enetClient && m_enetClient->Init() != 0) {
			m_enetClient = nullptr;
		}
	}

	Client::~Client() {
		if (m_enetClient) {
			delete m_enetClient;
			m_enetClient = nullptr;
		}
	}
} // namespace client