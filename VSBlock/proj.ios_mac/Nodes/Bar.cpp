//
//  Bar.cpp
//  VSBlock
//
//  Created by 131e55 on 2/5/15.
//
//

#include "Bar.h"


USING_NS_CC;

Bar* Bar::create(bool youSide)
{
    Bar *sprite = new Bar();
    if (sprite && sprite->initWithFile("Bar_0.png"))
    {
        sprite->autorelease();

        {
            int index = 0;
            for (auto &texture : sprite->_damageTextures) {
                texture = Director::getInstance()->getTextureCache()->addImage("Bar_" + std::to_string(index) + ".png");
                index++;
            }
        }

        // Touch Event Listener
        if (youSide) {
            auto listener = EventListenerTouchOneByOne::create();
            listener->onTouchBegan = CC_CALLBACK_2(Bar::onTouchBegan, sprite);
            listener->onTouchMoved = CC_CALLBACK_2(Bar::onTouchMoved, sprite);
            sprite->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sprite);
        }

        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Bar::initialize()
{
    this->_currentDamage = 0;
    this->setTexture(this->_damageTextures[0]);
}

bool Bar::onTouchBegan(Touch *touch, Event *event)
{
    this->_touchBeganPoint = touch->getLocation();
    return true;
}

void Bar::onTouchMoved(Touch *touch, Event *event)
{
    Size screenSize = Director::getInstance()->getVisibleSize();
    auto p = touch->getLocation();
    auto x = this->getPosition().x;
    auto y = this->getPosition().y;
    auto r = this->getContentSize().width / 2;
    auto dx = p.x - this->_touchBeganPoint.x;
    this->_touchBeganPoint.x = p.x;

    if (x + dx - r < 0) {
        x = r;
        dx = 0;
    }
    else if (x + dx + r > screenSize.width) {
        x = screenSize.width - r;
        dx = 0;
    }
    this->setPosition(x + dx, y);
}



