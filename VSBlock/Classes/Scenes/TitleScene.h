//
//  TitleScene.h
//  VSBlock
//
//  Created by 131e55 on 2/5/15.
//
//

#ifndef __VSBlock__TitleScene__
#define __VSBlock__TitleScene__

#include "cocos2d.h"

class TitleScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(TitleScene);
};

#endif /* defined(__VSBlock__TitleScene__) */
