#include <Geode/Geode.hpp>
#include <filesystem>

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

  struct Fields {
    Task<Result<std::filesystem::path>> pathToSaveToTask; 
    EventListener<Task<Result<std::filesystem::path>>> pathToSaveToListener;
  };

  void onMyButton(CCObject*) {
    std::filesystem::path songFileName = this->m_level->getAudioFileName();
    m_fields->pathToSaveToTask = utils::file::pick(
        utils::file::PickMode::SaveFile,
        utils::file::FilePickOptions {
          .defaultPath = this->m_songWidget->m_songLabel->getString() + songFileName.extension().string()
        }
      );

    m_fields->pathToSaveToListener.bind([songFileName](Task<Result<std::filesystem::path>>::Event* event) {
        if(!event->getValue()->isOk()) return;
        try {
          std::filesystem::copy(songFileName, event->getValue()->value());
          FLAlertLayer::create("Done!", "Successfully exported song", "OK")->show();
        } catch (const std::filesystem::filesystem_error& e) {
          FLAlertLayer::create("Error", "Error while exporting song", "OK")->show();
        }
    });

    m_fields->pathToSaveToListener.setFilter(m_fields->pathToSaveToTask);
  }
};

