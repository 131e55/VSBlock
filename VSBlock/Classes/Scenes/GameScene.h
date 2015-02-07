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

    // READY? ラベル
    cocos2d::Sprite* _ready;

    // ライフゲージ
    LifeGauge* _youLifeGauge;
    LifeGauge* _rivalLifeGauge;

    // ブロック
    const int _numBlocks = 48;
    std::vector<Block*> _youBlocks;
    std::vector<Block*> _rivalBlocks;

    // バー
    Bar* _youBar;
    Bar* _rivalBar;

    // ボール
    std::vector<Ball*> _balls;
    int _nextBallNumber;

    // ゲームの初期化
    void _initialize();

    // ゲームスタート
    void _start();

    // ゲームストップ
    void _over(bool youWin);
    
    // 新しいボールの生成
    void _newBalls();

    // ボールとバーとの衝突判定
    // 戻り値: 青いボールとバーが衝突したかどうか
    bool _detectCollisionBallAndBar(Ball *ball, bool youSide);

    // ボールとブロックとの衝突判定
    bool _detectCollisionBallAndBlocks(Ball *ball, bool youSide);

    // Rival CPU
    void _rivalCPU();

    // シーン遷移
    void _transition();
};

#endif /* defined(__GAME_SCENE_H__) */