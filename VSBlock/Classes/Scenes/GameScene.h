/*******************************************************************************
    File: GameScene.h
*******************************************************************************/

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "LifeGauge.h"
#include "Block.h"
#include "Bar.h"
#include "Ball.h"

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

    // Main loop
    void update(float frame);

private:
    cocos2d::Size _screenSize;
    cocos2d::Sprite* _ready;
    LifeGauge* _youLifeGauge;
    LifeGauge* _rivalLifeGauge;
    Block* _youBlocks[48];
    Bar* _youBar;
    std::vector<Ball*> _balls;
    int _nextBallNumber;

    // ゲームの初期化
    void _initialize();

    // ゲームスタート
    void _start();

    // 新しいボールの生成
    void _newBalls();

    void _transition();
};

#endif /* defined(__GAME_SCENE_H__) */