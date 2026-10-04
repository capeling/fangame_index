#include "FangameManager.hpp"

using namespace geode::prelude;
namespace fs = std::filesystem;

namespace fi {

void FangameManager::save() {
    
}

void FangameManager::load() {
}

Result<> FangameManager::loadFangames() {
    auto dir = Mod::get()->getConfigDir();
    auto iterator = fs::directory_iterator(dir);

    for (auto& f : iterator) {
        auto fg = Fangame::fromLazy(f);
        if (fg.isErr()) {
            log::error("Error loading: {}: {}", string::pathToString(dir.filename()), fg.err());
            continue;
        }

        auto fangame = fg.unwrap();

        // Fully load requested fangames, otherwise leave them lazy loaded
        if (std::ranges::contains(m_loadedFangameIDs, fangame->getID())) {
            GEODE_UNWRAP(fangame->forceFullLoad());
        }

        log::info("Loaded{} fangame: {}", fangame->isLazyLoaded() ? " (lazy)" : "", fangame->getID());
        m_fangames.insert({fangame->getID(), fangame});
    }

    return Ok();
}

Result<> FangameManager::refreshFangames() {
    m_fangames.clear();
    GEODE_UNWRAP(this->loadFangames());
    return Ok();
}

} // namespace fi