#pragma once
#include <enet/enet.h>
#include <string>
#include <vector>
#include <memory>

// Packet Types
enum class PacketType : uint8_t {
    HELLO,
    INPUT, // Client -> Server
    STATE  // Server -> Client
};

#pragma pack(push, 1)
struct PacketHeader {
    PacketType type;
};

struct InputPacket {
    PacketHeader header;
    bool left, right, up, down;
};

struct EntityState {
    size_t id;
    float x, y;
};

struct StatePacket {
    PacketHeader header;
    uint32_t entity_count;
    // Followed by EntityState[entity_count]
};
#pragma pack(pop)

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    bool init_server(int port);
    bool init_client(const std::string& host, int port);
    void update();
    void cleanup();

    bool is_server() const { return server_host != nullptr; }
    bool is_client() const { return client_host != nullptr; }

    // Client side: Send input
    void send_input(bool left, bool right, bool up, bool down);
    
    // Server side: Broadcast state
    void broadcast_state(const std::vector<EntityState>& states);

    // Callbacks/Events (Simplified for now, we'll poll state directly)
    // For this phase, we'll expose a way to get latest remote inputs or states
    
    // Server: Get latest input from a client (Map client ID to input?)
    // For Phase 3 demo, we assume 1 player = 1 client. We need to map ENetPeer to Entity ID.
    // Let's keep it simple: Just expose a list of "Pending Inputs" and "Received States".

    struct ReceivedInput {
        size_t client_id; // Just using peer pointer hash or similar for now
        bool left, right, up, down;
    };
    std::vector<ReceivedInput> pop_inputs();

    std::vector<EntityState> latest_states;

private:
    ENetHost* server_host = nullptr;
    ENetHost* client_host = nullptr;
    ENetPeer* server_peer = nullptr;
    
    std::vector<ReceivedInput> input_queue;
};
