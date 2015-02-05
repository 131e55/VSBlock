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

        sprite->_setVelocity();
        sprite->scheduleUpdate();

        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Ball::_setVelocity()
{
    this->_vx = (rand() % 400 + 1) / 100.0;
    this->_vy = (rand() % 400 + 1) / 100.0;
    if (rand() % 2 == 0) {
        this->_vx *= -1;
    }
    if (rand() % 2 == 0) {
        this->_vy *= -1;
    }
}

void Ball::update(float frame) {
    this->runAction(MoveBy::create(0, Vec2(this->_vx, this->_vy)));
}