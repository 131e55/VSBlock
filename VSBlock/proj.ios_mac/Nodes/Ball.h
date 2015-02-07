//
//  Ball.h
//  VSBlock
//
//  Created by 131e55 on 2/5/15.
//
//

#ifndef __VSBlock__Ball__
#define __VSBlock__Ball__

#include "cocos2d.h"

class Ball : public cocos2d::Sprite
{
public:
    // ボールのタイプ
    // 白色と青色
    enum Type {White, Blue};
    int type;

    // ボールの速度[px/frame]
    float vx;
    float vy;

    static Ball* create();

    // ボールの移動
    void move();

private:
    // 速度[px/frame]の設定
    void _setVelocity();
};

#endif /* defined(__VSBlock__Ball__) */
