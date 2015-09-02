//
//  Counter.h
//  CBlocks
//
//  Created by 有働 篤人 on 9/1/15.
//
//

#ifndef __CBlocks__Counter__
#define __CBlocks__Counter__

#include <stdio.h>
#include "cocos2d.h"

class Counter :public cocos2d::Label
{
  
protected:
    Counter();
    virtual ~Counter();
    bool init() override;
public:
    
    CREATE_FUNC(Counter);
};

#endif /* defined(__CBlocks__Counter__) */
