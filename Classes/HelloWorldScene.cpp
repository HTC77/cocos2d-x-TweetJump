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

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"

Scene* HelloWorld::createScene()
{
	auto layer = GameScene::create();
	auto scene = Scene::create();
	scene->addChild(layer);
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

	// Add SpriteSheet with file
	cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("spritesheet.plist");
	auto backgroundFrame = cache->getSpriteFrameByName("background.png");

	// Get background from sprite sheet
	background = Sprite::createWithSpriteFrame(backgroundFrame);
	this->addChild(background);
	background->setPosition(visibleSize / 2);

	// Initialize the clouds
	initClouds();

	// Start stepping
	schedule(schedule_selector(HelloWorld::step));


	return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::step(float dt)
{
	int t = kCloudsStartTag;
	for (t; t < kCloudsStartTag + kNumClouds; t++)
	{
		auto cloud = static_cast<Sprite*>(getChildByTag(t));
		auto pos = cloud->getPosition();
		auto size = cloud->getContentSize();

		// Control speed of clouds moving based on their size
		pos.x += 0.1f * cloud->getScaleY();

		if (pos.x > Director::getInstance()->getWinSize().width + size.width / 2)
		{
			pos.x = -size.width / 2;;
		}
		cloud->setPosition(pos);
	}
}

void HelloWorld::initCloud()
{
	Sprite *cloud;
	switch (rand() % 3)
	{
	case 0:
		// Get cloud from sprite sheet
		cloud = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("cloud1.png"));
		break;
	case 1:
		// Get cloud from sprite sheet
		cloud = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("cloud2.png"));
		break;
	case 2:
		// Get cloud from sprite sheet
		cloud = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("cloud3.png"));
		break;
	}

	this->addChild(cloud, 3, currentCloudTag);
	cloud->setOpacity(128);
}

void HelloWorld::resetCloud()
{
	auto *cloud = static_cast<Sprite*>(getChildByTag(currentCloudTag));

	// Calculate a random distance for this cloud
	float distance = rand() % 20 + 5;

	// Scale the cloud based on the distance
	float scale = 5.0f / distance;

	// Scale the X and Y equally for the cloud
	cloud->setScaleX(scale);
	cloud->setScaleY(scale);

	// randomly invert the X scale for some of the clouds
	if (rand() % 2 == 1) cloud->setScaleX(-cloud->getScaleX());

	auto size = cloud->getContentSize();

	float scaled_width = size.width * scale;

	// Randomly place each cloud within our view 
	float x = rand() % static_cast<int>(Director::getInstance()->getWinSize().width + static_cast<int>(scaled_width)) - scaled_width / 2;
	float y = rand() % static_cast<int>(Director::getInstance()->getWinSize().height - static_cast<int>(scaled_width)) + scaled_width / 2 + Director::getInstance()->getWinSize().height;

	cloud->setPosition(Vec2(x, y));
}

void HelloWorld::resetClouds()
{
	currentCloudTag = kCloudsStartTag;

	while (currentCloudTag < kCloudsStartTag + kNumClouds)
	{
		resetCloud();

		auto cloud = static_cast<Sprite*>(getChildByTag(currentCloudTag));
		auto pos = cloud->getPosition();
		pos.y -= static_cast<float>(Director::getInstance()->getWinSize().height);
		cloud->setPosition(pos);
		currentCloudTag++;
	}
}

void HelloWorld::initClouds()
{
	currentCloudTag = kCloudsStartTag;

	// Create the clouds and add them to the layer
	while (currentCloudTag < kCloudsStartTag + kNumClouds)
	{
		initCloud();
		currentCloudTag++;
	}

	// randomly size and place each cloud
	resetClouds();
}
