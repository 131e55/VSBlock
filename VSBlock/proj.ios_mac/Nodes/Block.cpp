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
    if (sprite && sprite->initWithFile("Block.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

