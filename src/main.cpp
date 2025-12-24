#include <Geode/Geode.hpp>
#include <Geode/Loader.hpp>

#include <Geode/modify/UILayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

using namespace geode::prelude;

class $modify(CBFIndUILayer, UILayer) {
	bool init(GJBaseGameLayer* layer) {
		if (!UILayer::init(layer)) return false;

		auto size = cocos2d::CCDirector::sharedDirector()->getWinSize();

		auto indicator = CCSprite::create("cbf.png"_spr);

		indicator->setScale(.2f);
		indicator->setPosition({ 0, size.height });
		indicator->setAnchorPoint({ 0.f, 1.f });
		indicator->setOpacity(50);

		indicator->setID("cbf_indicator"_spr);

		this->addChild(indicator);

		auto cbf = Loader::get()->getLoadedMod("syzzi.click_between_frames");
		bool isCBFOn = cbf && !cbf->getSettingValue<bool>("soft-toggle");
		indicator->setVisible(isCBFOn);

		listenForSettingChanges<bool>("soft-toggle", [indicator](bool val) {
			indicator->setVisible(!val);
		}, cbf);

		return true;
	}
};

class $modify(CBFIndEndLevelLayer, EndLevelLayer) {
	void customSetup() {
		EndLevelLayer::customSetup();

		auto cbf = Loader::get()->getLoadedMod("syzzi.click_between_frames");
		if (!cbf || cbf->getSettingValue<bool>("soft-toggle")) return;

		// CBF logo nearby the level complete text
		auto logo = CCSprite::create("cbf.png"_spr);

		logo->setScale(.3f);
		logo->setPosition({ 402.f, 225.f });
		logo->setRotation(-12.f);
		logo->setAnchorPoint({ .5f, .5f });
		logo->setZOrder(16);
		logo->setID("cbf_logo"_spr);

		// Edit the level complete label to say a custom message
		for (auto c : { "coin-1-background", "coin-2-background", "coin-3-background" }) {
			if (m_mainLayer->getChildByID(c)) return; // If there is no label, don't edit it.
		}

		std::string str = "CBF Detected Loser!";

		// tysm QOL for giving me a "base idea" as to what to do lol
		// https://github.com/TheSillyDoggo/GeodeMenu/blob/2d56276e907e61c2c85c1b6e3c5e87c96b92f39e/src/SafeMode/Hooks.cpp#L100-L109
		// (to the creator of QOL, I can change this if you want, but I need to find another method to do this)
		// (also I manually copied the code by hand, I almost never copy-paste)
		if (auto textArea = m_mainLayer->getChildByType<TextArea>(-1)) {
			textArea->setString(str);
			textArea->setScale(.6f);
		} else if (auto textLabel = m_mainLayer->getChildByType<CCLabelBMFont>(-1)) {
			textLabel->setString(str.c_str());
			textLabel->setScale(.6f);
		}

		m_mainLayer->addChild(logo);
	}
};