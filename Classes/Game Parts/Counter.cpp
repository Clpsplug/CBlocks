//
//  Counter.cpp
//  CBlocks
//
//  Created by 有働 篤人 on 9/1/15.
//
//

#include "Counter.h"

USING_NS_CC;

Counter::Counter(){
    
}

Counter::~Counter(){
    
}

bool Counter::init(){
    if (!this->setCharMap("digits.png", 34, 34, '0')){
        return false;
    }
    return true;
}