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

/// Number of blocks put horizontally
const int HORIZONTAL_COUNT = 6;
/// Number of blocks put vertically
const int VERTICAL_COUNT = 9;

MainScene::MainScene()
: _playField(NULL)
{
    
}

MainScene::~MainScene()
{
    CC_SAFE_RELEASE_NULL(_playField);
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
    
    //Get director
    auto director = Director::getInstance();
    
    //Get resolution size
    auto size = director->getWinSize();

    //Create background sprite
    auto backGround = Sprite::create("Back.png");
    
    //Set where to show the sprite; this time it fills the screen up
    backGround->setPosition(cocos2d::Vec2(size.width / 2.0, size.height / 2.0));
    
    // make a node, save it to _playField
    this->setPlayField((Sprite::create("playfield.png")));
    for (int x = 0; x < HORIZONTAL_COUNT; ++x) {
        for (int y = 0; y < VERTICAL_COUNT; ++y) {
            // create a block
            auto block = Blocks::create();
            // Set the position of block IN GRID
            block->setBlockPos(Vec2(x, y));
            // Add block into Stage
            this->addBlock(block);
        }
    }
    //描画位置設定！
    _playField->setPosition(cocos2d::Vec2(size.width / 2.0, size.height / 2.0));

    //Add this sprite to parent node
    this->addChild(backGround);
    this->addChild(_playField);

    return true;
}


void MainScene::addBlock(Blocks *block)
{
    // Add block into blocks list
    _blocks.pushBack(block);
    // Add block into _playField node
    _playField->addChild(block);
    // Adjust Position
    block->adjustPos();
}

Blocks* MainScene::getBlockAt(const cocos2d::Vec2& pos)
{
    for (auto& block : _blocks) {
        if (pos.equals(block->getBlockPos())) {
             return block;
        }
    }
    return nullptr;
}

Blocks* MainScene::getBlockAtByPixel(const cocos2d::Vec2& pixelPos)
{
     // Absolute pos to stage pos
     auto pfPosition = _playField->convertToNodeSpace(pixelPos);
     // stage pos to grid pos
     auto gridPosition = Blocks::pixelToGrid(pfPosition);
     return this->getBlockAt(gridPosition);
 }

//Refresh method
void MainScene::update(float dt)
{
    
}