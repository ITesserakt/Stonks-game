#include "GameLoading.h"

void saveGameData(WorldState &state) {
    // Saving World state
    std::ofstream out(savePath);
    jsoncons::encode_json_pretty(state, out);
    out.close();

    // Saving Statistic
    out.open(Stat::pathForSave);
    jsoncons::json j;
    // Connecting statistic from counter and Items
    j["counter"] = Stat::Counter();
    j["items"] = Stat::ItemStat();
    jsoncons::encode_json_pretty(j, out);
}

WorldState loadWorldState() {
    std::ifstream in{savePath};

    try {
        return jsoncons::decode_json<WorldState>(in);
    } catch (...) {
        return WorldState{Config::current().botsAmount, Config::current().debug};
    }
}

void loadStatistic() {
    std::ifstream in(Stat::pathForSave);
    try {
        jsoncons::json j = jsoncons::json::parse(in);
        Stat::Counter() = j["counter"].as<Stat::Counter>();
        Stat::ItemStat() = j["items"].as<Stat::ItemStat>();
    } catch (...) {
        // We are not loading Statistic from previous file
    }
}
