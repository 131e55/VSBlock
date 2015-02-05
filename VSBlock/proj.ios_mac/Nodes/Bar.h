//
//  Bar.h
//  VSBlock
//
//  Created by 131e55 on 2/5/15.
//
//

#ifndef __VSBlock__Bar__
#define __VSBlock__Bar__

#include "cocos2d.h"

class Bar : public cocos2d::Sprite
{
public:
    static Bar* create();

    // バーダメージの初期化
    void initialize();

    void hit();

private:
    cocos2d::Texture2D* _damageTextures[1];
    int _currentDamage;
};
#endif /* defined(__VSBlock__Bar__) */
