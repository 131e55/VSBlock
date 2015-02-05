//
//  TitleScene.cpp
//  VSBlock
//
//  Created by 131e55 on 2/5/15.
//
//

#include "TitleScene.h"

USING_NS_CC;

Scene* TitleScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = TitleScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    return true;
}