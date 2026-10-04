#pragma once

#include <Geode/Geode.hpp>

#include "util/Singleton.hpp"
#include "Fangame.hpp"

namespace fi {

enum class ModState {
    Unloaded = 0,
    ReloadWanted,
    Loaded,
    Editor
};

class FangameManager : public util::Singleton<FangameManager> {
public:
    geode::Result<> loadFangames();
    geode::Result<> refreshFangames();

    void save();
    void load();

    std::vector<std::shared_ptr<Fangame>> getFangames() { return geode::utils::map::values(m_fangames); }

protected:
    ModState m_state;

    // Will be the wanted IDs if m_state is ReloadWanted
    std::vector<std::string> m_loadedFangameIDs;
    geode::utils::StringMap<std::shared_ptr<Fangame>> m_fangames;
};

} // namespace fi