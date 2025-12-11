#include "Network.h"
#include <iostream>
#include <cstring>

NetworkManager::NetworkManager() {
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
    }
}

NetworkManager::~NetworkManager() {
    cleanup();
    enet_deinitialize();
}

void NetworkManager::cleanup() {
    if (server_host) {
        enet_host_destroy(server_host);
        server_host = nullptr;
    }
    if (client_host) {
        enet_host_destroy(client_host);
        client_host = nullptr;
    }
}

bool NetworkManager::init_server(int port) {
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;

    server_host = enet_host_create(&address, 32, 2, 0, 0);
    if (!server_host) {
        std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
        return false;
    }
    return true;
}

bool NetworkManager::init_client(const std::string& host_ip, int port) {
    client_host = enet_host_create(NULL, 1, 2, 0, 0);
    if (!client_host) {
        std::cerr << "An error occurred while trying to create an ENet client host." << std::endl;
        return false;
    }

    ENetAddress address;
    enet_address_set_host(&address, host_ip.c_str());
    address.port = port;

    server_peer = enet_host_connect(client_host, &address, 2, 0);
    if (!server_peer) {
        std::cerr << "No available peers for initiating an ENet connection." << std::endl;
        return false;
    }
    return true;
}

void NetworkManager::update() {
    ENetHost* host = server_host ? server_host : client_host;
    if (!host) return;

    ENetEvent event;
    while (enet_host_service(host, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "A new client connected from " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
                // Store mapped ID?
                event.peer->data = (void*)(size_t)event.peer->connectID; // Simple ID
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                if (event.packet->dataLength >= sizeof(PacketHeader)) {
                    PacketHeader* header = (PacketHeader*)event.packet->data;
                     if (header->type == PacketType::INPUT && server_host) {
                         if (event.packet->dataLength >= sizeof(InputPacket)) {
                             InputPacket* pkt = (InputPacket*)event.packet->data;
                             ReceivedInput input;
                             input.client_id = (size_t)event.peer->data; // Using data field we set on connect
                             if (input.client_id == 0) {
                                 // Assign ID if not set? Or use connectID
                                 // Let's use the peer pointer as ID for now to be unique
                                 input.client_id = (size_t)event.peer;
                             }
                             input.left = pkt->left;
                             input.right = pkt->right;
                             input.up = pkt->up;
                             input.down = pkt->down;
                             input_queue.push_back(input);
                         }
                    } else if (header->type == PacketType::STATE && client_host) {
                        if (event.packet->dataLength >= sizeof(StatePacket)) {
                            StatePacket* pkt = (StatePacket*)event.packet->data;
                            size_t count = pkt->entity_count;
                            if (event.packet->dataLength >= sizeof(StatePacket) + count * sizeof(EntityState)) {
                                EntityState* states = (EntityState*)(event.packet->data + sizeof(StatePacket));
                                latest_states.clear();
                                for(size_t i=0; i<count; ++i) {
                                    latest_states.push_back(states[i]);
                                }
                            }
                        }
                    }
                }
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                 std::cout << "Client disconnected." << std::endl;
                 break;
        }
    }
}

void NetworkManager::send_input(bool left, bool right, bool up, bool down) {
    if (!client_host || !server_peer) return;

    InputPacket pkt;
    pkt.header.type = PacketType::INPUT;
    pkt.left = left;
    pkt.right = right;
    pkt.up = up;
    pkt.down = down;

    ENetPacket* packet = enet_packet_create(&pkt, sizeof(InputPacket), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(server_peer, 0, packet);
}

void NetworkManager::broadcast_state(const std::vector<EntityState>& states) {
    if (!server_host) return;

    size_t payload_size = sizeof(StatePacket) + states.size() * sizeof(EntityState);
    std::vector<uint8_t> buffer(payload_size);
    
    StatePacket* header = (StatePacket*)buffer.data();
    header->header.type = PacketType::STATE;
    header->entity_count = (uint32_t)states.size();

    EntityState* data_ptr = (EntityState*)(buffer.data() + sizeof(StatePacket));
    std::memcpy(data_ptr, states.data(), states.size() * sizeof(EntityState));

    ENetPacket* packet = enet_packet_create(buffer.data(), payload_size, ENET_PACKET_FLAG_UNSEQUENCED);
    enet_host_broadcast(server_host, 0, packet);
}

std::vector<NetworkManager::ReceivedInput> NetworkManager::pop_inputs() {
    std::vector<ReceivedInput> inputs = input_queue;
    input_queue.clear();
    return inputs;
}
