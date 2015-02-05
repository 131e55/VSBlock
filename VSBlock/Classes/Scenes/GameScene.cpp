/*******************************************************************************
    File: GameScene.cpp
*******************************************************************************/

#include "GameScene.h"
#include "Block.h"
#include "TitleScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    for (auto &block : this->_myBlocks) {
        block = Block::create();
        this->addChild(block);
    }

    this->_gameInitialize();

    return true;
}

/*
 * ゲーム初期化
 */
void GameScene::_gameInitialize()
{
    Size screenSize = Director::getInstance()->getVisibleSize();

    // ブロックの位置を初期化
    {
        int index = 0;
        float width = this->_myBlocks[0]->getContentSize().width;
        float height = this->_myBlocks[0]->getContentSize().height;
        const int column = 16;
        float offsetLeft = (screenSize.width / 2) - (width * column / 2);
        float offsetBottom = 100;
        for (auto &block : this->_myBlocks) {
            block->setPosition(
                width * (index % column) + offsetLeft,
                height * (index / column) + offsetBottom
            );
            index++;
        }
    }
}

void GameScene::_transition()
{
    TransitionCrossFade* transition = NULL;
    float duration = 0.5f;

    transition = TransitionCrossFade::create(duration, TitleScene::createScene());
    Director::getInstance()->replaceScene(transition);
}
