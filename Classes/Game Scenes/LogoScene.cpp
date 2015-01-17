//
//  LogoScene.cpp
//  KawazLogo
//
//  Created by CollapsedPlug on 1/7/15.
//
//

#include "LogoScene.h"
#include "CBlocks_acf.h"
#include "logo.h"
#include "MainScene.h"

USING_NS_CC;

LogoScene::LogoScene()
: _frog(nullptr)
, _K(nullptr)
, _a1(nullptr)
, _w(nullptr)
, _a2(nullptr)
, _z(nullptr)
, _clearout(nullptr)
, _logo(nullptr)
, _cueSheet(nullptr)
, _finalScale(0.0f)
, _hasEnded(false)
{
    
}

LogoScene::~LogoScene(){
    
    CC_SAFE_RELEASE_NULL(_frog);
    CC_SAFE_RELEASE_NULL(_K);
    CC_SAFE_RELEASE_NULL(_a1);
    CC_SAFE_RELEASE_NULL(_w);
    CC_SAFE_RELEASE_NULL(_a2);
    CC_SAFE_RELEASE_NULL(_z);
    CC_SAFE_RELEASE_NULL(_clearout);
    CC_SAFE_RELEASE_NULL(_logo);
    CC_SAFE_RELEASE_NULL(_cueSheet);
    
}

Scene* LogoScene::createScene(){
    auto scene = Scene::create();
    auto layer = LogoScene::create();
    scene->addChild(layer);
    return scene;
}

bool LogoScene::init(){
    if(!Layer::init()){
        return false;
    }
    
    auto cuesheet = ADX2::CueSheet::create("adx2/CBlocks.acf", "adx2/logo.acb");
    this->setCueSheet(cuesheet);
    
    auto director = Director::getInstance();
    
    auto isLandScape = director->getWinSize().width >= director->getWinSize().height;
    
    this->setFinalScale((isLandScape ? 0.70f : 0.55f));
    
    //各パーツを読み込み、準備
    auto frog = Sprite::create("logo/Parts/Frog.png");
    frog->setScale(50.0f);
    frog->setPosition(director->getWinSize() / 2);
    
    auto K = Sprite::create("logo/Parts/K.png");
    K->setScale(_finalScale);
    K->setPosition(Vec2(director->getWinSize().width * (isLandScape ? 0.25f : 0.1f) +
                        (frog->getContentSize().width / 2 + K->getContentSize().width / 2) * _finalScale +
                        (isLandScape ? 0.0f : director->getWinSize().width * 0.05),
                        director->getWinSize().height * 1.5));
    auto a1 = Sprite::create("logo/Parts/a.png");
    a1->setScale(_finalScale);
    a1->setPosition(Vec2(K->getPositionX() +
                        (K->getContentSize().width / 2 + a1->getContentSize().width / 2) * _finalScale,
                        director->getWinSize().height * 1.5));
    auto w = Sprite::create("logo/Parts/w.png");
    w->setScale(_finalScale);
    w->setPosition(Vec2(a1->getPositionX() +
                        (a1->getContentSize().width / 2 + w->getContentSize().width / 2) * _finalScale,
                        director->getWinSize().height * 1.5));
    auto a2 = Sprite::create("logo/Parts/a.png");
    a2->setScale(_finalScale);
    a2->setPosition(Vec2(w->getPositionX() +
                         (w->getContentSize().width / 2 + a2->getContentSize().width / 2) * _finalScale,
                         director->getWinSize().height * 1.5));
    auto z = Sprite::create("logo/Parts/z.png");
    z->setScale(_finalScale);
    z->setPosition(Vec2(a2->getPositionX() +
                         (a2->getContentSize().width / 2 + z->getContentSize().width / 2) * _finalScale,
                         director->getWinSize().height * 1.5));

    auto clearout = Sprite::create("logo/Clearout.png");
    clearout->setScale(0.0f);
    clearout->setPosition(director->getWinSize() / 2); //クソ横着なことをしたが反省はしていない
    
    //縦か横は画面の縦横の長さどちらが長いのかで決定
    auto logo = (director->getWinSize().width > director->getWinSize().height ? Sprite::create("logo/1334-750.png") : Sprite::create("logo/750-1334.png"));
    logo->setScale(30.0f);
    logo->setOpacity(0); //Opacityは0から255
    logo->setPosition(director->getWinSize() / 2);
    
    this->setFrog(frog);
    this->setK(K);
    this->setA1(a1);
    this->setW(w);
    this->setA2(a2);
    this->setZ(z);
    this->setClearOut(clearout);
    this->setLogo(logo);
    this->addChild(frog);
    this->addChild(K);
    this->addChild(a1);
    this->addChild(w);
    this->addChild(a2);
    this->addChild(z);
    this->addChild(clearout);
    this->addChild(logo);
    
    return true;
}

void LogoScene::onEnterTransitionDidFinish(){
    
    auto director = Director::getInstance();
    
    auto isLandScape = (director->getWinSize().width >= director->getWinSize().height);
    auto firstDelay = DelayTime::create(0.5f);
    
    //カエルの部分のアニメーション　ズームアウト後、画面の左端から画面の幅の20%(縦画面なら12.5%)のところに陣取る
    auto FRanimationSound = CallFunc::create([this](){
        this->getCueSheet()->playCueByID(CRI_LOGO_FROG);
    });
    auto FRanimation = ScaleTo::create(1.0f, _finalScale);
    auto FRanimationEL = EaseOut::create(FRanimation, 3.0f);
    auto FRanimationEP = EaseOut::create(FRanimation, 3.0f);
    auto FRanimation2_1 = MoveBy::create(1.0f, Vec2::ZERO); //最初は動かない(ことを明示)(Delaytimeでもよい)
    auto FRanimation2_2 = MoveBy::create(1.2f, Vec2(-director->getWinSize().width * (isLandScape ? 0.25f : 0.375f), 0));
    auto FRanimation2_2EIO = EaseInOut::create(FRanimation2_2, 7.0f);
    auto FRanimation2 = Sequence::create(FRanimation2_1, FRanimation2_2EIO, NULL);
    if (isLandScape){
        auto FrSeq = Spawn::create(FRanimationSound, FRanimationEL, FRanimation2, NULL);
        this->getFrog()->runAction(Sequence::create(firstDelay, FrSeq, NULL));
    }
    else{
        auto FrSeq = Spawn::create(FRanimationSound, FRanimationEP, FRanimation2, NULL);
        this->getFrog()->runAction(Sequence::create(firstDelay, FrSeq, NULL));
    }

    auto wait4Frog = DelayTime::create(2.5f); //カエルアニメーションが終わるのを待つ
    
    //まずアニメーションを全て作成する
    //音の再生タイミングは落ちた時。
    auto FallSound = CallFunc::create([this](){
        this->getCueSheet()->playCueByID(CRI_LOGO_LETTERANDMAIN);
    });
    //落っこちて(落っこちる距離は確定していて画面の高さ分)
    auto FallActio = MoveBy::create(0.2f, Vec2(0, -director->getWinSize().height));
    auto FallAction = EaseIn::create(FallActio, 2.0f);
    //その際に重力で引き延ばされて
    auto FallStretch = ScaleBy::create(0.1f,1.0f, 1.2f);
    auto Fall = Spawn::create(FallAction, FallStretch, NULL);
    //着地した時ゼリーみたいに震えさせてみたり
    auto LandTremble = EaseElasticOut::create((ScaleTo::create(0.5f,_finalScale)));
    
    /* あとは適用をはじめる
     * FIXME : (本当は各スプライトに対してそれぞれ設定したかったのだが、
     * 複数のノードに個別のActionをそれぞれのスタックに一気に乗せすぎると
     * ActionManager内でTargetがNULLになってゲームが落ちるとか
     * Actionが実行されないとか予期しない動きをする謎バグを踏んだため
     * やむなくラムダを入れ子にした。非常にまずいコードであるのは承知の上だが)
     */
    auto KAction = Sequence::create(wait4Frog, Fall, Spawn::create(FallSound, LandTremble,CallFunc::create([this, Fall, LandTremble](){
        this->getA1()->runAction(Sequence::create(Fall, Spawn::create(LandTremble,CallFunc::create([this, Fall, LandTremble](){
            this->getW()->runAction(Sequence::create(Fall, Spawn::create(LandTremble,CallFunc::create([this, Fall, LandTremble](){
                this->getA2()->runAction(Sequence::create(Fall, Spawn::create(LandTremble,CallFunc::create([this, Fall, LandTremble](){
                    this->getZ()->runAction(Sequence::create(Fall, LandTremble, NULL));
                }), NULL), NULL));
            }), NULL), NULL));
        }), NULL), NULL));
    }), NULL), NULL);
    _K->runAction(KAction);
    
    /* 本来のコードがこちら。こちらの方がラムダを入れ子にするより見易いのだが、
     これを実行すると、なぜかクラスMoveByのメンバstartWithTargetが、その引数Target = NULLの状態で呼ばれて、
     ゲームが落ちた
     auto KAction = Sequence::create(wait4Frog, FallSequence, NULL);
     _K->runAction(KAction);
     auto a1Action = Sequence::create(wait4Frog, DelayTime::create(0.2f), FallSequence, NULL);
     _a1->runAction(a1Action);
     auto wAction = Sequence::create(wait4Frog, DelayTime::create(0.4f), FallSequence, NULL);
     _w->runAction(wAction);
     auto a2Action = Sequence::create(wait4Frog, DelayTime::create(0.6f), FallSequence, NULL);
     _a2->runAction(a2Action);
     auto zAction = Sequence::create(wait4Frog, DelayTime::create(0.8f), FallSequence, NULL);
     _z->runAction(zAction);
     */
    
    auto coExpand = EaseIn::create(ScaleTo::create(1.2f, 400.0f), 3.0f);
    auto LogoSpawn = CallFunc::create([this,director](){
        auto ShrinkInto = ScaleTo::create(0.4f, 1.0f / director->getContentScaleFactor());
        auto appear = FadeTo::create(0.4f, 255);
        this->getLogo()->runAction(Sequence::create(DelayTime::create(0.2f),Spawn::create(ShrinkInto,appear, NULL), NULL));
    });
    this->getClearOut()->runAction(Sequence::create(wait4Frog, DelayTime::create(1.3f),Spawn::create(coExpand, LogoSpawn, NULL), DelayTime::create(2.0f), CallFunc::create([this](){
        this->setHasEnded(true);
                                                            }), NULL));
    
    this->scheduleUpdate();
    
}

void LogoScene::update(float dt){
    
    
    if (_hasEnded){
        
        /* TODO : ここで次のシーンへ遷移します。行き先のシーンのヘッダファイルをIncludeするのをわすれずに
         * Example : 以下の例はこのシーンをリピートします。
         * [Beginning of code]
         * this->unscheduleUpdate();
         * auto scene = LogoScene::createScene();
         * auto sceneTr = TransitionFade::create(scene);
         * Director::getInstance()->replaceScene(sceneTr);
         * [End of code]
         * 注意 : this->unscheduleUpdate();を実行しないとsceneが生成され続けて予期しない結果を生むことがあります。
         */
        
        this->unscheduleUpdate();
        auto scene = MainScene::createScene();
        auto sceneTr = TransitionFade::create(0.5f, scene);
        Director::getInstance()->replaceScene(sceneTr);
        
    }
    
    
}