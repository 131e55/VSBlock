//
//  LifeGauge.h
//  VSBlock
//
//  Created by 131e55 on 2/5/15.
//
//

#ifndef __VSBlock__LifeGauge__
#define __VSBlock__LifeGauge__

#include "cocos2d.h"

class LifeGauge : public cocos2d::Layer
{
public:
    static LifeGauge* create(bool youSide);

    // ライフゲージの初期化
    void initialize(float life);

    void damaged();

private:
    cocos2d::Sprite* _gauge;
    float _gaugeMaxWidth;
    float _maxLife;
    float _currentLife;
};

#endif /* defined(__VSBlock__LifeGauge__) */
