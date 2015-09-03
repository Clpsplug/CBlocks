//
//  ResultScene.h
//  CBlocks
//
//  Created by 有働 篤人 on 9/3/15.
//
//

#ifndef __CBlocks__ResultScene__
#define __CBlocks__ResultScene__

#include <stdio.h>
#include "cocos2d.h"
#include "ADX2Manager.h"

USING_NS_CC;

class ResultScene :public cocos2d::Layer{
  
protected:
    ResultScene();
    virtual ~ResultScene();
    bool init() override;
    
    
public:
    void update(float dt);
    void onEnterTransitionDidFinish() override;
    
    static Scene* createScene();
    
    
    
    CREATE_FUNC(ResultScene);
    
};

#endif /* defined(__CBlocks__ResultScene__) */
