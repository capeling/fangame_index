#include "FangameCell.hpp"

#include "FangamePopup.hpp"
#include "manager/FangameManager.hpp"

#include <UIBuilder.hpp>

using namespace geode::prelude;

namespace fi {

FangameCell* FangameCell::create(FangameListPopup* popup, const std::shared_ptr<Fangame>& fangame) {
    auto ret = new FangameCell();
    if (ret->init(popup, fangame)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FangameCell::init(FangameListPopup* popup, const std::shared_ptr<Fangame>& fangame) {
    if (!CCLayerColor::init())
        return false;

    constexpr float HEIGHT = 50.f;
    constexpr float PADDING = 6.f;

    this->setContentSize({FangameListPopup::LIST_SIZE.width, HEIGHT});

    m_fangame = fangame;
    m_popup = popup;

    auto logoSize = CCSize{HEIGHT - PADDING * 3, HEIGHT - PADDING * 3};
    auto logo = Build<LazySprite>::create(logoSize, true)
        .id("logo")
        .parent(this)
        .anchorPoint(0.f, 0.5f)
        .move(PADDING * 2, 0.f)
        .centerY()
        .collect();

    logo->setLoadCallback([this, logo, logoSize](Result<> res) {
        if (res.isErr()) {
            logo->CCSprite::initWithFile("dialogIcon_018.png");
            logo->setOpacity(255 / 2);
        }
        limitNodeSize(logo, logoSize, 1.f, 0.1f);
        logo->setAnchorPoint({0.f, 0.5f});
    });
    logo->loadFromFile(m_fangame->getUnzippedPath() / "fangame.png");

    auto nameLabel = Build<Label>::create(m_fangame->getName(), "bigFont.fnt")
        .id("name")
        .anchorPoint(0.f, 0.5f)
        .collect();

    Label* authorLabel;

    auto authorWrapper = Build<Label>::create(m_fangame->getAuthorString(), "goldFont.fnt")
        .id("name")
        .anchorPoint(0.f, 0.5f)
        .posX(1.f)
        .store(authorLabel)
        .intoNewParent(CCNode::create())
        .collect();

    nameLabel->setLimitLabelWidth(this->getContentWidth() / 2, 0.5f, 0.1f);
    authorLabel->setLimitLabelWidth(this->getContentWidth() / 2, 0.5f, 0.1f);

    authorWrapper->setContentSize(authorLabel->getScaledContentSize());
    authorLabel->setPositionY(authorWrapper->getContentSize().height / 2.f);

    Build<CCMenu>::create()
        .parent(this)
        .anchorPoint(0.f, 0.5f)
        .children(nameLabel, authorWrapper)
        .layout(SimpleAxisLayout::create(Axis::Column)->setCrossAxisAlignment(CrossAxisAlignment::Start))
        .centerY()
        .posX(logo->getPositionX())
        .move(logoSize.width + PADDING, 1.f);

    CCMenuItemSpriteExtra* viewButton;
    geode::Button* viewButtonSprite;

    Build<ButtonSprite>::create("View", "bigFont.fnt", "GJ_button_01.png")
        .scale(0.8f)
        .intoMenuItem([](auto) {

        });

    return true;
}

}