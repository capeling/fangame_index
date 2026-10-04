#pragma once

#include <Geode/Geode.hpp>
#include <cue/ListNode.hpp>

#include "Fangame.hpp"

namespace fi {

class FangameCell;

class FangameListPopup : public geode::Popup {
public:
    static inline const cocos2d::CCSize POPUP_SIZE = {420.f, 280.f};
    static inline const cocos2d::CCSize LIST_SIZE = { 350.f, 200.f };

    static FangameListPopup* create();

protected:
    friend class FangameCell;

    bool init() override;

    cue::ListNode* m_list = nullptr;
};

}