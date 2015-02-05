/*******************************************************************************
    File: GameScene.h
*******************************************************************************/

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "LifeGauge.h"
#include "Block.h"
#include "Bar.h"


class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

private:
    LifeGauge* _youLifeGauge;
    Block* _youBlocks[48];
    Bar* _youBar;

    void _gameInitialize();
    void _transition();
};

#endif /* defined(__GAME_SCENE_H__) */