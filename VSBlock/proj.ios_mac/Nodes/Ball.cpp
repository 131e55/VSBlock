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
    this->_move();
    this->_detectCollisionWithWalls();
}

void Ball::_move() {
    auto x = this->getPosition().x + this->_vx;
    auto y = this->getPosition().y + this->_vy;
    this->setPosition(x, y);
}

void Ball::_detectCollisionWithWalls()
{
    auto x = this->getPosition().x;
    auto y = this->getPosition().y;
    auto r = this->getContentSize().width / 2;

    if (x - r <= 0) { // left
        this->setPosition(r, this->getPosition().y);
        this->_vx *= -1;
    }
    else if (x + r >= this->_screenSize.width) { // right
        this->setPosition(this->_screenSize.width - r, this->getPosition().y);
        this->_vx *= -1;
    }
    else if (y - r <= 0) { // bottom
        this->setPosition(this->getPosition().x, r);
        this->_vy *= -1;
    }
    else if(y + r >= this->_screenSize.height) { // top
        this->setPosition(this->getPosition().x, this->_screenSize.height - r);
        this->_vy *= -1;
    }
}

void Ball::detectCollisionWithBar(Rect bar)
{
    auto x = this->getPosition().x;
    auto r = this->getContentSize().width / 2;

    if (this->getBoundingBox().intersectsRect(bar)) {
        if (this->_vy < 0) { // top
            this->_vy *= -1;
            this->setPosition(x, bar.getMaxY() + r);
        }
        else if (this->_vy > 0) { // bottom
            this->_vy *= -1;
            this->setPosition(x, bar.getMinY() - r);
        }
    }
}