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

    auto block = Block::create();
    this->addChild(block);

    return true;
}

void GameScene::_transition()
{
    TransitionCrossFade* transition = NULL;
    float duration = 0.5f;

    transition = TransitionCrossFade::create(duration, TitleScene::createScene());
    Director::getInstance()->replaceScene(transition);
}
