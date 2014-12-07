//
//  MainScene.cpp
//  ComboBlocks
//
//  Created by 有働 篤人 on 11/28/14.
//
//

#include <stdio.h>
#include "MainScene.h"
#include "main.h"

using namespace cocos2d;

/// Number of blocks put horizontally
const int HORIZONTAL_COUNT = 6;
/// Number of blocks put vertically
const int VERTICAL_COUNT = 9;

MainScene::MainScene()
: _playField(NULL)
, _curBlock(nullptr)
, _score(0)
, _aniScore(0)
, _combo(0)
, _simCount(0)
, _time(0)
, _comboLevel(0.0f)
, _curComboLevel(0.0f)
, _scoreLabel(nullptr)
, _timeLabel(nullptr)
, _cueSheet(nullptr)
{
    // Initialize ADX2
    CriAtomExStandardVoicePoolConfig vp_config;
    criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&vp_config);
    // Maximum voices
    vp_config.num_voices = 8;
    // Enable Streaming
    vp_config.player_config.streaming_flag = CRI_TRUE;
    // Maximum sampling rate
    vp_config.player_config.max_sampling_rate = 48000 << 1;
   
    CriAtomExPlayerConfig pf_config;
    criAtomExPlayer_SetDefaultConfig(&pf_config);
    // Maximum number of file paths to be held
    pf_config.max_path_strings = 1;
    // Maximum length of a file path
    pf_config.max_path = 256;
    
    // ADX2を有効にする
    ADX2::Manager::initialize(pf_config, vp_config);
}

MainScene::~MainScene()
{
    CC_SAFE_RELEASE_NULL(_playField);
    CC_SAFE_RELEASE_NULL(_curBlock);
    CC_SAFE_RELEASE_NULL(_scoreLabel);
    CC_SAFE_RELEASE_NULL(_timeLabel);
    CC_SAFE_RELEASE_NULL(_cueSheet);
    
    // Finish ADX2
    ADX2::Manager::finalize();
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

    //Create a background sprite
    auto backGround = Sprite::create("Back.png");
    
    //Set where to show the sprite; this time it fills the screen up
    backGround->setPosition(cocos2d::Vec2(size.width / 2.0, size.height / 2.0));
    
    // Create infobox sprite(s)
    auto scorePanel = Sprite::create("ScorePanel.png");
    auto timePanel = Sprite::create("TimePanel.png");
    
    //Set where to show those sprites...
    scorePanel->setPosition(cocos2d::Vec2(scorePanel->getContentSize().width / 2 + 10.0f, -scorePanel->getContentSize().height/2 + size.height - 10.0f));
    timePanel->setPosition(cocos2d::Vec2(size.width - timePanel->getContentSize().width / 2 - 10.0f, -timePanel->getContentSize().height / 2 + size.height - 10.0f));
    
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

    //Add those sprites to parent node
    this->addChild(backGround);
    this->addChild(_playField);
    this->addChild(scorePanel);
    this->addChild(timePanel);
    
    // Create Labels for actual info
    // Score Label
    auto label = Label::createWithCharMap("digits.png", 17, 17, '0');
    this->addChild(label);
    label->setScaleX(0.67f);
    label->setPosition(cocos2d::Vec2(scorePanel->getContentSize().width + 10.0f - 7.0f, -scorePanel->getContentSize().height/2 + size.height - 32.0f));
    label->setAnchorPoint(cocos2d::Vec2(1.0f, 0.0f));
    this->setScoreLabel(label);
    
    auto timeLabel = Label::createWithCharMap("digits.png", 17, 17, '0');
    this->addChild(timeLabel);
    timeLabel->setPosition(cocos2d::Vec2(size.width - timePanel->getContentSize().width / 2 + 52.0f, -timePanel->getContentSize().height/2 + size.height - 32.0f));
    timeLabel->setAnchorPoint(cocos2d::Vec2(1.0f, 0.0f));
    this->setTimeLabel(timeLabel);

    
    
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
    
    // Load cuesheet(s)
    auto cueSheet = ADX2::CueSheet::create("adx2/CBlocks.acf",
                                           "adx2/main.acb");
    this->setCueSheet(cueSheet);
    
    _cueSheet->setAisacById(2, 0.0f);
    _cueSheet->updateAll();
    this->scheduleUpdate();
    return true;
}

void MainScene::onEnterTransitionDidFinish(){
    // メインBGMを鳴らす
    _cueSheet->playCueByID(CRI_MAIN_BGM);
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
    // Set the position IN GRID
    block->setBlockPos(blockPos);
    // Adjust the position!
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

BlockVct MainScene::checkNeighboringBlocks(Blocks *block){
    
    BlockVct deletion;
    
    auto pos = block->getBlockPos();
    //Check 2 blocks that is neiboring the block in question
    auto up0 = this->getBlockAt(cocos2d::Vec2(pos.x, pos.y+1));
    auto up1 = this->getBlockAt(Vec2(pos.x, pos.y+2));
    auto down0 = this->getBlockAt(cocos2d::Vec2(pos.x, pos.y-1));
    auto down1 = this->getBlockAt(Vec2(pos.x, pos.y-2));
    auto right0 = this->getBlockAt(cocos2d::Vec2(pos.x+1, pos.y));
    auto right1 =  this->getBlockAt(Vec2(pos.x+2, pos.y));
    auto left0 = this->getBlockAt(cocos2d::Vec2(pos.x-1, pos.y));
    auto left1 = this->getBlockAt(Vec2(pos.x-2, pos.y));
    
    //Matching means instant deletion!!
    if (up0 && up1 && up0->getBlockSort() == block->getBlockSort() && up1->getBlockSort() == block->getBlockSort()){
        //NOT IF THEY ARE MOVING
        if (up0->isStill() && up1->isStill()){
            deletion.pushBack(block);
            deletion.pushBack(up0);
            deletion.pushBack(up1);
        }
    }
    if (down0 && down1 && down0->getBlockSort() == block->getBlockSort() && down1->getBlockSort() == block->getBlockSort()){
        if (down0->isStill() && down1->isStill()){
            deletion.pushBack(block);
            deletion.pushBack(down0);
            deletion.pushBack(down1);
        }
    }
    if (right0 && right1 && right0->getBlockSort() == block->getBlockSort() && right1->getBlockSort() == block->getBlockSort()){
        if (right0->isStill() && right1->isStill()){
            deletion.pushBack(block);
            deletion.pushBack(right0);
            deletion.pushBack(right1);
        }
    }
    if (left0 && left1 && left0->getBlockSort() == block->getBlockSort() && left1->getBlockSort() == block->getBlockSort()){
        if (left0->isStill() && left1->isStill()){
            deletion.pushBack(block);
            deletion.pushBack(left0);
            deletion.pushBack(left1);
        }
    }
    
    return std::move(deletion);
}

bool MainScene::checkDeletion()
{
    BlockVct finalDeletionList;
    for (Blocks * block : _blocks) {
        // Check if they can be deleted if the block is standstill
        if (block->isStill()){
            
            BlockVct deletionList = this->checkNeighboringBlocks(block);
            if (deletionList.size() >= 1) {
                // put all the block for deletion list
                for (auto deletion : deletionList) {
                    finalDeletionList.pushBack(deletion);
                }
            }
        }
    }
    //if any of them are deleted
    if (finalDeletionList.size() >= 1) {
        _simCount=0;
        // delete the block NOW!
        for (auto deletion : finalDeletionList) {
            if (deletion->isStill()){
                _simCount++;
            }
            this->deleteBlock(deletion);
        }
        
        // Play with score
        _combo++; // +1 to combo
        /**Score system
         * Base Score = 10 + (# of blocks deleted simultaneously - 3)
         * Combo Bonus = 2 to the power (# of combo), MAXIMUM = 16384
         * Score gained = Base Score * Combo Bonus
         */
        auto baseScore = 10 + _simCount - 3;
        auto comboMultiplyer = (_combo < 15 ? _combo - 1 : 15);
        _score = _score + baseScore * powf(2.0f,(comboMultiplyer));
        _comboLevel = MIN(1.0f, _comboLevel+0.02);
        return true;
    }
    else{
        return false;
    }
}

void MainScene::deleteBlock(Blocks * block)
{
    // do nothing if nullptr
    if (!block) return;
    // make it as being deleted
    block->setState(Blocks::State::DELETED);
    // time it takes to be deleted
    const auto duration = 0.5f;
    // animation
    block->runAction(Sequence::create(FadeOut::create(duration),
                                       CallFuncN::create([this](Node* node) {
        // delete from the list
        auto block = dynamic_cast<Blocks *>(node);
        _blocks.eraseObject(block);
    }),
                                       RemoveSelf::create(),
                                       NULL));
}


bool MainScene::dropBlock(Blocks *block)
{
    auto position = block->getBlockPos();
    // unfallable states
    if (position.y == 0 || !block->isStill()) {
        return false;
    }
    // get the grid below
    auto downPosition = Vec2(position.x, position.y - 1);
    // get the block below
    auto down = this->getBlockAt(Vec2(position.x, position.y - 1));
    // if there's no one, drop it!
    if (down == nullptr) {
        // time it takes to drop
        const auto duration = 0.05;
        // and the distance
        auto distance = -Blocks::getSize();
        // Set the state
        block->setState(Blocks::State::DROPPING);
        // Drop!
        block->runAction(Sequence::create(MoveBy::create(duration, Vec2(0, distance)),
                                           CallFuncN::create([this, downPosition] (Node *node) {
            // After landing
            auto block = dynamic_cast<Blocks *>(node);
            // move the block
            this->moveBlock(block, downPosition);
            // set the state
            block->setState(Blocks::State::STANDSTILL);
            // do another check for multiple falling
            this->dropBlock(block);
        }),
                                           NULL));
        return true;
    }
    return false;
}

cocos2d::Vector<Blocks *> MainScene::checkSpawn()
{
    // List of blocks
    cocos2d::Vector<Blocks *> Blocks;
    // get the uppermost position IN GRID
    auto y = VERTICAL_COUNT - 1;
    // check all the rows
    for (int x = 0; x < HORIZONTAL_COUNT; ++x) {
        auto block = this->getBlockAt(Vec2(x, y));
        if (!block) { // if there are no blocks
            // Add a new block!
            auto block = Blocks::create();
            block->setBlockPos(Vec2(x, y));
            this->addBlock(block);
        }
    }
    return std::move(Blocks);
}

bool MainScene::shouldChangeMusic(){
    if ((int)(_comboLevel / 0.25) != _curComboLevel){
        _curComboLevel = (int)(_comboLevel / 0.25);
        return true;
    }
    return false;
}

//Refresh method
void MainScene::update(float dt)
{
    
    // Update ADX2
    ADX2::Manager::getInstance()->update();
    
    // Make new blocks
    this->checkSpawn();
 
    // Try to drop what it can drop
    for (auto block : _blocks){
        this->dropBlock(block);
    }
    
    // Try to delete what to delete
    this->checkDeletion();
    
    // Animate the score! (My favorite animation)
    if (_score - _aniScore > 3){
        _aniScore = _aniScore + round((_score - _aniScore) / 3);
    }
    else{
        if (_score > _aniScore){
            _aniScore = _aniScore + 1;
        }
        else{
            _aniScore = _score;
        }
    }
    
    
    // _comboLevel decreases over time, but don't let it go below 0
    _comboLevel = MAX(0.0, _comboLevel - 0.000075);
    // Set AISAC!
    _cueSheet->setAisacById((CriAtomExAisacControlId)2, _comboLevel);
    
    if (this->shouldChangeMusic()){
        _cueSheet->updateAll();
    }
    
    // Timer
    _time += dt;
    
    // Display the score!
    this->getScoreLabel()->setString(StringUtils::toString(_aniScore));
    this->getTimeLabel()->setString(StringUtils::toString(round(_time)));
}