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
class MainScene :public cocos2d::Layer
{
protected:
    MainScene(); //constructor
    virtual ~MainScene(); //destructor
    bool init() override; //initialization
    
    
    
public:
    static cocos2d::Scene * createScene(); //things to do when creating this scene
    CREATE_FUNC(MainScene);
    
    void update(float dt); //update method
    //void onEnterTransitionDidFinish() override; //called when transisiton is over

    
};

#endif
