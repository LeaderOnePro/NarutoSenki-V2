#pragma once
#include "CharacterBase.h"
#include "HPBar.h"

class Flog : public CharacterBase
{
public:
	float _mainPosY = 0;
	float _randomPosX = 0;
	float _randomPosY = 0;
#if COCOS2D_DEBUG
	string _targetName;
#endif

	CREATE_FUNC(Flog);

	bool init()
	{
		RETURN_FALSE_IF(!Sprite::init());

		setAnchorPoint(Vec2(0.5, 0));
		scheduleUpdate();

		return true;
	}

	void setID(const string &name, Role role, Group group)
	{
		clearActionData();
		setName(name);
		setRole(role);
		setGroup(group);

		CCArray *animationArray = CCArray::create();
		auto filePath = format("Unit/Flog/{}.xml", name);
		KTools::readXMLToArray(filePath, animationArray);

		CCArray *tmpAction = (CCArray *)(animationArray->objectAtIndex(0));
		CCArray *tmpData = (CCArray *)(tmpAction->objectAtIndex(0));
		idleArray = (CCArray *)(tmpAction->objectAtIndex(1));

		string unitName;
		uint32_t maxHP;
		int tmpWidth;
		int tmpHeight;
		uint32_t tmpSpeed;
		int tmpCombatPoint;

		readData(tmpData, unitName, maxHP, tmpWidth, tmpHeight, tmpSpeed, tmpCombatPoint);

		setMaxHPValue(maxHP, false);
		setHPValue(maxHP, false);
		setCKR(0);
		setCKR2(0);
		setHeight(tmpHeight);
		setWalkSpeed(tmpSpeed);

		setKillNum(0);

		// init WalkFrame
		tmpAction = (CCArray *)(animationArray->objectAtIndex(1));
		walkArray = (CCArray *)(tmpAction->objectAtIndex(1));

		// init HurtFrame
		tmpAction = (CCArray *)(animationArray->objectAtIndex(2));
		hurtArray = (CCArray *)(tmpAction->objectAtIndex(1));

		// init AirHurtFrame
		tmpAction = (CCArray *)(animationArray->objectAtIndex(3));
		airHurtArray = (CCArray *)(tmpAction->objectAtIndex(1));

		// init KnockDownFrame
		tmpAction = (CCArray *)(animationArray->objectAtIndex(4));
		knockDownArray = (CCArray *)(tmpAction->objectAtIndex(1));

		// init FloatFrame
		tmpAction = (CCArray *)(animationArray->objectAtIndex(5));
		floatArray = (CCArray *)(tmpAction->objectAtIndex(1));

		// init DeadFrame
		tmpAction = (CCArray *)(animationArray->objectAtIndex(6));
		deadArray = (CCArray *)(tmpAction->objectAtIndex(1));

		// init nAttack data & Frame Array
		tmpAction = (CCArray *)(animationArray->objectAtIndex(7));
		tmpData = (CCArray *)(tmpAction->objectAtIndex(0));

		uint32_t tmpValue;
		uint32_t tmpCD;
		readData(tmpData, _nAttackType, tmpValue, _nAttackRangeX, _nAttackRangeY, tmpCD, tmpCombatPoint);
		setNAttackValue(tmpValue);
		nattackArray = (CCArray *)(tmpAction->objectAtIndex(1));

		setCoin(50);

		initAction();
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CharacterBase::acceptAttack), "acceptAttack", nullptr);
	}

	void initAction()
	{
		setIdleAction(createAnimation(idleArray, 5, true, false));
		setWalkAction(createAnimation(walkArray, 10, true, false));
		setHurtAction(createAnimation(hurtArray, 10, false, true));

		setAirHurtAction(createAnimation(airHurtArray, 10, false, false));
		setKnockDownAction(createAnimation(knockDownArray, 10, false, true));
		setDeadAction(createAnimation(deadArray, 10, false, false));

		setFloatAction(createAnimation(floatArray, 10, false, false));

		setNAttackAction(createAnimation(nattackArray, 10, false, true));
	}

	void setHPbar()
	{
		if (getGroup() != getGameLayer()->currentPlayer->getGroup())
			_hpBar = HPBar::create("flog_bar_r.png");
		else
			_hpBar = HPBar::create("flog_bar.png");
		_hpBar->setPositionY(getHeight());
		_hpBar->setDelegate(this);
		addChild(_hpBar);
	}

protected:
	void dealloc()
	{
		setState(State::DEAD);
		unscheduleAllSelectors();
		stopAllActions();

		if (isKonohaGroup())
			std::erase(getGameLayer()->_KonohaFlogArray, this);
		else
			std::erase(getGameLayer()->_AkatsukiFlogArray, this);

		removeFromParent();
	}

	void setAI(float dt)
	{
		if (isFreeState())
		{
			if (!_randomPosY)
			{
				_randomPosY = rand() % 8 + 4;
				_randomPosX = rand() % 32 + 20;
			}

			bool hasTarget = false;

			if (_mainTarget)
			{
				if (_mainTarget->getState() != State::DEAD && !_mainTarget->_isInvincible && _mainTarget->_isVisable &&
					_mainTarget->getGroup() != getGroup())
				{
					hasTarget = true;
				}
			}

			if (!hasTarget)
			{
				if (notFindFlog(0))
				{
					if (notFindHero(64))
					{
						if (notFindTower(0))
						{
							_mainTarget = nullptr;
						}
					}
				}
			}
#if COCOS2D_DEBUG
			if (_mainTarget)
				_targetName = _mainTarget->getName();
#endif

			Vec2 moveDirection;

			if (_mainTarget)
			{
				Vec2 sp = getDistanceToTargetAndIgnoreOriginY();
				if (abs(sp.x) > _randomPosX || abs(sp.y) > _randomPosY)
				{
					if (abs(sp.x) > 64 && _mainTarget->isNotFlog() && _mainTarget->isNotTower())
					{
						_mainTarget = nullptr;
						return;
					}

					moveDirection = sp.getNormalized();
					walk(moveDirection);
				}
				else
				{
					if (_state != State::NATTACK)
					{
						changeSide(sp);
						attack(NAttack);
					}
				}
				return;
			}

			// no target then step on
			if (abs(getPositionY() - _mainPosY) > 8)
			{
				if (isKonohaGroup())
					moveDirection = Vec2(1, getPositionY() > _mainPosY ? -1 : 1).getNormalized();
				else
					moveDirection = Vec2(-1, getPositionY() > _mainPosY ? -1 : 1).getNormalized();
			}
			else
			{
				if (isKonohaGroup())
					moveDirection = Vec2(1, 0).getNormalized();
				else
					moveDirection = Vec2(-1, 0).getNormalized();
			}
			walk(moveDirection);
		}
	}
};
