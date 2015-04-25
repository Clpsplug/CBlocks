//
//  MenuScene.h
//  CBlocks
//
//  Created by 有働 篤人 on 4/25/15.
//
//

#ifndef __CBlocks__MenuScene__
#define __CBlocks__MenuScene__

#include <stdio.h>
#include "cocos2d.h"


class MenuScene :public cocos2d::Layer{
  
protected:
    MenuScene(); //constructor
    virtual ~MenuScene(); //destructor
    bool init() override; //initialization
    
    
    
public:
    
    void update(float dt); //update method
    void onEnterTransitionDidFinish() override; //called when transisiton is over

    //things to do when creating this scene
    static cocos2d::Scene * createScene();
    CREATE_FUNC(MenuScene);

    
};




#endif /* defined(__CBlocks__MenuScene__) */
