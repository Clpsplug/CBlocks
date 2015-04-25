//
//  MenuScene.cpp
//  CBlocks
//
//  Created by 有働 篤人 on 4/25/15.
//
//

#include "MenuScene.h"

USING_NS_CC;

MenuScene::MenuScene()
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

    return true;
    
}