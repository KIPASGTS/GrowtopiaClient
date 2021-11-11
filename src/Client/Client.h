#pragma once
#include <enet/enet.h>

#include "ENetClient.h"

namespace Client {
    class Client {
    public:
        Client();
        ~Client();

        void Execute() {
            if (m_enetClient) {
                m_enetClient->Execute();
            }
        }

    private:
        ENetClient* m_enetClient;
    };
} // namespace Client