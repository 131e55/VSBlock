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
    Bar *bar = new Bar();

    if (bar && bar->initWithFile("Bar_0.png")) {
        bar->autorelease();

        // テクスチャを読み込む
        for (int i = 0; i < bar->_numTextures; i ++) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(
                "Bar_" + std::to_string(i) + ".png"
            );
            bar->_textures.push_back(texture);
        }

        // 自分側のバーのときだけタッチイベントを取る
        if (youSide) {
            auto listener = EventListenerTouchOneByOne::create();
            listener->onTouchBegan = CC_CALLBACK_2(Bar::onTouchBegan, bar);
            listener->onTouchMoved = CC_CALLBACK_2(Bar::onTouchMoved, bar);
            bar->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, bar);
        }

        return bar;
    }

    CC_SAFE_DELETE(bar);
    return nullptr;
}

void Bar::initialize()
{
    this->_currentDamage = 0;
    this->setTexture(this->_textures.at(0));
}

bool Bar::onTouchBegan(Touch *touch, Event *event)
{
    this->_touchBeganPoint = touch->getLocation();
    return true;
}

void Bar::onTouchMoved(Touch *touch, Event *event)
{
    auto p = touch->getLocation();
    this->touchMoved(p);
}

void Bar::cpuTouchBegan(Point p)
{
    this->_touchBeganPoint = p;
}

void Bar::cpuTouchMoved(Point p)
{
    this->touchMoved(p);
}

void Bar::touchMoved(Point p)
{
    Size screenSize = Director::getInstance()->getVisibleSize();
    auto x = this->getPosition().x;
    auto y = this->getPosition().y;
    auto halfW = this->getContentSize().width / 2;
    auto dx = p.x - this->_touchBeganPoint.x;
    this->_touchBeganPoint.x = p.x;

    // 画面の外まではバーを移動させない
    if (x + dx - halfW < 0) {
        x = halfW;
        dx = 0;
    }
    else if (x + dx + halfW > screenSize.width) {
        x = screenSize.width - halfW;
        dx = 0;
    }
    this->setPosition(x + dx, y);
}

void Bar::hit()
{
    if (this->_currentDamage < this->_numTextures - 1) {
        this->_currentDamage ++;
        auto newTexture = this->_textures.at(this->_currentDamage);
        auto newRect = Rect(0, 0, newTexture->getContentSize().width, newTexture->getContentSize().height);
        this->setTexture(newTexture);
        this->setTextureRect(newRect);
    }
    else {
        this->setOpacity(0);
        this->broken = true;
    }
}
