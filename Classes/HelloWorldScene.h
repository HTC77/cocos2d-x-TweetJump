/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#define kNumClouds			12
#define kNumPlatforms		10		// Number of platforms
 // Fist platform is 30 pixels from the bottom of the screen
#define kStartingCurrentPlatformY	30
#define kMinPlatformStep	50
#define kMaxPlatformStep	300		// This is the top of the screen (SHOULD BE RESOLUTION INDEPENDANT)
#define kMaxBonusStep		40
#define kMinBonusStep		20
#define kPlatformTopPadding 10


#include "cocos2d.h"
USING_NS_CC;
enum
{
	kCloudsStartTag = 100,
	kBird,
	kPlatformsStartTag = 200,
	kBonusStartTag = 300,
	kScoreLabel,
};
enum
{
	kBonus5 = 0,
	kBonus10,
	kBonus50,
	kBonus100,
	kNumBonuses
};
class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	Sprite* background;
    void step(float dt);
    void initCloud();
    void resetCloud();
    void resetClouds();
	void initClouds();
	int currentCloudTag;
	SpriteFrameCache* cache;
};

#endif // __HELLOWORLD_SCENE_H__
