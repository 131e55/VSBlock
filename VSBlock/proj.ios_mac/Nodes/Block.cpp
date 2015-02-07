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
    Block *block = new Block();
    if (block && block->initWithFile("Block_0.png")) {
        block->autorelease();

        // テクスチャを読み込む
        for (int i = 0; i < block->numTextures; i ++) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(
                "Block_" + std::to_string(i) + ".png");
            block->_textures.push_back(texture);
        }

        return block;
    }
    CC_SAFE_DELETE(block);
    return nullptr;
}

void Block::initialize()
{
    this->_currentDamage = 0;
    this->setTexture(this->_textures.at(0));
}

void Block::hit()
{
    if (this->_currentDamage < this->numTextures - 1) {
        this->_currentDamage ++;
        this->setTexture(this->_textures.at(this->_currentDamage));
    }
    else {
        this->setOpacity(0);
        this->broken = true;
    }
}

void Block::fix()
{
    // 壊れたのはさすがに直さない
    if (!this->broken && this->_currentDamage > 0) {
        this->_currentDamage --;
        this->setTexture(this->_textures.at(this->_currentDamage));
    }
}
