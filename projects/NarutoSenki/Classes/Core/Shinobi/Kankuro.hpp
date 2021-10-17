#pragma once
#include "Hero.hpp"
#include "Kuchiyose/Karasu.hpp"
#include "Kuchiyose/Sanshouuo.hpp"
#include "Kuchiyose/Saso.hpp"

class Kankuro : public Hero
{
	void setID(CCString *character, CCString *role, CCString *group) override
	{
		Hero::setID(character, role, group);

		getGameLayer()->onHUDInitialized(BIND(Kankuro::tryLockSkillButton));
	}

	void perform() override
	{
		_mainTarget = nullptr;
		findHeroHalf();

		if (_isCanGear06)
		{
			if ((_actionState == State::FLOAT ||
				 _actionState == State::AIRHURT ||
				 _actionState == State::HURT ||
				 _actionState == State::KNOCKDOWN) &&
				getHpPercent() < 0.5 && !_isArmored && !_isInvincible)
			{
				useGear(gear06);
			}
		}

		if (getCoinValue() >= 500 && !_isControlled && getGameLayer()->_enableGear)
		{
			if (getGearArray().size() == 0)
				setGear(gear06);
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
				if (stepBack())
					return;
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
				if (stepBack2())
					return;
			}
		}

		bool isFound1 = false;
		bool isFound2 = false;
		bool isFound3 = false;

		if (hasMonsterArrayAny())
		{
			for (auto mo : _monsterArray)
			{
				if (mo->isCharacter("Saso"))
				{
					isFound3 = true;
				}
				else if (mo->isCharacter("Sanshouuo"))
				{
					isFound2 = true;
				}
				else if (mo->isCharacter("Karasu"))
				{
					isFound1 = true;
				}
			}
		}

		if (_mainTarget && _mainTarget->isNotFlog())
		{
			CCPoint moveDirection;
			CCPoint sp = getDistanceToTarget();

			if (isFreeActionState())
			{
				if (_isCanSkill3)
				{
					changeSide(sp);
					attack(SKILL3);
					return;
				}
				else if (_isCanSkill1 && !isFound1 && !_isControlled)
				{
					attack(SKILL1);
					return;
				}
				else if (_isCanOugis1 && !_isControlled && !isFound2)
				{
					changeSide(sp);
					attack(OUGIS1);
					return;
				}
				else if (_isCanOugis2 && !_isControlled && getGameLayer()->_isOugis2Game && !isFound3)
				{
					changeSide(sp);
					attack(OUGIS2);
					return;
				}
				else if (enemyCombatPoint > friendCombatPoint && abs(enemyCombatPoint - friendCombatPoint) > 5000 && !_isHealling && !_isControlled)
				{
					if (abs(sp.x) < 160)
						stepBack2();
					else
						idle();
					return;
				}
				else if (abs(sp.x) < 128)
				{
					if (abs(sp.x) > 32 || abs(sp.y) > 32)
					{
						moveDirection = ccpNormalize(sp);
						walk(moveDirection);
						return;
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
				if (_isCanOugis1 && !_isControlled && !isFound2)
				{
					changeSide(sp);
					attack(OUGIS1);
				}
				else if (_isCanOugis2 && !_isControlled && getGameLayer()->_isOugis2Game && !isFound3)
				{
					changeSide(sp);
					attack(OUGIS2);
				}
				else if (_isCanSkill1 && !isFound1)
				{
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
		if (isPlayer())
		{
			getGameLayer()->getHudLayer()->skill1Button->setLock();
			getGameLayer()->getHudLayer()->skill2Button->unLock();
		}
	}

	void setActionResume() override
	{
		if (isPlayer())
		{
			getGameLayer()->getHudLayer()->skill1Button->unLock();
			getGameLayer()->getHudLayer()->skill2Button->setLock();
		}
		_skillChangeBuffValue = 0;
	}

	Hero *createClone(int cloneTime) override
	{
		Hero *clone = nullptr;

		if (cloneTime == 0)
		{
			clone = create<Karasu>(CCString::create("Karasu"), CCString::create(kRoleKugutsu), getGroup());
		}
		else if (cloneTime == 1)
		{
			clone = create<Sanshouuo>(CCString::create("Sanshouuo"), CCString::create(kRoleKugutsu), getGroup());
			if (isPlayer())
			{
				getGameLayer()->getHudLayer()->skill4Button->setLock();
			}
		}
		else if (cloneTime == 2)
		{
			clone = create<Saso>(CCString::create("Saso"), CCString::create(kRoleKugutsu), getGroup());
			if (isPlayer())
			{
				getGameLayer()->getHudLayer()->skill5Button->setLock();
			}
		}

		clone->_isArmored = true;
		_monsterArray.push_back(clone);
		return clone;
	}

private:
	inline void tryLockSkillButton()
	{
		if (isPlayer())
		{
			getGameLayer()->getHudLayer()->skill2Button->setLock();
		}
	}
};
