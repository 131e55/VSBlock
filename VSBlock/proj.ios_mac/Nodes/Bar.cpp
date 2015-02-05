//
//  Bar.cpp
//  VSBlock
//
//  Created by 131e55 on 2/5/15.
//
//

#include "Bar.h"


USING_NS_CC;

Bar* Bar::create()
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