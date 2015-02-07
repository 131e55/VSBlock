//
//  Bar.h
//  VSBlock
//
//  Created by 131e55 on 2/5/15.
//
//

#ifndef __VSBlock__Bar__
#define __VSBlock__Bar__

#include "cocos2d.h"

class Bar : public cocos2d::Sprite
{
public:
    // バーが壊れているか
    bool broken = false;

    // bool youSide : 自分側のバーであるか
    static Bar* create(bool youSide);

    // バーのダメージの初期化
    void initialize();

    // プレイヤー用タッチイベントハンドラ
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

    // RIVAL用擬似タッチイベントハンドラ
    void cpuTouchBegan(cocos2d::Point p);
    void cpuTouchMoved(cocos2d::Point p);

    // プレイヤー, RIVAL共通の処理
    void touchMoved(cocos2d::Point p);

    // バーへのダメージ
    void hit();

private:
    // バーのテクスチャ数
    const int _numTextures = 8;

    // バーのテクスチャ
    std::vector<cocos2d::Texture2D*> _textures;

    // 現在のダメージ数
    int _currentDamage;

    // タッチされた位置
    cocos2d::Point _touchBeganPoint;
};
#endif /* defined(__VSBlock__Bar__) */
