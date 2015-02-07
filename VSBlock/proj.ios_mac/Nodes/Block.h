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
    // ブロックのテクスチャ数
    const int numTextures = 3;

    // ブロックが壊れているか
    bool broken = false;

    static Block* create();

    // ブロックのダメージの初期化
    void initialize();

    // ブロックへダメージを与える
    void hit();

private:
    // バーのテクスチャ
    std::vector<cocos2d::Texture2D*> _textures;

    // 現在のダメージ数
    int _currentDamage;
};

#endif /* defined(__VSBlock__Block__) */
