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

private:
    float _vx;
    float _vy;

    void _setVelocity();
};

#endif /* defined(__VSBlock__Ball__) */
