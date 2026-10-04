#pragma once

#include <Geode/Geode.hpp>

#include "Fangame.hpp"

namespace fi {

class FangamePopup : public geode::Popup {
public:
    static FangamePopup* create(const std::shared_ptr<Fangame>& fangame);

protected:
    bool init(const std::shared_ptr<Fangame>& fangame);

    std::shared_ptr<Fangame> m_fangame;
};

} // namespace fi