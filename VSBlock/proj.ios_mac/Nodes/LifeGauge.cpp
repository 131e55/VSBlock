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
    LifeGauge *layer = new LifeGauge();
    if (layer && layer->init())
    {
        layer->autorelease();

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
            layer->addChild(label);
        }

        layer->_gauge = Sprite::create("LifeGauge.png");
        {
            float x = label->getPosition().x + label->getContentSize().width / 2 + space;
            float y = label->getPosition().y;

            layer->_gauge->setPosition(x, y);
            layer->_gauge->setAnchorPoint(Point(0, 0.5));
            layer->addChild(layer->_gauge);
        }

        layer->_gaugeMaxWidth = screenSize.width - layer->_gauge->getPosition().x - space;

        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

void LifeGauge::initialize(float life)
{
    this->_maxLife = life;
    this->currentLife = life;
    this->_gauge->setScaleX(this->_gaugeMaxWidth);
}

void LifeGauge::damaged()
{
    this->currentLife--;
    auto per = this->currentLife / this->_maxLife;
    this->_gauge->setScaleX(this->_gaugeMaxWidth * per);
}