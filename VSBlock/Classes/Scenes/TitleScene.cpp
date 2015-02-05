//
//  TitleScene.cpp
//  VSBlock
//
//  Created by 131e55 on 2/5/15.
//
//

#include "TitleScene.h"
#include "GameScene.h"

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

    auto screenSize = Director::getInstance()->getVisibleSize();

    // Background
    auto background = Sprite::create("TitleBG.png");
    background->setPosition(screenSize.width / 2, screenSize.height / 2);
    this->addChild(background);

    // Title
    auto title = Sprite::create("Title.png");
    title->setPosition(screenSize.width / 2, screenSize.height / 2);
    this->addChild(title);

    // Start
    auto start = Sprite::create("Start.png");
    start->setPosition(screenSize.width / 2, screenSize.height / 2 - start->getContentSize().height * 2);
    auto fadeOut = FadeOut::create(0.5f);
    auto fadeIn = FadeIn::create(0.5f);
    auto sequence = Sequence::create(fadeOut, fadeIn, NULL);
    start->runAction(RepeatForever::create(sequence));
    this->addChild(start);

    // Touch Event Listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(TitleScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(TitleScene::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

bool TitleScene::onTouchBegan(Touch *touch, Event *event)
{
    return true;
}

void TitleScene::onTouchEnded(Touch *touch, Event *event)
{
    // 画面をクリックしたらGameSceneへ遷移する
    this->_transition();
}

// Scene transition
void TitleScene::_transition()
{
    Director::getInstance()->replaceScene(GameScene::createScene());
}