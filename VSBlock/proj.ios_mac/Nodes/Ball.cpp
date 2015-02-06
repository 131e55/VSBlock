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

    // 大体白いボール。たまに青いボール。
    sprite->type = White;
    if (rand() % 100 < 10) {
        sprite->type = Blue;
    }

    if (sprite && sprite->initWithFile("Ball_" + std::to_string(sprite->type) + ".png"))
    {
        sprite->autorelease();

        sprite->_screenSize = Director::getInstance()->getVisibleSize();
        sprite->_setVelocity();
        sprite->scheduleUpdate();

        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Ball::_setVelocity()
{
    this->vx = (rand() % 400 + 50) / 100.0;
    this->vy = (rand() % 400 + 50) / 100.0;
    if (rand() % 2 == 0) {
        this->vx *= -1;
    }
    if (rand() % 2 == 0) {
        this->vy *= -1;
    }
}

void Ball::update(float frame) {
    this->_move();
    this->_detectCollisionWithWalls();
}

void Ball::_move() {
    auto x = this->getPosition().x + this->vx;
    auto y = this->getPosition().y + this->vy;
    this->setPosition(x, y);
}

void Ball::_detectCollisionWithWalls()
{
    auto x = this->getPosition().x;
    auto y = this->getPosition().y;
    auto r = this->getContentSize().width / 2;

    if (x - r <= 0) { // left
        this->setPosition(r, this->getPosition().y);
        this->vx *= -1;
    }
    else if (x + r >= this->_screenSize.width) { // right
        this->setPosition(this->_screenSize.width - r, this->getPosition().y);
        this->vx *= -1;
    }
    else if (y - r <= 0) { // bottom
        this->setPosition(this->getPosition().x, r);
        this->vy *= -1;
    }
    else if(y + r >= this->_screenSize.height) { // top
        this->setPosition(this->getPosition().x, this->_screenSize.height - r);
        this->vy *= -1;
    }
}