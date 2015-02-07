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
    // 現在のライフ数
    float currentLife;

    static LifeGauge* create(bool youSide);

    // ライフゲージの初期化
    void initialize(float life);

    // ゲージを減らす
    void decrease();

    // ゲージを増やす
    void increase();

private:
    // ライフの最大数
    float _maxLife;

    // ゲージのスプライト
    cocos2d::Sprite* _gauge;

    // ゲージの最大横幅
    float _gaugeMaxWidth;
};

#endif /* defined(__VSBlock__LifeGauge__) */
