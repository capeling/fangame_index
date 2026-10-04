#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>

namespace fi {

struct HLevelSelectLayer : public geode::Modify<HLevelSelectLayer, LevelSelectLayer> {
    // bool init(int page);
};

}