#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/LevelInfoLayer.hpp>
class $modify(LevelInfoLayerHook, LevelInfoLayer) {
  bool init(GJGameLevel* p0, bool p1) {
    if(!LevelInfoLayer::init(p0, p1)) {return false;}

    auto saveSongButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_orderUpBtn_001.png"),
			this,
			menu_selector(LevelInfoLayerHook::onMyButton)
		);

    auto menu = this->getChildByID("left-side-menu");
		menu->addChild(saveSongButton);

    saveSongButton->setID("save-song-button"_spr);

    menu->updateLayout();

    return true;
  }

  void onMyButton(CCObject*) {
    auto songFileName = this->m_level->getAudioFileName();
    Task<Result<std::filesystem::path>> pathToSaveToTask = utils::file::pick(
        utils::file::PickMode::SaveFile,
        utils::file::FilePickOptions {
          .filters = {
              { "MP3 Files", { "*.mp3" } }
          }
        }
      );
    EventListener<Task<Result<std::filesystem::path>>> pathToSaveToListener;
    pathToSaveToListener.bind([songFileName](Task<Result<std::filesystem::path>>::Event* event) {
      FLAlertLayer::create("Export Song", "Name: " + event->getValue()->value().filename().string(), "OK")->show();
    });
    pathToSaveToListener.setFilter(pathToSaveToTask);
  }
};

