//
//  MenuScene.cpp
//  CBlocks
//
//  Created by 有働 篤人 on 4/25/15.
//
//

#include "MenuScene.h"
#include "MainScene.h"

USING_NS_CC;

MenuScene::MenuScene()
: _onMenuTrans(false)
, _mainMenu(nullptr)
, _playMenu(nullptr)
, _menuNo(0)
{
    //
}

MenuScene::~MenuScene()
{
    //
}

Scene* MenuScene::createScene()
{
    
    auto scene = Scene::create();
    auto layer = MenuScene::create();
    
    scene->addChild(layer);
    return scene;
}

bool MenuScene::init(){
    
    if (!Layer::init()){
        return false;
    }
    
    auto backLbl = Label::createWithSystemFont("Back", "Marker Felt", 60);
    auto playLbl = Label::createWithSystemFont("PLAY!", "Marker Felt", 60);
    auto optionLbl = Label::createWithSystemFont("Option", "Marker Felt", 60);
    auto NormalLbl = Label::createWithSystemFont("Normal Mode", "Marker Felt", 60);
    auto HazardLbl = Label::createWithSystemFont("Hazard Mode", "Marker Felt", 60);

    auto backBtn = MenuItemLabel::create(backLbl, [this](Ref *sender){
        this->setMenuNo(0);
        this->setOnMenuTrans(true);
    });
    auto playBtn = MenuItemLabel::create(playLbl, [this](Ref *sender){
        this->setMenuNo(1);
        this->setOnMenuTrans(true);
    });
    auto optionBtn = MenuItemLabel::create(optionLbl, [this](Ref *sender){
        this->setMenuNo(2);
        this->setOnMenuTrans(true);
    });
    auto NormalBtn = MenuItemLabel::create(NormalLbl, [this](Ref *sender){
        this->unscheduleUpdate();
        auto scene = MainScene::createScene(0);
        auto sceneTr = TransitionFade::create(1.0f,scene);
        Director::getInstance()->replaceScene(sceneTr);
    });
    auto HazardBtn = MenuItemLabel::create(HazardLbl, [this](Ref *sender){
        this->unscheduleUpdate();
        auto scene = MainScene::createScene(1);
        auto sceneTr = TransitionFade::create(1.0f,scene);
        Director::getInstance()->replaceScene(sceneTr);
    });
    
    MenuItems mainMenuItems;
    mainMenuItems.clear();
    mainMenuItems.pushBack(playBtn);
    mainMenuItems.pushBack(optionBtn);

    MenuItems playMenuItems;
    playMenuItems.clear();
    playMenuItems.pushBack(NormalBtn);
    playMenuItems.pushBack(HazardBtn);
    playMenuItems.pushBack(backBtn);
    
    this->setMainMenuItems(mainMenuItems);
    this->setPlayMenuItems(playMenuItems);
    
    auto mainMenu = Menu::createWithArray(_mainMenuItems);
    mainMenu->alignItemsVerticallyWithPadding(30); // ボタンを縦に並べる
    this->setMainMenu(mainMenu);
    this->addChild(mainMenu);
    _mainMenu->setPosition(Director::getInstance()->getWinSize()/2);

    auto playMenu = Menu::createWithArray(_playMenuItems);
    playMenu->alignItemsVerticallyWithPadding(30); // ボタンを縦に並べる
    this->setPlayMenu(playMenu);
    this->addChild(playMenu);
    _playMenu->setPosition((Vec2)Director::getInstance()->getWinSize()/2 + Vec2(Director::getInstance()->getWinSize().width,0));
    
    this->scheduleUpdate();

    return true;
    
}

void MenuScene::onEnterTransitionDidFinish(){
    
}

void MenuScene::update(float dt){
    
    Vec2 winSize = Director::getInstance()->getWinSize();
    
    if (this->getOnMenuTrans() == true) {
        switch (this->getMenuNo()) {
            case 0:
                //Backで戻って来たときは、一旦メインメニュー以外のメニューを右へ画面の幅分動かして、
                //そのあと定位置に全員待機させる。
                //メインメニューは左からやってくる。
                this->getMainMenu()->runAction(EaseOut::create(MoveBy::create(0.5f, Vec2(winSize.x,0)), 2));
                //その他のメニュー
                this->getPlayMenu()->runAction(Sequence::create(EaseOut::create(MoveBy::create(0.5f, Vec2(winSize.x,0)), 2), MoveTo::create(0.0f, winSize / 2 + Vec2(winSize.x,0)), NULL));
                break;
            case 1:
                
                //Playボタンが押された。メインメニューは左へはけて、Playメニューが右からくる
                this->getMainMenu()->runAction(EaseOut::create(MoveBy::create(0.5f, Vec2(-winSize.x,0)), 2));
                this->getPlayMenu()->runAction(EaseOut::create(MoveBy::create(0.5f, Vec2(-winSize.x,0)), 2));
                break;
            default:
                break;
        }
        this->setOnMenuTrans(false);
    }
    
    
}