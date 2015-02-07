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
    Ball *ball = new Ball();

    // 大体白いボール。たまに青いボール。
    auto type = White;
    if (rand() % 100 < 10) {
        type = Blue;
    }

    if (ball && ball->initWithFile("Ball_" + std::to_string(type) + ".png")) {
        ball->autorelease();

        ball->type = type;
        ball->_setVelocity();

        return ball;
    }

    CC_SAFE_DELETE(ball);
    return nullptr;
}

void Ball::_setVelocity()
{
    // 移動方向と速度はランダム
    this->vx = (rand() % 350 + 75) / 100.0;
    this->vy = (rand() % 350 + 75) / 100.0;
    if (rand() % 2 == 0) {
        this->vx *= -1;
    }
    if (rand() % 2 == 0) {
        this->vy *= -1;
    }
}

void Ball::move() {
    auto x = this->getPosition().x + this->vx;
    auto y = this->getPosition().y + this->vy;
    this->setPosition(x, y);
}