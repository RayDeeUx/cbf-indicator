#include <Geode/Geode.hpp>
#include <Geode/Loader.hpp>

#include <Geode/modify/UILayer.hpp>

using namespace geode::prelude;

class $modify(CBFIndUILayer, UILayer) {
	bool init(GJBaseGameLayer* layer) {
		if (!UILayer::init(layer)) return false;

		auto size = cocos2d::CCDirector::sharedDirector()->getWinSize();

		auto test = CCLabelBMFont::create("cbf loser", "bigFont.fnt");

		test->setPosition({ 0, size.height });
		test->setAnchorPoint({ 0.f, 1.f });
		test->setOpacity(30);
		test->setScale(.5f);

		this->addChild(test);

		auto cbf = Loader::get()->getLoadedMod("syzzi.click_between_frames");
		bool isCBFOn = cbf && !cbf->getSettingValue<bool>("soft-toggle");
		test->setVisible(isCBFOn);

		listenForSettingChanges<bool>("soft-toggle", [test](bool val) {
			test->setVisible(!val);
		}, cbf);

		return true;
	}
};