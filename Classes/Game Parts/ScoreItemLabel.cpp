//
//  ScoreItemLabel.cpp
//  CBlocks
//
//  Created by 有働 篤人 on 1/1/15.
//
//

#include "ScoreItemLabel.h"

USING_NS_CC;

ScoreItemLabel::ScoreItemLabel()
: _score(0)
, _pos(cocos2d::Vec2(0,0))
, _hasEnded(false)
{
    
}

ScoreItemLabel::~ScoreItemLabel(){

}

bool ScoreItemLabel::init(){
    
    this->setCharMap("digits.png", 17, 17, '0');
    return true;
}

void ScoreItemLabel::start(cocos2d::Node * parentNode, cocos2d::Vec2 startingPoint){
    
    this->setPosition(startingPoint);
    this->setScale(0.75f);
    
    // Make an animation (Score Earnt will Fly towards the Score Label)
    auto motion1 = MoveTo::create(0.75f, _pos);
    auto motion1_withEaseIn = EaseIn::create(motion1, 10.0f);
    
    auto explosion = CallFuncN::create([this, parentNode](Node* node){
        //auto *label = dynamic_cast<Label *>(node);
        // パーティクル表示
        auto explosion = ParticleExplosion::create();
        explosion->setPosition(_pos);
        explosion->setEmissionRate(100.0f*(float)(_score/327680));
        explosion->setSpeed(100.0f);
        explosion->setAutoRemoveOnFinish(true);
        explosion->setLife(1.0f);
        parentNode->addChild(explosion);
        _hasEnded = true;
    });
    
    this->setString(cocos2d::StringUtils::toString(_score));
    
    auto animation = Sequence::create(motion1_withEaseIn, explosion, NULL);
    
    this->runAction(animation);
}