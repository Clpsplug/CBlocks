//
//  MainScene.cpp
//  ComboBlocks
//
//  Created by 有働 篤人 on 11/28/14.
//
//

#include <stdio.h>
#include "MainScene.h"

using namespace cocos2d;

MainScene::MainScene()
{
    
}

MainScene::~MainScene()
{
    
}

Scene* MainScene::createScene()
{
    
    auto scene = Scene::create();
    auto layer = MainScene::create();
    
    scene->addChild(layer);
    return scene;
}


bool MainScene::init()
{
    if (!Layer::init()){
        return false;
    }
    
    return true;
}


//Refresh method
void MainScene::update(float dt)
{
    
}