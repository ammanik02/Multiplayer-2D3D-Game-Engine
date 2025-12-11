#include "Config.h"
#include "Logger.h"

int main(int argc, char* argv[]) {
    // defaults
    std::string configPath = "config.json";
    bool is_server = false;

    // CLI Args
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--server" || arg == "server") is_server = true;
        if (arg == "--client" || arg == "client") is_server = false; // default anyway
        if (arg == "--config" && i + 1 < argc) {
            configPath = argv[++i];
        }
    }

    // Load Config
    json config = Config::load(configPath);
    // Fallback if load fails
    if (config.empty()) {
        Log::Info("Config not found or empty, using defaults.");
        config["window"]["title"] = "Default Game";
        config["window"]["width"] = 800;
        config["window"]["height"] = 600;
        config["network"]["port"] = 1234;
        config["initial_scene"] = "scenes/main.json"; // Default scene path
    }

    // Start Python Interpreter
    py::scoped_interpreter guard{};
    // Add paths to Python sys.path
    try {
        auto sys_path = py::module_::import("sys").attr("path");
        sys_path.attr("append")("./scripts"); // Standard scripts
        sys_path.attr("append")("./examples/demo_game/scripts"); // Demo game scripts
    } catch(...) {}

    Engine engine;
    if (!engine.init(config, is_server)) {
        return 1;
    }

    // Load Scene from Config Path
    std::string scenePath = "";
    if (config.contains("initial_scene")) {
        scenePath = config["initial_scene"];
    } else {
        // Fallback checks
        scenePath = "scenes/main.json";
    }
    
    // Load the scene file
    json sceneData = Config::load(scenePath);
    if (!sceneData.empty() && sceneData.contains("entities")) {
        Log::Info("Loading scene: " + scenePath);
        for (const auto& item : sceneData["entities"]) {
            size_t id = item.value("id", 0);
            float x = item.value("x", 0.0f);
            float y = item.value("y", 0.0f);
            float z = item.value("z", 0.0f);
            
            auto entity = std::make_shared<Entity>(id, x, y, z, 40, 40, 
                item.value("color", std::vector<int>{255,255,255})[0], 
                item.value("color", std::vector<int>{255,255,255})[1], 
                item.value("color", std::vector<int>{255,255,255})[2]); 
            
            if (item.contains("script_module") && item.contains("script_class")) {
                 entity->attach_script(item["script_module"], item["script_class"]);
            }
            
            engine.add_entity(entity);
        }
    } else {
        Log::Info("No scene loaded or scene file empty.");
    }

    Log::Info("Starting Game Loop...");
    engine.run();
    Log::Info("Game Loop Ended.");

    return 0;
}
