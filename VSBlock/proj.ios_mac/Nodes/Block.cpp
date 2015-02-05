//
//  Block.cpp
//  VSBlock
//
//  Created by 131e55 on 2/5/15.
//
//

#include "Block.h"

USING_NS_CC;

Block* Block::create()
{
    Block *sprite = new Block();
    if (sprite && sprite->initWithFile("Block_0.png"))
    {
        sprite->autorelease();

        {
            int index = 0;
            for (auto &texture : sprite->_damageTextures) {
                texture = Director::getInstance()->getTextureCache()->addImage("Block_" + std::to_string(index) + ".png");
                index++;
            }
        }

        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Block::initialize()
{
    this->_currentDamage = 0;
    this->setTexture(this->_damageTextures[0]);
}

void Block::hit()
{
    this->_currentDamage ++;
    if (this->_currentDamage >= sizeof(this->_damageTextures) / sizeof(this->_damageTextures[0])) {
        this->setOpacity(0);
        this->broken = true;
    }
    else {
        this->setTexture(this->_damageTextures[this->_currentDamage]);
    }
}
