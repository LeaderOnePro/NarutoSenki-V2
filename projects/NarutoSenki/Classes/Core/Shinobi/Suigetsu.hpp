#pragma once
#include "Hero.hpp"

class Suigetsu : public Hero
{
	void perform() override
	{
		_mainTarget = nullptr;
		findHeroHalf();

		if (canBuyGear())
		{
			if (getGearArray().size() == 0)
				setGear(gear00);
			else if (getGearArray().size() == 1)
				setGear(gear05);
			else if (getGearArray().size() == 2)
				setGear(gear01);
		}

		if (checkRetri())
		{
			if (_mainTarget != nullptr)
			{
				if (stepBack2())
					return;
			}
			else
			{
				if (stepBack())
				{
					if (_isCanGear00)
					{
						useGear(gear00);
					}
					return;
				}
			}
		}

		if (isBaseDanger && checkBase() && !_isControlled)
		{
			bool needBack = false;
			if (isAkatsukiGroup())
			{
				if (getPositionX() < 85 * 32)
					needBack = true;
			}
			else
			{
				if (getPositionX() > 11 * 32)
					needBack = true;
			}

			if (needBack)
			{
				if (_isCanGear00)
				{
					useGear(gear00);
				}

				if (stepBack2())
					return;
			}
		}

		if (_mainTarget && _mainTarget->isNotFlog())
		{
			CCPoint moveDirection;
			CCPoint sp = getDistanceToTarget();

			if (isFreeActionState())
			{
				if (_isCanOugis2 && !_isControlled && getGameLayer()->_isOugis2Game && !_isArmored && _mainTarget->getGP() < 5000 && !_mainTarget->_isArmored && _mainTarget->getActionState() != State::KNOCKDOWN && !_mainTarget->_isSticking)
				{
					if (abs(sp.x) > 48 || abs(sp.y) > 16)
					{
						if (_isCanGear00)
						{
							useGear(gear00);
						}
						moveDirection = ccpNormalize(sp);
						walk(moveDirection);
						return;
					}

					changeSide(sp);
					attack(OUGIS2);
					return;
				}
				else if (_isCanOugis1 && !_isControlled && _mainTarget->getGP() < 5000 && !_isArmored)
				{
					if (abs(sp.x) > 48 || abs(sp.y) > 16)
					{
						if (_isCanGear00)
						{
							useGear(gear00);
						}
						moveDirection = ccpNormalize(sp);
						walk(moveDirection);
						return;
					}

					changeSide(sp);
					attack(OUGIS1);
					return;
				}
				else if (_isCanSkill3 && _mainTarget->getGP() < 5000 && !_isArmored && !_skillChangeBuffValue)
				{
					changeSide(sp);
					attack(SKILL3);
					return;
				}
				else if (enemyCombatPoint > friendCombatPoint && abs(enemyCombatPoint - friendCombatPoint) > 3000 && !_isHealling && !_isControlled)
				{
					if (abs(sp.x) < 160)
						stepBack2();
					else
						idle();
					return;
				}
				else if (abs(sp.x) < 128)
				{
					if (abs(sp.x) > 96 || abs(sp.y) > 32)
					{
						moveDirection = ccpNormalize(sp);
						walk(moveDirection);
						return;
					}
					else if ((abs(sp.x) > 32 || abs(sp.y) > 32) && !_isArmored)
					{
						moveDirection = ccpNormalize(sp);
						walk(moveDirection);
						return;
					}

					if (_isCanSkill2 && _mainTarget->getGP() < 5000 && !_isArmored)
					{
						changeSide(sp);
						attack(SKILL2);
					}
					else if (_isCanSkill1 && _mainTarget->getGP() < 5000 && !_isArmored)
					{
						changeSide(sp);
						attack(SKILL1);
					}
					else
					{
						changeSide(sp);
						attack(NAttack);
					}

					return;
				}
			}
		}
		_mainTarget = nullptr;
		if (notFindFlogHalf())
			findTowerHalf();

		if (_mainTarget)
		{
			CCPoint moveDirection;
			CCPoint sp = getDistanceToTarget();

			if (abs(sp.x) > 32 || abs(sp.y) > 32)
			{
				moveDirection = ccpNormalize(sp);
				walk(moveDirection);
				return;
			}

			if (isFreeActionState())
			{
				if (_mainTarget->isFlog() && _isCanSkill1 && !_isArmored)
				{
					changeSide(sp);
					attack(SKILL1);
				}
				else if (_isCanSkill2 && _mainTarget->getGP() < 5000 && !_isArmored && _mainTarget->isFlog() && isBaseDanger)
				{
					changeSide(sp);
					attack(SKILL2);
				}
				else
				{
					changeSide(sp);
					attack(NAttack);
				}
			}
			return;
		}

		if (_isHealling && getHpPercent() < 1)
		{
			if (isFreeActionState())
				idle();
		}
		else
		{
			stepOn();
		}
	}

	void changeAction() override
	{
		settempAttackValue1(to_ccstring(getNAttackValue()));
		setnAttackValue(to_ccstring(1960));

		_isOnlySkillLocked = true;

		_nattackRangeX = 16;
		_nattackRangeY = 48;

		_isArmored = true;

		setIdleAction(createAnimation(skillSPC1Array, 5.0f, true, false));
		setWalkAction(createAnimation(skillSPC2Array, 10.0f, true, false));
		setNAttackAction(createAnimation(skillSPC3Array, 10.0f, false, true));

		lockOugisButtons();
	}

	void setActionResume() override
	{
		_isArmored = false;
		if (hasTempAttackValue1())
		{
			setnAttackValue(to_ccstring(getTempAttackValue1()));
			settempAttackValue1(nullptr);
		}
		_isOnlySkillLocked = false;
		unlockOugisButtons();

		_nattackRangeX = 16;
		_nattackRangeY = 48;

		setIdleAction(createAnimation(idleArray, 5.0f, true, false));
		setWalkAction(createAnimation(walkArray, 10.0f, true, false));
		setNAttackAction(createAnimation(nattackArray, 10.0f, false, true));

		_skillChangeBuffValue = 0;
	}
};
