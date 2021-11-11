cmake_minimum_required(VERSION 3.8)
project(GrowtopaiaClient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME} 
	Main.cpp 
	Client/Client.cpp
	Client/Client.h
	Client/ENetClient.cpp 
	Client/ENetClient.h
	Client/Peer.cpp 
	Client/Peer.h
	Packet/PacketCommon.h
	Packet/PacketDecoder.h
	Packet/PacketTextParse.cpp
	Packet/PacketTextParse.h
	Include/randutils.hpp
	Utilities/Random.h
	Utilities/Utils.cpp
	Utilities/Utils.h
	Utilities/Variant.cpp
	Utilities/Variant.h)

target_include_directories(${PROJECT_NAME} PRIVATE "../vendor/enet/include")
target_link_libraries(${PROJECT_NAME} PRIVATE enet winmm ws2_32)
