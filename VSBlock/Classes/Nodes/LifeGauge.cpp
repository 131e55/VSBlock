//
//  LifeGauge.cpp
//  VSBlock
//
//  Created by 131e55 on 2/5/15.
//
//

#include "LifeGauge.h"

USING_NS_CC;

LifeGauge* LifeGauge::create(bool youSide)
{
    LifeGauge *gauge = new LifeGauge();

    if (gauge && gauge->init()) {
        gauge->autorelease();

        Size screenSize = Director::getInstance()->getVisibleSize();
        float space = 8;

        auto label = youSide ? Sprite::create("You.png") : Sprite::create("Rival.png");
        {
            float width = label->getContentSize().width;
            float height = label->getContentSize().height;

            if (youSide) {
                label->setPosition(width / 2 + space, height / 2 + space);
            }
            else {
                label->setPosition(width / 2 + space, screenSize.height - height / 2 - space);
            }
            gauge->addChild(label);
        }

        gauge->_gauge = Sprite::create("LifeGauge.png");
        {
            float x = label->getPosition().x + label->getContentSize().width / 2 + space;
            float y = label->getPosition().y;

            // 横に拡大してゲージを表現するため左端をオリジンにする
            gauge->_gauge->setPosition(x, y);
            gauge->_gauge->setAnchorPoint(Point(0, 0.5));
            gauge->addChild(gauge->_gauge);
        }

        gauge->_gaugeMaxWidth = screenSize.width - gauge->_gauge->getPosition().x - space;

        return gauge;
    }

    CC_SAFE_DELETE(gauge);
    return nullptr;
}

void LifeGauge::initialize(float life)
{
    this->_maxLife = life;
    this->currentLife = life;
    this->_gauge->setScaleX(this->_gaugeMaxWidth);
}

void LifeGauge::decrease()
{
    if (this->currentLife > 0) {
        this->currentLife --;
        auto per = this->currentLife / this->_maxLife;
        this->_gauge->setScaleX(this->_gaugeMaxWidth * per);
    }
}

void LifeGauge::increase()
{
    if (this->currentLife < this->_maxLife) {
        this->currentLife ++;
        auto per = this->currentLife / this->_maxLife;
        this->_gauge->setScaleX(this->_gaugeMaxWidth * per);
    }
}