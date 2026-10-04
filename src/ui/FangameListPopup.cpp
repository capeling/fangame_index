#include "FangameListPopup.hpp"

#include <cue/ListNode.hpp>
#include <UIBuilder.hpp>

#include "manager/FangameManager.hpp"
#include "FangameCell.hpp"

using namespace geode::prelude;

namespace fi {

FangameListPopup* FangameListPopup::create() {
    auto ret = new FangameListPopup();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FangameListPopup::init() {
    if (!Popup::init(POPUP_SIZE, "GJ_square02.png"))
        return false;

    auto& fm = FangameManager::get();

    this->setTitle("Fangames", "bigFont.fnt", 0.6f);

    Build<cue::ListNode>::create(LIST_SIZE, ccColor4B{0x33, 0x44, 0x99, 255}, cue::ListBorderStyle::CommentsBlue)
        .parent(m_mainLayer)
        .center()
        .store(m_list)
        .id("fangame-list");

    // Colors taken from https://github.com/GlobedGD/globed2/blob/05fbd11756b81173075178adb8b4c8eefce40e43/src/ui/menu/GlobedMenuLayer.cpp#L428
    m_list->setJustify(cue::Justify::Center);
    m_list->setCellColors(
        ccColor4B{0x28, 0x35, 0x77, 255},
        ccColor4B{0x33, 0x44, 0x99, 255}
    );

    for (auto& fg : fm.getFangames()) {
        auto cell = FangameCell::create(this, fg);
        m_list->addCell(cell);
    }

    return true;
}

}