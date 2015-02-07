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
    for (int i = 0; i < this->_numBlocks; i ++) {
        auto block = Block::create();
        this->_youBlocks.push_back(block);
        this->addChild(block);
    }

    // 相手側のブロックを生成
    for (int i = 0; i < this->_numBlocks; i ++) {
        auto block = Block::create();
        this->_rivalBlocks.push_back(block);
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
    this->_youLifeGauge->initialize(this->_numBlocks * this->_youBlocks.at(0)->numTextures);
    this->_rivalLifeGauge->initialize(this->_numBlocks * this->_rivalBlocks.at(0)->numTextures);

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
    // タッチ開始位置はバーの位置とする
    this->_rivalBar->cpuTouchBegan(this->_rivalBar->getPosition());

    // ボールの出現数を初期化
    this->_nextBallNumber = 5;
}

void GameScene::_start()
{
    // READY? ラベルをアニメーション表示させた後, ゲームスタート
    auto moveto = EaseBounceOut::create(MoveTo::create(
        1.0f,
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

void GameScene::_over(bool youWin)
{
    // ボールやバーは動いていいが衝突判定だけは止める
    this->unscheduleUpdate();

    // リザルトラベルを表示してタイトルへ戻る
    auto result = youWin ? Sprite::create("Win.png") : Sprite::create("Lose.png");
    result->setPosition(
        this->_screenSize.width / 2,
        this->_screenSize.height + result->getContentSize().height
    );
    this->addChild(result);

    auto moveto = EaseBounceOut::create(MoveTo::create(
        1.0f,
        Point(this->_screenSize.width / 2, this->_screenSize.height / 2))
    );
    auto fadeto = FadeTo::create(2.0f, 0);
    auto overAction = Sequence::create(
        moveto,
        fadeto,
        CallFunc::create([this](){
            this->_transition();
        }),
        NULL
    );
    result->runAction(overAction);
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
            if (!this->_youBar->broken) {
                if (this->_detectCollisionBallAndBar(ball, true)) {
                    break;
                }
            }

            // ブロックとの衝突判定
            if (this->_detectCollisionBallAndBlocks(ball, true)) {
                break;
            }
        }
        // RIVAL Side
        else {
            // バーとの衝突判定
            if (!this->_rivalBar->broken) {
                if(this->_detectCollisionBallAndBar(ball, false)) {
                    break;
                }
            }

            // ブロックとの衝突判定
            if(this->_detectCollisionBallAndBlocks(ball, false)) {
                break;
            }
        }
    }

    // Rival CPU
    this->_rivalCPU();
}

// ボールとバーの衝突判定処理
// 戻り値: 青いボールとバーが衝突したかどうか
bool GameScene::_detectCollisionBallAndBar(Ball *ball, bool youSide)
{
    auto barRect = youSide ? this->_youBar->getBoundingBox() : this->_rivalBar->getBoundingBox();
    auto x = ball->getPosition().x;
    auto r = ball->getContentSize().width / 2;

    if (ball->getBoundingBox().intersectsRect(barRect)) {
        switch (ball->type) {
            // 白いボールは跳ね返す
            case Ball::White:
                if (ball->vy < 0) { // top
                    ball->vy *= -1;
                    ball->setPosition(x, barRect.getMaxY() + r);
                }
                else if (ball->vy > 0) { // bottom
                    ball->vy *= -1;
                    ball->setPosition(x, barRect.getMinY() - r);
                }
                break;

            // 青いボールはバーがダメージを受ける
            case Ball::Blue:
                if (youSide) {
                    this->_youBar->hit();
                }
                else {
                    this->_rivalBar->hit();
                }

                // ボールを削除
                ball->removeFromParent();
                auto pos = std::find(this->_balls.begin(), this->_balls.end(), ball);
                this->_balls.erase(pos);

                return true;
                break;
        }
    }

    return false;
}

// ボールとブロックの衝突判定処理
bool GameScene::_detectCollisionBallAndBlocks(Ball *ball, bool youSide)
{
    for (auto &block : (youSide ? this->_youBlocks : this->_rivalBlocks)) {
        if (!block->broken) {
            auto blockRect = block->getBoundingBox();

            if (ball->getBoundingBox().intersectsRect(blockRect)) {
                switch (ball->type) {
                    // 白いボールなら, ブロックに傷を入れ, ライフゲージを減らす
                    case Ball::White:
                        block->hit();
                        if (youSide) {
                            this->_youLifeGauge->decrease();

                            // 負け
                            if (this->_youLifeGauge->currentLife <= 0) {
                                this->_over(false);
                            }
                        }
                        else {
                            this->_rivalLifeGauge->decrease();

                            // 勝ち
                            if (this->_rivalLifeGauge->currentLife <= 0) {
                                this->_over(true);
                            }
                        }
                        break;

                    // 青いボールなら, ブロックの傷を直し, ライフゲージを増やす
                    case Ball::Blue:
                        if(block->fix()) {
                            if (youSide) {
                                this->_youLifeGauge->increase();
                            }
                            else {
                                this->_rivalLifeGauge->increase();
                            }
                        }
                        break;
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

void GameScene::_rivalCPU()
{
    //
    // ポリシー：バーに一番近いボールのx座標へバーを移動させる
    //

    // 視野の境界
    auto startY = this->_rivalBar->getPosition().y - this->_rivalBar->getContentSize().height / 2 - 64;
    auto endY = this->_rivalBar->getPosition().y - this->_rivalBar->getContentSize().height / 2;
    float nearestBallY = startY;
    float nearestBallX = this->_screenSize.width / 2;

    for (auto &ball : this->_balls) {
        // 視野に入っているボールの中から一番近いものを探す
        auto ballY = ball->getPosition().y;
        if (startY <= ballY && ballY < endY) {
            if (ballY > nearestBallY) {
                nearestBallY = ballY;
                nearestBallX = ball->getPosition().x;
            }
        }
    }

    // スピードを決めてその分だけ近づくようにする
    auto dragSpeed = 12;
    auto x = this->_rivalBar->getPosition().x;
    if (x > nearestBallX) {
        x -= dragSpeed;
        if (x < nearestBallX) {
            x = nearestBallX;
        }
    }
    else if (x < nearestBallX) {
        x += dragSpeed;
        if (x > nearestBallX) {
            x = nearestBallX;
        }
    }
    this->_rivalBar->cpuTouchMoved(Point(x, this->_rivalBar->getPosition().y));
}

void GameScene::_transition()
{
    TransitionCrossFade* transition = TransitionCrossFade::create(0.5f, TitleScene::createScene());
    Director::getInstance()->replaceScene(transition);
}
