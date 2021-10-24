#pragma once
#include "UI/GameModeLayer.h"

class ModeMenuButton : public CCSprite, public CCTouchDelegate
{
private:
	CCSprite *lockMask;

public:
	CC_SYNTHESIZE(GameModeLayer *, _gameModeLayer, Delegate);

	GameMode mode;
	bool useMask2;

	bool init(const string &szImage)
	{
		RETURN_FALSE_IF(!CCSprite::initWithFile(szImage.c_str()));
		// initWithSpriteFrameName(szImage);
		setAnchorPoint(ccp(0.5, 0.5));

		return true;
	}

	void onEnter()
	{
		CCSprite::onEnter();
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 10, true);
	}

	void onExit()
	{
		CCSprite::onExit();
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	}

	CCRect getRect()
	{
		CCSize size = getContentSize();
		return CCRect(0, 0, size.width, size.height);
	}

	bool ccTouchBegan(CCTouch *touch, CCEvent *event)
	{
		return getRect().containsPoint(convertTouchToNodeSpace(touch));
	}

	void ccTouchEnded(CCTouch *touch, CCEvent *event)
	{
		SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/confirm.ogg");
		_gameModeLayer->selectMode(mode);
	}

	bool isLocked()
	{
		return lockMask != nullptr;
	}

	void lock()
	{
		if (lockMask == nullptr)
		{
			auto maskPath = useMask2 ? "GameMode/chain_mask2.png" : "GameMode/chain_mask.png";
			lockMask = CCSprite::create(maskPath);
			lockMask->setPosition(getPosition());
			_gameModeLayer->addChild(lockMask, 1000);
		}
	}

	void unlock()
	{
		if (lockMask)
		{
			_gameModeLayer->removeChild(lockMask, true);
			lockMask = nullptr;
		}
	}

	static ModeMenuButton *create(const string &szImage)
	{
		ModeMenuButton *mb = new ModeMenuButton();
		if (mb && mb->init(szImage))
		{
			mb->autorelease();
			return mb;
		}
		else
		{
			delete mb;
			return nullptr;
		}
	}
};
