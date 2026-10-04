#pragma once

#include <Geode/Geode.hpp>

#include "Fangame.hpp"
#include "FangameListPopup.hpp"

namespace fi {

class FangameCell : public cocos2d::CCLayerColor {
public:
    static FangameCell* create(FangameListPopup* popup, const std::shared_ptr<Fangame>& fangame);

protected:
    bool init(FangameListPopup* popup, const std::shared_ptr<Fangame>& fangame);

    FangameListPopup* m_popup;
    std::shared_ptr<Fangame> m_fangame;
};

}