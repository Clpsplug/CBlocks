//
//  Blocks.h
//  CBlocks
//
//  Created by 有働 篤人 on 12/2/14.
//
//

#ifndef __CBlocks__Blocks__
#define __CBlocks__Blocks__

#include <stdio.h>
#include "cocos2d.h"

//size of the block...
const int BLOCK_SIZE = 72;

///Blocks Class, aka the subclass of cocos2d::Sprite
class Blocks :public cocos2d::Sprite
{
protected:
    Blocks();
    virtual ~Blocks();
    bool init() override;
    
public:
    
    /** A method to prevent instant death of the block
     *  A call will switch the sort of the block by 1. If the block is "5," then it will be "1"
     */
    void changeBlockSort();
    
    enum class Sort{
        FIRST,
        SECOND,
        THIRD,
        FOURTH,
        FIFTH,
        COUNT //Last one will represent the number of contents in this enum. Ingenious trick!
    };
    
    enum class State{
        //The block will be in those 5 states:
        STANDSTILL, //Doing Nothing
        SPAWNING, //Spawning into field
        DROPPING, //Dropping
        SWAPPED, //Being Swapped
        DELETED //Being deteted
    };
    
    static int getSize(){
        return BLOCK_SIZE;
    }
    
    /**Converts position(s) of Blocks in grid into in pixel
     * @param PosInGrid
     * @return Position in pixel
     */
    static cocos2d::Vec2 gridToPixel(const cocos2d::Vec2& PosInGrid);
    
    /**Converts position(s) of Blocks in pixel into in grid
     * @param PosInPixel
     * @return Position in grid
     */
    static cocos2d::Vec2 pixelToGrid(const cocos2d::Vec2& PosInPixel);
    
    /**Sets the position of blocks in pixel.
     * @param pos Set in terms of cocos2d::Vec2.
     */
    void setBlockPos(const cocos2d::Vec2& pos);
    
    /**Adjusts the position of blocks into the current position in grid.
     */
    void adjustPos();
    
    /** Tells if the block specified is standing still
     *  @return True if yes
     */
    bool isStill(){
        return _state == State::STANDSTILL;
    }
    
    CC_SYNTHESIZE(State, _state, State);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(cocos2d::Vec2, _blockPos, BlockPos); //Position of Block in GRID
    CC_SYNTHESIZE_READONLY(Sort, _blockSort, BlockSort);
    CREATE_FUNC(Blocks);
};

#endif /* defined(__CBlocks__Blocks__) */
