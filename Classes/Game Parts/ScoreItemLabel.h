//
//  ScoreItemLabel.h
//  CBlocks
//
//  Created by 有働 篤人 on 1/1/15.
//
//

#ifndef __CBlocks__ScoreItemLabel__
#define __CBlocks__ScoreItemLabel__

#include <stdio.h>
#include "cocos2d.h"

class ScoreItemLabel :public cocos2d::Label
{
    
protected:
    ScoreItemLabel();
 
    // this will NOT trigger animation
    bool init() override;
    
public:
       virtual ~ScoreItemLabel();
    /** Starts the animation
     *  @param parentNode Parent node of game scene
     *  @remarks _score & _pos should be set before this is called
     */
    void start(cocos2d::Node * parentNode, cocos2d::Vec2 startingPoint);
    
    // Set the score here
    CC_SYNTHESIZE(int, _score, Score);
    // Tell the position of score label
    CC_SYNTHESIZE(cocos2d::Vec2, _pos, Pos);
    CC_SYNTHESIZE(bool, _hasEnded, HasEnded);
    // Animation
    //CC_SYNTHESIZE_RETAIN(cocos2d::Sequence*, _animation, Animation);
    
    CREATE_FUNC(ScoreItemLabel);
};

#endif /* defined(__CBlocks__ScoreItemLabel__) */
