#include "GameScene.h"

const char * bonus_image[] =
{
	"5.png",
	"10.png",
	"50.png",
	"100.png"
};

Scene* GameScene::createScene()
{
    return GameScene::create();
}

void GameScene::startGame()
{
	score = 0;

	resetClouds();
	resetPlatforms();
	resetBird();
	resetBonus();

	gameSuspended = false;
}

bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !HelloWorld::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    winSize = Director::getInstance()->getWinSize();
    origin = Director::getInstance()->getVisibleOrigin();  


	// Start off as game suspended
	gameSuspended = true;

	bird = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("bird.png"));
	this->addChild(bird, 4, kBird);

	// Initialize the platforms
	initPlatforms();

	// Create the bonus sprite
	Sprite* bonus;

	// Load in the bonus images, 5, 10, 50, 100
	for (int i = 0; i<kNumBonuses; i++)
	{
		bonus = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(bonus_image[i]));
		this->addChild(bonus, 4, kBonusStartTag + i);
		bonus->setVisible(false);
	}

	// Create the Score Label
	scoreLabel = Label::createWithBMFont("bitmapFont.fnt", "0");
	this->addChild(scoreLabel, 5, kScoreLabel);
	scoreLabel->setPosition(visibleSize.width / 2, visibleSize.height - 50);

	// Start the GameScene stepping
	schedule(schedule_selector(GameScene::step));

	// Enable the touch events
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan,this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded,this);
	getEventDispatcher()->addEventListenerWithFixedPriority(touchListener,1);
	
	// Enable accelerometer events
	Device::setAccelerometerEnabled(true);
	auto accelerometerListener = EventListenerAcceleration::create(
		CC_CALLBACK_1(GameScene::didAccelerate,this));
	getEventDispatcher()->addEventListenerWithFixedPriority(accelerometerListener, 1);
	
	// Start the game
	startGame();

	return true;
}

void GameScene::initPlatform()
{
	Sprite* platform;

	switch (rand() % 2)
	{
	case 0:
		platform = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("platform1.png"));
		this->addChild(platform, 3, currentPlatformTag);

		break;
	case 1:
		platform = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("platform2.png"));
		this->addChild(platform, 3, currentPlatformTag);
		break;
	}
}

void GameScene::resetPlatform()
{
	// We set this to -1 to initialize the first platform y coordinate
	if (currentPlatformY < 0)
	{
		currentPlatformY = static_cast<float>(kStartingCurrentPlatformY);
	}
	else
	{
		// If not the first one then randomly determine a y coordinate for the platform
		currentPlatformY += rand() % static_cast<int>(currentMaxPlatformStep - kMinPlatformStep) + kMinPlatformStep;

		// If the current platofmr step is less than the max (top of screen) then increment it
		if (currentMaxPlatformStep < kMaxPlatformStep)
		{
			currentMaxPlatformStep += 0.5f;
		}
	}

	auto platform = dynamic_cast<Sprite*>(getChildByTag(currentPlatformTag));

	if (rand() % 2 == 1) platform->setScaleX(-1.0f);

	float x;
	auto size = platform->getContentSize();

	// If the current platform is the first one initialized then just center it
	if (currentPlatformY == static_cast<float>(kStartingCurrentPlatformY))
	{
		x = static_cast<float>(Director::getInstance()->getWinSize().width) / 2;
	}
	else
	{
		x = rand() % static_cast<int>(
			Director::getInstance()->getWinSize().width - static_cast<int>(size.width)) + size.width / 2;
	}

	platform->setPosition(Vec2(x, currentPlatformY));
	platformCount++;

	// If the platform is to have to bonus then put it there.
	if (platformCount == currentBonusPlatformIndex)
	{
		auto bonus = static_cast<Sprite*>(getChildByTag(kBonusStartTag + currentBonusType));
		bonus->setPosition(Vec2(x, currentPlatformY + 30));
		bonus->setVisible(true);
	}

}

void GameScene::resetPlatforms()
{
	currentPlatformY = -1;						// Set the current platform y to -1 so it gets initialized
	currentPlatformTag = kPlatformsStartTag;	// set starting platform
	currentMaxPlatformStep = 60.0f;				// maximum space between platforms
	currentBonusPlatformIndex = 0;
	currentBonusType = 0;
	platformCount = 0;

	while(currentPlatformTag < kPlatformsStartTag + kNumPlatforms) 
	{
		resetPlatform();
		currentPlatformTag++;
	}
}

void GameScene::initPlatforms()
{
	currentPlatformTag = kPlatformsStartTag;
	while (currentPlatformTag < kPlatformsStartTag + kNumPlatforms)
	{
		initPlatform();
		currentPlatformTag++;
	}

	resetPlatforms();
}

void GameScene::resetBonus()
{
	auto bonus = static_cast<Sprite*>(getChildByTag(kBonusStartTag + currentBonusType));

	// Set the bonus to not be visible
	bonus->setVisible(false);

	// Randomly determine which platform will get the bonus next by adding a random amount
	currentBonusPlatformIndex += rand() % (kMaxBonusStep - kMinBonusStep) + kMinBonusStep;

	if (score < 10000)
	{
		currentBonusType = 0;
	}
	else if (score < 20000)
	{
		currentBonusType = rand() % 2;
	}
	else if (score < 10000)
	{
		currentBonusType = rand() % 3;
	}
	else
	{
		currentBonusType = rand() % 2 + 2;
	}
}

void GameScene::jump()
{
	bird_vel.y = 350.0f + fabsf(bird_vel.x);
}

void GameScene::resetBird()
{
	// Place bird in center
	bird_pos.x = static_cast<float>(Director::getInstance()->getWinSize().width) / 2;
	bird_pos.y = static_cast<float>(Director::getInstance()->getWinSize().width) / 2;
	bird->setPosition(bird_pos);

	bird_vel.x = 0;
	bird_vel.y = 0;

	bird_acc.x = 0;
	bird_acc.y = -550.0f;	// -250.0f makes the bird jump higher, -950.0f makes the bird jump lower

	birdLookingRight = true;
	bird->setScaleX(1.0f);
}

void GameScene::step(float dt)
{
	//	Log("Game::step");

	// Return if game suspended
	if (gameSuspended) return;

	// Update the player x position based on velocity and delta time
	bird_pos.x += bird_vel.x * dt;

	// Flip the player based on it's x velocity and current looking direction
	if (bird_vel.x < -30.0f && birdLookingRight)
	{
		birdLookingRight = false;
		bird->setScaleX(-1.0f);
	}
	else if (bird_vel.x > 30.0f && !birdLookingRight)
	{
		birdLookingRight = true;
		bird->setScaleX(1.0f);
	}

	// Calculate the max and min x values for the bird
	// based on the screen and bird widths
	auto bird_size = bird->getContentSize();
	float max_x = static_cast<float>(Director::getInstance()->getWinSize().width) - bird_size.width / 2;
	float min_x = bird_size.width / 2;

	// Limit the bird position based on max and min values allowed
	if (bird_pos.x>max_x) bird_pos.x = max_x;
	if (bird_pos.x<min_x) bird_pos.x = min_x;

	// Update the bird velocity based on acceleration and time
	bird_vel.y += bird_acc.y * dt;

	// Update the bird y positin based on velocity and time
	bird_pos.y += bird_vel.y * dt;

	////////////////////////////////////////////////////////////////////////////
	// Handle the bonus scoring
	auto bonus = static_cast<Sprite*>(getChildByTag(kBonusStartTag + currentBonusType));

	// If bonus is visible then see if the bird is within range to get the bonus
	if (bonus->isVisible())
	{
		auto bonus_pos = bonus->getPosition();
		float range = 20.0f;

		// If the player is within range of the bonus value then award the prize
		if (bird_pos.x > bonus_pos.x - range &&
			bird_pos.x < bonus_pos.x + range &&
			bird_pos.y > bonus_pos.y - range &&
			bird_pos.y < bonus_pos.y + range)
		{
			// Update score based on bonus
			switch (currentBonusType)
			{
			case kBonus5:   score += 5000;   break;
			case kBonus10:  score += 10000;  break;
			case kBonus50:  score += 50000;  break;
			case kBonus100: score += 100000; break;
			}

			// Build the score string to display
			char scoreStr[10] = { 0 };
			sprintf(scoreStr, "%d", score);
			scoreLabel->setString(scoreStr);

			// Highlight the score with some actions to celebrate the bonus win
			ActionInterval* a1 = ScaleTo::create(0.2f, 1.5f, 0.8f);
			ActionInterval* a2 = ScaleTo::create(0.2f, 1.0f, 1.0f);
			scoreLabel->runAction(Sequence::create(a1, a2, a1, a2, a1, a2, NULL));

			// Reset the bonus to another platform
			resetBonus();
		}
	}

	// If the bird has stopped moving then make it jump from the platform it is on
	int t;
	if (bird_vel.y < 0)
	{
		t = kPlatformsStartTag;

		// Search through all the platforms and compare the birds position with the platfor position
		for (t; t < kPlatformsStartTag + kNumPlatforms; t++)
		{
			auto platform = static_cast<Sprite*>(getChildByTag(t));

			auto platform_size = platform->getContentSize();
			auto platform_pos = platform->getPosition();

			max_x = platform_pos.x - platform_size.width / 2 - 10;
			min_x = platform_pos.x + platform_size.width / 2 + 10;

			auto min_y = platform_pos.y + (platform_size.height + bird_size.height) / 2 - kPlatformTopPadding;

			if (bird_pos.x > max_x &&
				bird_pos.x < min_x &&
				bird_pos.y > platform_pos.y &&
				bird_pos.y < min_y)
			{
				jump();
				break;	// Can only jump from one platform at a time to break out of the loop
			}
		}

		// If the bird has fallen below the screen then game over
		if (bird_pos.y < -bird_size.height / 2)
		{
			// [self showHighscores];   <== NEED TO IMPLEMENT THE HIGHTSCORE
			resetBird();	// Highscore not implmented yet so just keep on going.
		}
	}
	else if (bird_pos.y >(static_cast<float>(Director::getInstance()->getWinSize().height) / 2))
	{
		// If bird position is greater than the middle of the screen then move the platforms
		// the difference between the bird y position and middle point of the screen
		auto delta = bird_pos.y - (static_cast<float>(Director::getInstance()->getWinSize().height) / 2);

		// Set the bird y position to the middle of the screen
		bird_pos.y = (float)Director::getInstance()->getWinSize().height / 2;

		// Move the current platform y by the delta amount
		currentPlatformY -= delta;

		// Move the clouds vertically and reset if necessary
		t = kCloudsStartTag;
		for (t; t < kCloudsStartTag + kNumClouds; t++)
		{
			auto cloud = static_cast<Sprite*>(getChildByTag(t));

			auto pos = cloud->getPosition();

			// Calculate new position for cloud
			pos.y -= delta * cloud->getScaleY() * 0.8f;

			// If the cloud is off the screen then need to reset this cloud else set its new position
			if (pos.y < -cloud->getContentSize().height / 2)
			{
				currentCloudTag = t;
				resetCloud();
			}
			else
			{	// Update the new y position for the cloud.
				cloud->setPosition(pos);
			}
		}

		// Move the platforms vertically and reset if necessary
		t = kPlatformsStartTag;
		for (t; t < kPlatformsStartTag + kNumPlatforms; t++)
		{
			auto platform = static_cast<Sprite*>(getChildByTag(t));

			auto pos = platform->getPosition();

			// Calculate new position for platform
			pos = Vec2(pos.x, pos.y - delta);

			// If the platform is off the screen then reset the platform else set its new position
			if (pos.y < -platform->getContentSize().height / 2)
			{
				currentPlatformTag = t;
				resetPlatform();
			}
			else
			{
				platform->setPosition(pos);
			}
		}

		// If the bonus is visible then adjust it's y position
		if (bonus->isVisible())
		{
			auto pos = bonus->getPosition();

			// Calculate new position of bonus
			pos.y -= delta;

			// If the bonus is off the screen then reset the bonus else set its new position
			if (pos.y < -bonus->getContentSize().height / 2)
			{
				resetBonus();
			}
			else
			{
				bonus->setPosition(pos);
			}
		}

		// Update score based on how much the bird has moved
		score += static_cast<int>(delta);

		// Display the new score value
		char scoreStr[10] = { 0 };
		sprintf(scoreStr, "%d", score);
		scoreLabel->setString(scoreStr);
	}

	// Set the birds position
	bird->setPosition(bird_pos);
}

void GameScene::didAccelerate(Acceleration* acceleration)
{
	if (gameSuspended) return;

	auto accel_filter = 0.1f;
	bird_vel.x = bird_vel.x * accel_filter + acceleration->x * (1.0f - accel_filter) * 500.0f;
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}

void GameScene::onTouchEnded(Touch* touch, Event* event)
{
	auto touchLocation = touch->getLocationInView();
	touchLocation = Director::getInstance()->convertToGL(touchLocation);
	touchLocation = convertToNodeSpace(touchLocation);

	auto touchCenter = Director::getInstance()->getWinSize().width / 2 - touchLocation.x;
	auto accel_filter = 0.1f;
	bird_vel.x = bird_vel.x * accel_filter - touchCenter;
}
