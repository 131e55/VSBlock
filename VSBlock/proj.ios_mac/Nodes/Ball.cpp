//
//  Ball.cpp
//  VSBlock
//
//  Created by 131e55 on 2/5/15.
//
//

#include "Ball.h"

USING_NS_CC;

Ball* Ball::create()
{
    Ball *sprite = new Ball();
    if (sprite && sprite->initWithFile("Ball_0.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}