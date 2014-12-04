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
, _curBlock(nullptr)
{
    
}

MainScene::~MainScene()
{
    CC_SAFE_RELEASE_NULL(_playField);
    CC_SAFE_RELEASE_NULL(_curBlock);
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
    
    
    
    // Create a touch event listener
    auto listener = EventListenerTouchOneByOne::create();
    // When touched
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
            auto position = touch->getLocation();
            // Get the block under the finger...
            auto block = this->getBlockAtByPixel(position);
        
            // And make it selected
            this->setCurBlock(block);
            return true;
    };
    // And when dragged...
    listener->onTouchMoved = [this](Touch* touch, Event* event) {
            // get the block in the destination
            auto nextBlock = this->getBlockAtByPixel(touch->getLocation());
        
            // if both selected block and the block in the destination exists, and are different
            if (_curBlock != nullptr &&
                nextBlock != nullptr &&
                _curBlock != nextBlock) {
                
                // and if those blocks are standstill...
                if (_curBlock->isStill() && nextBlock->isStill()) {
                    auto cp = _curBlock->getBlockPos();
                    auto np = nextBlock->getBlockPos();
                    
                    // and finally, if those are neighboring up/downwards or right/left side...
                    // that is the distance between them squared is equals to 1...
                    if (cp.distanceSquared(np) == 1) {
                        // We'll swap the block
                        this->swapBlocks(_curBlock, nextBlock);
                        
                        // And unselect the block
                        this->setCurBlock(nullptr);
                    }
                }
            }
    };
    // If the finger is released (or swapping canceled...)
    listener->onTouchEnded = listener->onTouchCancelled = [this](Touch* touch, Event* event) {
            // Unselect the block
            this->setCurBlock(nullptr);
    };
    // Add those event listener
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    

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

void MainScene::moveBlock(Blocks *block, const cocos2d::Vec2& blockPos)
{
    // クッキーのグリッド座標を設定
    block->setBlockPos(blockPos);
    // クッキーの表示位置を調整する
    block->adjustPos();
}

bool MainScene::swapBlocks(Blocks *block0, Blocks *block1)
{
    // get the position in PIXEL
    auto position0 = block0->getPosition();
    auto position1 = block1->getPosition();
    
    // also get the position in GRID
    auto blockPosition0 = block0->getBlockPos();
    auto blockPosition1 = block1->getBlockPos();
    
    // make them as being swapped
    block0->setState(Blocks::State::SWAPPED);
    block1->setState(Blocks::State::SWAPPED);
    
    /** function to add a motion
     *  @param block block to move
     *  @param toPosition destination in PIXEL
     *  @param toBlockPosition destination in GRID
     */
    auto addMoveAnimation = [this](Blocks *block, Vec2 toPosition, Vec2 toBlockPosition)
    {
        // time it takes to move
        const auto duration = 0.1;
        auto fromPosition = block->getPosition();
        
        block->runAction(Sequence::create(MoveTo::create(duration, toPosition),
                                           CallFuncN::create([=](Node *node) {
            auto block = dynamic_cast<Blocks *>(node);
            // move the block here
            this->moveBlock(block, toBlockPosition);
            // make it as standstill
            block->setState(Blocks::State::STANDSTILL);
        }), NULL));
    };
    
    // add the motions
    addMoveAnimation(block0, position1, blockPosition1);
    addMoveAnimation(block1, position0, blockPosition0);
    return true;
}

//Refresh method
void MainScene::update(float dt)
{
    
}