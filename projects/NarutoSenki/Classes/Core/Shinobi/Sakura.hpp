#pragma once
#include "Hero.hpp"

class Sakura : public Hero
{
	void perform() override
	{
		_mainTarget = nullptr;
		findHeroHalf();
		if (getCoinValue() >= 500 && !_isControlled && getGameLayer()->_enableGear)
		{
			if (getGearArray().size() == 0)
				setGear(gear00);
			else if (getGearArray().size() == 1)
				setGear(gear04);
			else if (getGearArray().size() == 2)
				setGear(gear08);
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
				if (_isCanGear00)
				{
					useGear(gear00);
				}
				if (stepBack())
					return;
			}
		}

		if (_mainTarget && (battleCondiction >= 0 || _isCanOugis1 || _isCanOugis2))
		{
			CCPoint moveDirection;
			CCPoint sp = getDistanceToTarget();

			if (isFreeActionState())
			{
				if (_isCanOugis2 && !_isControlled && getGameLayer()->_isOugis2Game && !_skillChangeBuffValue && _mainTarget->getGP() < 5000 && !_mainTarget->_isArmored && _mainTarget->getActionState() != State::KNOCKDOWN && !_mainTarget->_isSticking)
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
				else if (_isCanOugis1 && !_isControlled && _mainTarget->getGP() < 5000 && !_skillChangeBuffValue)
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
				else if (enemyCombatPoint > friendCombatPoint && abs(enemyCombatPoint - friendCombatPoint) > 3000 && !_isHealling && !_isControlled)
				{
					if (abs(sp.x) < 160)
						stepBack2();
					else
						idle();
					return;
				}
				else if (getHpPercent() < 0.9 && _isCanSkill1)
				{
					attack(SKILL1);
					return;
				}
				else if (abs(sp.x) < 128)
				{
					if ((abs(sp.x) > 64 || abs(sp.y) > 32) && (!_isCanSkill2 || _mainTarget->getGP() < 5000))
					{
						moveDirection = ccpNormalize(sp);
						walk(moveDirection);
						return;
					}
					else if (abs(sp.x) > 32 || abs(sp.y) > 32)
					{
						moveDirection = ccpNormalize(sp);
						walk(moveDirection);
						return;
					}

					if (_isCanSkill2 && !_skillChangeBuffValue && _mainTarget->getGP() < 5000)
					{
						changeSide(sp);
						attack(SKILL2);
					}
					else if (_isCanSkill3 && !_skillChangeBuffValue && getHpPercent() > 0.5)
					{
						changeSide(sp);
						attack(SKILL3);
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

		if (battleCondiction >= 0)
		{
			_mainTarget = nullptr;
			if (notFindFlogHalf())
			{
				findTowerHalf();
			}
		}
		else
		{
			_mainTarget = nullptr;
			findTowerHalf();
		}

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
				if (_isCanSkill1 && getHpPercent() < 0.9)
				{
					changeSide(sp);
					attack(SKILL1);
				}
				else if (_mainTarget->isFlog() && _isCanSkill2)
				{
					changeSide(sp);
					attack(SKILL2);
				}
				else if (_mainTarget->isTower() && _isCanSkill3 && !_skillChangeBuffValue)
				{
					changeSide(sp);
					attack(SKILL3);
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
			if (_isCanGear00)
				useGear(gear00);
			stepOn();
		}
	}
};
