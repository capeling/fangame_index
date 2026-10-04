#include "Fangame.hpp"

#include <matjson.hpp>
#include <matjson/reflect.hpp>

using namespace geode::prelude;
namespace fs = std::filesystem;

namespace fi {

geode::Result<> Fangame::zip(const std::filesystem::path& output) {
    GEODE_UNWRAP_INTO(auto zip, file::Zip::create(output));

    GEODE_UNWRAP(zip.addFolder("songs"));
    GEODE_UNWRAP(zip.addFolder("levels"));
    GEODE_UNWRAP(zip.addFolder("assets"));

    matjson::Value fi = m_info;
    GEODE_UNWRAP(zip.add("fangame.json", fi.dump()));

    for (const auto& level : m_levels) {
        auto fileName = fmt::format("levels/{}.json", level.name);
        matjson::Value v = level;
        GEODE_UNWRAP(zip.add(fileName, v.dump()));
    }

    for (const auto& song : m_songs) {
        auto fileName = fmt::format("songs/{}.json", song.second.name);
        matjson::Value v = song.second;
        GEODE_UNWRAP(zip.add(fileName, v.dump()));
    }

    return Ok();
}

Result<> Fangame::forceFullLoad() {
    if (!m_lazyLoaded)
        return Ok();

    GEODE_UNWRAP(this->parseLevels());
    GEODE_UNWRAP(this->parseSongs());

    this->m_lazyLoaded = false;
    return Ok();
}

Result<std::shared_ptr<Fangame>> Fangame::from(const std::filesystem::path& path) {
    GEODE_UNWRAP_INTO(auto obj, Fangame::fromLazy(path));

    GEODE_UNWRAP(obj->parseLevels());
    GEODE_UNWRAP(obj->parseSongs());

    obj->m_lazyLoaded = false;

    return Ok(obj);
}

geode::Result<std::shared_ptr<Fangame>> Fangame::fromLazy(const std::filesystem::path& path) {
    if (!fs::exists(path))
        return Err("nonexistent dir");

    auto obj = std::make_shared<Fangame>();
    obj->m_path = path;
    obj->m_unzippedPath = path;

    GEODE_UNWRAP(obj->extract());
    GEODE_UNWRAP(obj->parseFangameJson());

    obj->m_lazyLoaded = true;

    return Ok(obj);
}

std::string Fangame::getAuthorString() {
    if (m_info.authors.empty())
        return "Unknown";
    if (m_info.authors.size() == 1)
        return m_info.authors[0];
    else if (m_info.authors.size() == 2)
        return fmt::format("{} & {}", m_info.authors[0], m_info.authors[1]);
    else
        return fmt::format("{} + {} more", m_info.authors[0], m_info.authors.size() - 1);

    return "?";
}

geode::Result<> Fangame::parseFangameJson() {
    if (!fs::exists(m_unzippedPath / "fangame.json"))
        return Err("fangame.json does not exist");

    GEODE_UNWRAP_INTO(auto json, file::readJson(m_unzippedPath / "fangame.json"));
    GEODE_UNWRAP_INTO(m_info, json.as<FangameInfo>());

    return Ok();
}


geode::Result<> Fangame::parseLevels() {
    auto levelsPath = m_unzippedPath / "levels";

    if (!fs::exists(levelsPath))
        return Err("levels directory does not exist");

    if (!fs::is_directory(levelsPath))
        return Err("levels is not a directory");

    for (const auto& f : fs::directory_iterator(levelsPath)) {
        GEODE_UNWRAP_INTO(auto json, file::readJson(f));
        GEODE_UNWRAP_INTO(auto level, json.as<Level>());
        m_levels.emplace_back(level);
    }

    return Ok();
}

geode::Result<> Fangame::parseSongs() {
    auto songsPath = m_unzippedPath / "songs";

    if (!fs::exists(songsPath))
        return Err("songs directory does not exist");

    if (!fs::is_directory(songsPath))
        return Err("songs is not a directory");

    for (const auto& f : fs::directory_iterator(songsPath)) {
        GEODE_UNWRAP_INTO(auto json, file::readJson(f));
        GEODE_UNWRAP_INTO(auto song, json.as<Song>());
        m_songs[song.name] = song;
    }

    return Ok();
}

// Taken from: https://github.com/geode-sdk/textureldr/blob/main/src/Pack.cpp
Result<> Fangame::extract() {
    if (fs::is_directory(m_path))
        return Ok();

    auto extension = string::pathToString(m_path.extension());
    if (extension != ".fangame")
        return Err("File is not .fangame!");

    auto extractPath = Mod::get()->getSaveDir() / "unzipped" / m_path.filename();
    (void) utils::file::createDirectoryAll(extractPath);

    auto datePath = extractPath / "modified-at";
    std::string currentHash = file::readString(datePath).unwrapOr("");

    std::error_code ec;
    auto modifiedDate = std::filesystem::last_write_time(m_path, ec);
    if (ec) {
        return Err("Unable get last_write_time: {}", ec.message());
    }
    auto modifiedCount = std::chrono::duration_cast<std::chrono::milliseconds>(modifiedDate.time_since_epoch());
    auto modifiedHash = std::to_string(modifiedCount.count());
    if (currentHash == modifiedHash) {
        m_unzippedPath = extractPath;
        return Ok();
    }
    log::debug("Hash mismatch detected, unzipping {}", m_path.filename());

    std::filesystem::remove_all(extractPath, ec);
    if (ec) {
        return Err("Unable to delete temp dir: {}", ec.message());
    }

    (void) utils::file::createDirectoryAll(extractPath);
    auto res = file::writeString(datePath, modifiedHash);
    if (!res) {
        log::warn("Failed to write modified date of extracted pack: {}", res.unwrapErr());
    }

    GEODE_UNWRAP_INTO(auto unzip, file::Unzip::create(m_path));
    GEODE_UNWRAP(unzip.extractAllTo(extractPath));

    m_unzippedPath = extractPath;

    return Ok();
}

}
