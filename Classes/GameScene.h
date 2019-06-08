#ifndef __TweetJump__GameScene__
#define __TweetJump__GameScene__

#include "cocos2d.h"
#include "HelloWorldScene.h"
USING_NS_CC;

class GameScene : public HelloWorld
{
public:
    static Scene* createScene();


    void startGame();
    virtual bool init();
    
    Size visibleSize;
    Size winSize;
    Vec2 origin;
	Label* scoreLabel;
	Sprite* bird;
    CREATE_FUNC(GameScene);
	bool gameSuspended;
    void initPlatform();
    void resetPlatform();
    void resetPlatforms();
	void initPlatforms();
	int currentPlatformTag;
	float currentPlatformY;
	float currentMaxPlatformStep;
	int currentBonusPlatformIndex;
	int currentBonusType;
	int platformCount;

    void resetBonus();
    void jump();
    void resetBird();
	void step(float dt);
	Vec2 bird_pos;
	Vec2 bird_vel;
	Vec2 bird_acc;
	bool birdLookingRight;
	int score;
	void didAccelerate(Acceleration* acceleration);
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
};
#endif // __TweetJump__GameScene__
