//
//  ResultScene.cpp
//  CBlocks
//
//  Created by 有働 篤人 on 9/3/15.
//
//

#include "ResultScene.h"

USING_NS_CC;

ResultScene::ResultScene()
{
    
}

ResultScene::~ResultScene()
{
    
}

bool ResultScene::init()
{
    if (!Layer::init()){
        return false;
    }
    return true;
}

Scene* ResultScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ResultScene::create();
    
    scene->addChild(layer);
    return scene;
}

void ResultScene::update(float dt)
{
    
}

void ResultScene::onEnterTransitionDidFinish()
{
    
}