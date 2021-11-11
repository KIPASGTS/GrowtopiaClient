#include <iostream>
#include <chrono>
#include <thread>
#include <enet/enet.h>

#include "Client/Client.h"
#include "Client/ENetClient.h"

// Almost function here i reversed from Growtopia android library and from Proton SDK.
int main() {
	if (Client::ENetClient::OneTimeInit() != 0) {
		return EXIT_FAILURE;
	}

	Client::Client* client = new Client::Client;
	client->Execute();

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	return EXIT_SUCCESS;
}
