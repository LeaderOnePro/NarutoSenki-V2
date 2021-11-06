#pragma once
#include "Hero.hpp"

class Tobirama : public Hero
{
	void perform() override
	{
		_mainTarget = nullptr;
		findHeroHalf();

		tryUseGear6();
		tryBuyGear(gear06, gear01, gear07);

		if (needBackToTowerToRestoreHP() ||
			needBackToDefendTower())
			return;

		if (_mainTarget && _mainTarget->isNotFlog())
		{
			Vec2 moveDirection;
			Vec2 sp = getDistanceToTarget();

			if (isFreeActionState())
			{
				if (_isCanOugis2 && !_isControlled && getGameLayer()->_isOugis2Game && _mainTarget->getGP() < 5000)
				{
					changeSide(sp);
					attack(OUGIS2);
				}
				else if (_isCanSkill1 && !_mainTarget->_isArmored && _mainTarget->isFreeActionState())
				{
					changeSide(sp);
					attack(SKILL1);
					return;
				}
				else if (_isCanSkill2 && _mainTarget->getGP() < 5000)
				{
					if ((abs(sp.x) > 64 || abs(sp.y) > 16))
					{
						moveDirection = ccpNormalize(sp);
						walk(moveDirection);
						return;
					}
					changeSide(sp);
					attack(SKILL2);
					return;
				}
				else if (enemyCombatPoint > friendCombatPoint && abs(enemyCombatPoint - friendCombatPoint) > 3000 && !_isHealing && !_isControlled)
				{
					if (abs(sp.x) < 160)
						stepBack2();
					else
						idle();
					return;
				}
				else if (abs(sp.x) < 128)
				{
					if (abs(sp.x) > 48 || abs(sp.y) > 32)
					{
						moveDirection = ccpNormalize(sp);
						walk(moveDirection);
						return;
					}

					if (_isCanOugis1 && !_isControlled && _mainTarget->getGP() < 5000)
					{
						changeSide(sp);
						attack(OUGIS1);
					}
					else if (_isCanSkill3)
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
		_mainTarget = nullptr;
		if (notFindFlogHalf())
			findTowerHalf();

		if (_mainTarget)
		{
			Vec2 moveDirection;
			Vec2 sp = getDistanceToTarget();

			if (abs(sp.x) > 32 || abs(sp.y) > 32)
			{
				moveDirection = ccpNormalize(sp);
				walk(moveDirection);
				return;
			}

			if (isFreeActionState())
			{
				if (_isCanSkill3 && _mainTarget->isFlog() && isBaseDanger)
				{
					changeSide(sp);
					attack(SKILL3);
				}
				else if (_isCanSkill2 && _mainTarget->isFlog() && isBaseDanger)
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

		checkHealingState();
	}
};
