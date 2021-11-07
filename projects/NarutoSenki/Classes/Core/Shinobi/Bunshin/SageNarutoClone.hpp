#pragma once
#include "Hero.hpp"

class SageNarutoClone : public Hero
{
	void perform() override
	{
		if (notFindHero(0))
		{
			if (notFindFlog(0))
			{
				if (notFindTower(0))
				{
					_mainTarget = nullptr;
				}
			}
		}

		if (_mainTarget)
		{
			Vec2 moveDirection;
			Vec2 sp = getDistanceToTarget();

			if (_mainTarget->isTower() ||
				_mainTarget->isFlog())
			{
				if (abs(sp.x) > 32 || abs(sp.y) > 32)
				{
					moveDirection = sp.getNormalized();
					walk(moveDirection);
				}
				else
				{
					if (isFreeActionState())
					{
						changeSide(sp);
						attack(NAttack);
					}
				}
				return;
			}
			else
			{
				if (abs(sp.x) > 128 || abs(sp.y) > 16)
				{
					moveDirection = sp.getNormalized();
					walk(moveDirection);
					return;
				}
				else if ((abs(sp.x) > 48 || abs(sp.y) > 8) && !_isCanSkill1)
				{
					moveDirection = sp.getNormalized();
					walk(moveDirection);
					return;
				}
				else if (isFreeActionState())
				{
					if (_isCanSkill1 && abs(sp.x) < 32)
					{
						stepBack();
						return;
					}

					if (_isCanSkill1 && _mainTarget->getGP() < 5000)
					{
						changeSide(sp);
						attack(SKILL1);
					}
					else
					{
						changeSide(sp);
						attack(NAttack);
					}
				}
				return;
			}
		}

		stepOn();
	}
};
