//
//  MainScene.h
//  ComboBlocks
//
//  Created by 有働 篤人 on 11/28/14.
//
//

#ifndef ComboBlocks_MainScene_h
#define ComboBlocks_MainScene_h

#include "cocos2d.h"
#include "Blocks.h"

typedef cocos2d::Vector<Blocks *> BlockVct;

class MainScene :public cocos2d::Layer
{
protected:
    MainScene(); //constructor
    virtual ~MainScene(); //destructor
    bool init() override; //initialization
    
    
    
public:
    
    void update(float dt); //update method
    //void onEnterTransitionDidFinish() override; //called when transisiton is over
    
    /** Add block specified
     *  @param block to be added
     */
    void addBlock(Blocks *block);
    
    /** tells the sort of the block at the designated position IN GRID
     *   nullptr if nothing found
     *  @param position position of blocks IN GRID
     *  @return Sort of block OR nullptr
     */
    Blocks* getBlockAt(const cocos2d::Vec2& pos);
    
    /** tells the sort of the block at the designated position IN PIXEL
     *  nullptr if nothing found
     *  @param position position of blocks IN PIXEL
     *  @return Sort of block OR nullptr
     */
    Blocks* getBlockAtByPixel(const cocos2d::Vec2& pixelPos);
    
    /** Move the block into designated position IN GRID
     *  @param block0 Block to be moved
     *  @param destination The name says it all
     */
    void moveBlock(Blocks* block0, const cocos2d::Vec2& destination);
    
    /** Swap 2 blocks specified
     *  @param block0 The block specified
     *  @param block1
     */
    bool swapBlocks(Blocks* block0, Blocks* block1);
    
    //things to do when creating this scene
    static cocos2d::Scene * createScene();
    CREATE_FUNC(MainScene);
    
    /// Vector for saving blocks
    CC_SYNTHESIZE(BlockVct, _blocks, Blocks);
    /// Node to put blocks
    CC_SYNTHESIZE_RETAIN(cocos2d::Node*, _playField, PlayField);
    /// Block being selected
    CC_SYNTHESIZE_RETAIN(Blocks *, _curBlock, CurBlock);
    
};

#endif
