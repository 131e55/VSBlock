//
//  Block.h
//  VSBlock
//
//  Created by 131e55 on 2/5/15.
//
//

#ifndef __VSBlock__Block__
#define __VSBlock__Block__

#include "cocos2d.h"

class Block : public cocos2d::Sprite
{
public:
    static Block* create();

    // ブロックダメージの初期化
    void initialize();

    void hit();

    // ブロックが壊されているか
    bool broken = false;

private:
    cocos2d::Texture2D* _damageTextures[3];
    int _currentDamage;
};

#endif /* defined(__VSBlock__Block__) */
