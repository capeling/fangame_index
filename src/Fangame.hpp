#pragma once

#include <matjson.hpp>

namespace fi {

struct Song {
    std::string name;
    std::string author;
    std::string id;
};

struct Level {
    std::string name;
    std::string id;
    std::string author;
    std::string string;
    std::string song;
    GJDifficulty difficulty;
    int requiredCoins = 0;
    int timestamp = 0;
    int coinCount = 0;
    int starCount = 0;
};

struct FangameInfo {
    int version = 0;
    std::string name;
    std::string id;
    std::string description;
    std::vector<std::string> authors;
};

class Fangame {
public:
    static geode::Result<std::shared_ptr<Fangame>> from(const std::filesystem::path& path);
    static geode::Result<std::shared_ptr<Fangame>> fromLazy(const std::filesystem::path& path);

    geode::Result<> zip(const std::filesystem::path& output);
    geode::Result<> extract();

    geode::Result<> forceFullLoad();

    bool isLazyLoaded() const { return m_lazyLoaded; }

    std::string getID() const { return m_info.id; }
    std::string getName() const { return m_info.name; }

    std::string getAuthorString();

    std::filesystem::path getPath() const { return m_path; }
    std::filesystem::path getUnzippedPath() const { return m_unzippedPath; }

protected:
    geode::Result<> parseFangameJson();
    geode::Result<> parseLevels();
    geode::Result<> parseSongs();

    FangameInfo m_info;

    std::unordered_map<std::string, Song> m_songs;
    std::vector<Level> m_levels;

    std::filesystem::path m_path;
    std::filesystem::path m_unzippedPath;

    bool m_lazyLoaded = false;
};

} // namespace fi

template <>
struct matjson::Serialize<GJDifficulty> {
    static geode::Result<GJDifficulty> fromJson(const matjson::Value& value) {
        int res;
        GEODE_UNWRAP_INTO(res, value.asInt());
        return geode::Ok(static_cast<GJDifficulty>(res));
    }

    static matjson::Value toJson(const GJDifficulty& diff) {
        return matjson::Value(static_cast<int>(diff));
    }
};