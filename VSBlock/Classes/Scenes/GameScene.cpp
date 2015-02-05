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

    // 自分側のライフゲージを生成
    this->_youLifeGauge = LifeGauge::create(true);
    this->addChild(this->_youLifeGauge);

    // 相手側のライフゲージを生成
    this->_rivalLifeGauge = LifeGauge::create(false);
    this->addChild(this->_rivalLifeGauge);

    // 自分側のブロックを生成
    for (auto &block : this->_youBlocks) {
        block = Block::create();
        this->addChild(block);
    }

    // 相手側のブロックを生成
    for (auto &block : this->_rivalBlocks) {
        block = Block::create();
        this->addChild(block);
    }

    // 自分側のバーを生成
    this->_youBar = Bar::create(true);
    this->addChild(this->_youBar);

    // 相手側のバーを生成
    this->_rivalBar = Bar::create(false);
    this->addChild(this->_rivalBar);

    // READY? ラベル
    this->_ready = Sprite::create("Ready.png");
    this->addChild(this->_ready);

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
    this->_youLifeGauge->initialize(sizeof(this->_youBlocks) / sizeof(this->_youBlocks[0]) * this->_youBlocks[0]->life);
    this->_rivalLifeGauge->initialize(sizeof(this->_youBlocks) / sizeof(this->_youBlocks[0]) * this->_youBlocks[0]->life);

    // ブロックを初期化
    {
        int index = 0;
        float width = this->_youBlocks[0]->getContentSize().width;
        float height = this->_youBlocks[0]->getContentSize().height;
        const int column = 16;
        float offsetLeft = (this->_screenSize.width - width * column + width) / 2;

        // 自分側
        float offsetBottom = 64;
        for (auto &block : this->_youBlocks) {
            block->initialize();
            block->setPosition(
                width * (index % column) + offsetLeft,
                height * (index / column) + offsetBottom
            );
            index++;
        }

        // 相手側
        index = 0;
        offsetBottom = this->_screenSize.height - 64;
        for (auto &block : this->_rivalBlocks) {
            block->initialize();
            block->setPosition(
                width * (index % column) + offsetLeft,
                offsetBottom - height * (index / column)
            );
            index++;
        }
    }

    // バーを初期化
    this->_youBar->initialize();
    this->_youBar->setPosition(this->_screenSize.width / 2, 160);

    // バーを初期化
    this->_rivalBar->initialize();
    this->_rivalBar->setPosition(this->_screenSize.width / 2, this->_screenSize.height - 160);

    // ボールの出現数を初期化
    this->_nextBallNumber = 3;
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
            this->_newBalls();
            this->scheduleUpdate();
        }),
        NULL
    );
    this->_ready->runAction(startAction);
}

/*
 * nextBallNumberの数になるまでボールを生成
 */
void GameScene::_newBalls()
{
    auto num = this->_balls.size();
    for (int i = 0; i < this->_nextBallNumber - (int)num; i ++) {
        auto ball = Ball::create();
        ball->setPosition(this->_screenSize.width / 2, this->_screenSize.height / 2);
        this->addChild(ball);
        this->_balls.push_back(ball);
    }
}

void GameScene::update(float frame) {
    // 全ボールの衝突判定
    for (auto &ball : this->_balls) {
        // YOU Side
        if (ball->getPosition().y < this->_screenSize.height / 2) {
            // バーとの衝突判定
            this->_detectCollisionBallAndBar(ball, true);

            // ブロックとの衝突判定
            if (this->_detectCollisionBallAndBlocks(ball, true)) {
                break;
            }
        }
        // RIVAL Side
        else {
            // バーとの衝突判定
            this->_detectCollisionBallAndBar(ball, false);

            // ブロックとの衝突判定
            if(this->_detectCollisionBallAndBlocks(ball, false)) {
                break;
            }
        }
    }
}

void GameScene::_detectCollisionBallAndBar(Ball *ball, bool youSide)
{
    auto barRect = youSide ? this->_youBar->getBoundingBox() : this->_rivalBar->getBoundingBox();
    auto x = ball->getPosition().x;
    auto r = ball->getContentSize().width / 2;

    if (ball->getBoundingBox().intersectsRect(barRect)) {
        if (ball->vy < 0) { // top
            ball->vy *= -1;
            ball->setPosition(x, barRect.getMaxY() + r);
        }
        else if (ball->vy > 0) { // bottom
            ball->vy *= -1;
            ball->setPosition(x, barRect.getMinY() - r);
        }
    }
}

bool GameScene::_detectCollisionBallAndBlocks(Ball *ball, bool youSide)
{
    for (auto &block : (youSide ? this->_youBlocks : this->_rivalBlocks)) {
        if (!block->broken) {
            auto blockRect = block->getBoundingBox();

            if (ball->getBoundingBox().intersectsRect(blockRect)) {
                // ブロックに傷を入れ, ライフゲージを更新する
                block->hit();
                if (youSide) {
                    this->_youLifeGauge->damaged();
                }
                else {
                    this->_rivalLifeGauge->damaged();
                }

                // ボールを削除
                ball->removeFromParent();
                auto pos = std::find(this->_balls.begin(), this->_balls.end(), ball);
                this->_balls.erase(pos);

                // ボール出現数を増やしボールを生成
                this->_nextBallNumber++;
                if (this->_nextBallNumber > 50) {
                    this->_nextBallNumber = 50;
                }
                this->_newBalls();

                return true;
            }
        }
    }
    return false;
}

void GameScene::_transition()
{
    TransitionCrossFade* transition = NULL;
    float duration = 0.5f;

    transition = TransitionCrossFade::create(duration, TitleScene::createScene());
    Director::getInstance()->replaceScene(transition);
}
