#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "manager/FangameManager.hpp"
#include "ui/FangameListPopup.hpp"

using namespace geode::prelude;

class $modify(MenuLayer) {
	bool init() {
		if (!MenuLayer::init())
			return false;

		auto btn = Button::createWithSprite("dialogIcon_002.png", [](Button* sender) {
			fi::FangameListPopup::create()->show();
		});
		btn->setZOrder(50);
		btn->setPosition(50, 100);
		this->addChild(btn);

		return true;
	}
};

$on_mod(Loaded) {
	auto& fgm = fi::FangameManager::get();

	auto v = fgm.loadFangames();
	if (v.isErr()) {
		log::error("Error loading fangames: {}", v.err());
	}
}