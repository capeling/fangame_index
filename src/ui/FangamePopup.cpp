#include "FangamePopup.hpp"

namespace fi {

FangamePopup* FangamePopup::create(const std::shared_ptr<Fangame>& fangame) {
    auto ret = new FangamePopup();
    if (ret->init(fangame)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FangamePopup::init(const std::shared_ptr<Fangame>& fangame) {
    if (!Popup::init(400.f, 280.f))
        return false;

    m_fangame = fangame;
    this->setTitle(m_fangame->getName());

    return true;
}

}