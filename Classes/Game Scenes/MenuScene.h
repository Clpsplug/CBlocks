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
#include "ADX2Manager.h"
#include "menu.h"

typedef cocos2d::Vector<cocos2d::MenuItem *> MenuItems;

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
    
    CC_SYNTHESIZE(MenuItems, _mainMenuItems, MainMenuItems);
    CC_SYNTHESIZE_RETAIN(cocos2d::Menu *, _mainMenu, MainMenu);
    CC_SYNTHESIZE(MenuItems, _playMenuItems, PlayMenuItems);
    CC_SYNTHESIZE_RETAIN(cocos2d::Menu *, _playMenu, PlayMenu);
    CC_SYNTHESIZE(bool, _onMenuTrans, OnMenuTrans);
    CC_SYNTHESIZE(int, _menuNo, MenuNo);
    CC_SYNTHESIZE_RETAIN(ADX2::CueSheet *, _cuesheet, CueSheet);

    
};




#endif /* defined(__CBlocks__MenuScene__) */
