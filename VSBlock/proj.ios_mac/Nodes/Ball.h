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
    static Ball* create();

    void update(float frame);

    enum Type {White, Blue};
    float vx;
    float vy;
    int type;

private:
    cocos2d::Size _screenSize;

    void _setVelocity();
    void _move();
    void _detectCollisionWithWalls();
};

#endif /* defined(__VSBlock__Ball__) */
