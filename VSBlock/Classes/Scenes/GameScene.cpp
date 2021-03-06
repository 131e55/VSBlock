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

    // 背景画像を生成
    auto background = Sprite::create("GameBackground.png");
    background->setPosition(this->_screenSize.width / 2, this->_screenSize.height / 2);
    this->addChild(background);

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

    // [デバッグ用] Rival が対象にしているボールへ付ける印
#ifdef __DEBUG_MODE__
        this->_lockon = Sprite::create("LockOn.png");
        this->addChild(this->_lockon);
#endif

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

    // ボールの出現数を初期化
    this->_numNextBalls = 5;
}

void GameScene::_start()
{
    // READY? ラベルをアニメーション表示させた後, ゲームスタート
    auto moveto = EaseBounceOut::create(MoveTo::create(
        1.0f,
        Point(this->_screenSize.width / 2, this->_screenSize.height / 2)
    ));
    auto fadeto = FadeTo::create(0.5f, 0);
    auto startAction = Sequence::create(
        moveto,
        fadeto,
        CallFunc::create([this]() {
            this->_newBalls();
            this->scheduleUpdate();
        }),
        NULL
    );
    this->_ready->runAction(startAction);
}

void GameScene::_over(bool youWin)
{
    // メインループを止める
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
        Point(this->_screenSize.width / 2, this->_screenSize.height / 2)
    ));
    auto fadeto = FadeTo::create(2.0f, 0);
    auto overAction = Sequence::create(
        moveto,
        fadeto,
        CallFunc::create([this]() {
            this->_transition();
        }),
        NULL
    );
    result->runAction(overAction);
}

// nextBallNumberの数になるまでボールを生成
void GameScene::_newBalls()
{
    auto num = this->_balls.size();
    for (int i = 0; i < this->_numNextBalls - (int)num; i ++) {
        auto ball = Ball::create();
        ball->setPosition(this->_screenSize.width / 2, this->_screenSize.height / 2);
        this->addChild(ball);
        this->_balls.push_back(ball);
    }
}

void GameScene::update(float frame) {
    // 全ボールの移動と衝突判定
    for (auto &ball : this->_balls) {
        // YOU Side
        if (ball->getPosition().y < this->_screenSize.height / 2) {
            // バーとの衝突判定
            if (!this->_youBar->broken) {
                this->_detectCollisionBallAndBar(ball, true);
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
                this->_detectCollisionBallAndBar(ball, false);
            }

            // ブロックとの衝突判定
            if(this->_detectCollisionBallAndBlocks(ball, false)) {
                break;
            }
        }

        // 壁との衝突判定
        this->_detectCollisionWalls(ball);

        // ボールの移動
        ball->move();
    }

    // Rival CPU
    this->_rivalCPU();
}

// ボールとバーの衝突判定処理
void GameScene::_detectCollisionBallAndBar(Ball *ball, bool youSide)
{
    auto barRect = youSide ? this->_youBar->getBoundingBox() : this->_rivalBar->getBoundingBox();
    auto x = ball->getPosition().x;
    auto r = ball->getContentSize().width / 2;

    if (ball->getBoundingBox().intersectsRect(barRect)) {
        // ボールを跳ね返す
        if (ball->vy < 0) { // top
            ball->vy *= -1;
            ball->setPosition(x, barRect.getMaxY() + r);
        }
        else if (ball->vy > 0) { // bottom
            ball->vy *= -1;
            ball->setPosition(x, barRect.getMinY() - r);
        }

        // 青いボールはバーがダメージを受ける
        if (ball->type == Ball::Blue) {
            if (youSide) {
                this->_youBar->hit();
            }
            else {
                this->_rivalBar->hit();
            }
        }
    }
}

// ボールとブロックの衝突判定処理
// 戻り値: ボールとバーが衝突したかどうか
bool GameScene::_detectCollisionBallAndBlocks(Ball *ball, bool youSide)
{
    for (auto &block : (youSide ? this->_youBlocks : this->_rivalBlocks)) {
        if (!block->broken) {
            auto blockRect = block->getBoundingBox();

            if (ball->getBoundingBox().intersectsRect(blockRect)) {
                switch (ball->type) {
                    // 白いボールなら, ブロックに傷を入れ, ライフゲージを減らす
                    case Ball::White:
                    {
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

                        // ボール出現数を増やす
                        this->_numNextBalls ++;
                        if (this->_numNextBalls > 50) {
                            this->_numNextBalls = 50;
                        }

                        break;
                    }

                    // 青いボールなら, ブロックの傷を直し, ライフゲージを増やす
                    case Ball::Blue:
                    {
                        // 傷のあるブロックを探し直す
                        for (auto &block2 : (youSide ? this->_youBlocks : this->_rivalBlocks)) {
                            if(block2->fix()) {
                                if (youSide) {
                                    this->_youLifeGauge->increase();
                                }
                                else {
                                    this->_rivalLifeGauge->increase();
                                }
                                break;
                            }
                        }

                        // ボール出現数を減らす
                        this->_numNextBalls -= 3;
                        if (this->_numNextBalls < 5) {
                            this->_numNextBalls = 5;
                        }

                        break;
                    }

                    default:
                        break;
                }

                // ボールを削除
                ball->removeFromParent();
                auto pos = std::find(this->_balls.begin(), this->_balls.end(), ball);
                this->_balls.erase(pos);

                // 新しいボールを生成
                this->_newBalls();

                return true;
            }
        }
    }
    return false;
}

// ボールと壁との衝突判定
void GameScene::_detectCollisionWalls(Ball *ball)
{
    auto x = ball->getPosition().x;
    auto y = ball->getPosition().y;
    auto r = ball->getContentSize().width / 2;

    if (x - r <= 0) { // left
        ball->setPosition(r, ball->getPosition().y);
        ball->vx *= -1;
    }
    else if (x + r >= this->_screenSize.width) { // right
        ball->setPosition(this->_screenSize.width - r, ball->getPosition().y);
        ball->vx *= -1;
    }
    else if (y - r <= 0) { // bottom
        ball->setPosition(ball->getPosition().x, r);
        ball->vy *= -1;
    }
    else if(y + r >= this->_screenSize.height) { // top
        ball->setPosition(ball->getPosition().x, this->_screenSize.height - r);
        ball->vy *= -1;
    }
}

void GameScene::_rivalCPU()
{
    /*
     * Rival の戦略
     *  1-1. 視野内で, バーに一番近く, ブロックへ向かっているボールのx座標に注目する
     *    2-1. 白いボールならその位置へ近づく
     *    2-2. 青いボールならその位置から遠ざかる
     *  1-2. 視野内で, ブロックへ向かっているボールがなかったら中央へ向かう
     */

    // タッチ開始位置はバーの位置とする
    this->_rivalBar->cpuTouchBegan(this->_rivalBar->getPosition());

    // 視野を決めて、視野以外のボールに対しては走査しないようにする
    auto length = 128;
    auto startY = this->_rivalBar->getPosition().y;
    auto endY = startY - length;

    auto x = this->_rivalBar->getBoundingBox().getMidX();

    // 初期値は最低に
    float nearestBallY = endY;
    float nearestBallX = this->_screenSize.width / 2;

    // 対象のボールが見つかったか
    bool found = false;

    // 対象のボールのタイプ
    int type;

    // スピードを決めてその分だけ移動するようにする
    auto dragSpeed = 10;

    // 全ボール走査
    for (auto &ball : this->_balls) {
        auto ballY = ball->getPosition().y;

        // 1. 視野内で, バーに一番近く, ブロックへ向かっているボールを探す
        if (ball->vy > 0 && startY > ballY && ballY >= endY) {
            if (ballY > nearestBallY) {
                nearestBallY = ballY;
                nearestBallX = ball->getPosition().x;
                type = ball->type;
                found = true;
            }
        }
    }

    // 視野内で, ブロックへ向かっているボールがあれば
    if (found) {
        switch (type) {
            // 白いボールなら目的地へ近づく
            case Ball::White:
            {
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
                break;
            }

            // 青いボールなら目的地から遠ざかる
            case Ball::Blue:
            {
                if (x > nearestBallX) {
                    x += dragSpeed;
                }
                else if (x < nearestBallX) {
                    x -= dragSpeed;
                }
                break;
            }

            default:
                break;
        }
    }
    else {
        auto centerX = this->_screenSize.width / 2;
        if (x < centerX) {
            x += dragSpeed;
            if (x > centerX) {
                x = centerX;
            }
        }
        else {
            x -= dragSpeed;
            if (x < centerX) {
                x = centerX;
            }
        }
    }
    this->_rivalBar->cpuTouchMoved(Point(x, this->_rivalBar->getPosition().y));

#ifdef __DEBUG_MODE__
    // [デバッグ用] Rival が対象にしているボールへ付ける印
    this->_lockon->setPosition(nearestBallX, nearestBallY);
#endif
}

void GameScene::_transition()
{
    TransitionCrossFade* transition = TransitionCrossFade::create(0.5f, TitleScene::createScene());
    Director::getInstance()->replaceScene(transition);
}
