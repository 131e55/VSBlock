/*******************************************************************************
    File: GameScene.cpp
*******************************************************************************/

#include "GameScene.h"
#include "TitleScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    this->_screenSize = Director::getInstance()->getVisibleSize();

    // READY? ラベル
    this->_ready = Sprite::create("Ready.png");
    this->addChild(this->_ready);

    // 自分側のライフゲージを生成
    this->_youLifeGauge = LifeGauge::create(true);
    this->addChild(this->_youLifeGauge);

    // 自分側のブロックを生成
    for (auto &block : this->_youBlocks) {
        block = Block::create();
        this->addChild(block);
    }

    // 自分側のバーを生成
    this->_youBar = Bar::create();
    this->addChild(this->_youBar);

    // ゲーム初期化
    this->_initialize();

    // ゲームスタート
    this->_start();

    return true;
}

/*
 * ゲーム初期化
 */
void GameScene::_initialize()
{
    // Ready? の初期化
    this->_ready->setPosition(
        this->_screenSize.width / 2,
        this->_screenSize.height + this->_ready->getContentSize().height
    );

    // ライフゲージを初期化
    this->_youLifeGauge->initialize(sizeof(this->_youBlocks) / sizeof(this->_youBlocks[0]));

    // ブロックを初期化
    {
        int index = 0;
        float width = this->_youBlocks[0]->getContentSize().width;
        float height = this->_youBlocks[0]->getContentSize().height;
        const int column = 16;
        float offsetLeft = (this->_screenSize.width - width * column + width) / 2;
        float offsetBottom = 64;
        for (auto &block : this->_youBlocks) {
            block->initialize();
            block->setPosition(
                width * (index % column) + offsetLeft,
                height * (index / column) + offsetBottom
            );
            index++;
        }
    }

    // バーを初期化
    this->_youBar->initialize();
    this->_youBar->setPosition(this->_screenSize.width / 2, 160);
}


void GameScene::_start()
{
    // READY? ラベルをアニメーション表示させた後, ゲームスタート
    auto moveto = EaseBounceOut::create(MoveTo::create(
        1.5f,
        Point(this->_screenSize.width / 2, this->_screenSize.height / 2))
    );
    auto fadeto = FadeTo::create(0.5f, 0);
    auto startAction = Sequence::create(
        moveto,
        fadeto,
        CallFunc::create([this](){
            this->_newBall();
            this->scheduleUpdate();
        }),
        NULL
    );
    this->_ready->runAction(startAction);
}

void GameScene::_newBall()
{
    auto ball = Ball::create();
    ball->setPosition(this->_screenSize.width / 2, this->_screenSize.height / 2);
    this->addChild(ball);
    this->_balls.push_back(ball);
}

void GameScene::update(float frame) {

    // 全ボールに対してバーとの当たり判定を行う
    for (auto &ball : this->_balls) {
        ball->detectCollisionWithBar(this->_youBar->getBoundingBox());
    }
}

void GameScene::_transition()
{
    TransitionCrossFade* transition = NULL;
    float duration = 0.5f;

    transition = TransitionCrossFade::create(duration, TitleScene::createScene());
    Director::getInstance()->replaceScene(transition);
}
