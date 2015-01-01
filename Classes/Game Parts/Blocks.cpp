//
//  Blocks.cpp
//  CBlocks
//
//  Created by 有働 篤人 on 12/2/14.
//
//

#include "Blocks.h"
#include "cocos2d.h"
#include <random>

using namespace cocos2d;

Blocks::Blocks()
: _state(State::STANDSTILL)
{
}

Blocks::~Blocks()
{
    
}

bool Blocks::init()
{
    //Activate a randomizer
    std::random_device rdev;
    auto engine = std::mt19937(rdev());
    auto dist = std::uniform_int_distribution<>(0, (int)Blocks::Sort::COUNT - 1);
    
    //Select one random sort from the enum Sort
    auto sort = dist(engine);
    _blockSort = static_cast<Blocks::Sort>(sort);
    
    //initialize the sprite from variant sort
    if (!Sprite::initWithFile("blocks.png", Rect(Blocks::getSize() * sort,
                                                  0,
                                                  Blocks::getSize(),
                                                  Blocks::getSize()))) {
        return false;
    }
    return true;
}

void Blocks::changeBlockSort(){
    _blockSort = (_blockSort == Sort::FIFTH ? Sort::FIRST : static_cast<Blocks::Sort>((int)_blockSort + 1));
    // Re-initialize the sprite!
    Sprite::initWithFile("blocks.png", Rect(Blocks::getSize() * (int)_blockSort,
                                            0,
                                            Blocks::getSize(),
                                            Blocks::getSize()));
}

Vec2 Blocks::gridToPixel(const cocos2d::Vec2& PosInGrid)
{
    //That 3.0f is offset!
    return std::move((PosInGrid + Vec2::ONE * 0.5) * Blocks::getSize() + Vec2(3.0f, 3.0f));
}

Vec2 Blocks::pixelToGrid(const cocos2d::Vec2& PosInPixel)
{
    auto x = floor(PosInPixel.x / Blocks::getSize());
    auto y = floor(PosInPixel.y / Blocks::getSize());
    return std::move(Vec2(x, y));
}

void Blocks::setBlockPos(const cocos2d::Vec2& pos)
{
    // Assert if pos contains numbers other than integers
    CCASSERT(floor(pos.x) == pos.x || floor(pos.y) == pos.y, "Position must be in Integers!!!!");
    _blockPos = pos;
}

void Blocks::adjustPos(){
    auto position = _blockPos;
    // _blockPosを元にpositionを設定する
    this->setPosition(Blocks::gridToPixel(position));
}
