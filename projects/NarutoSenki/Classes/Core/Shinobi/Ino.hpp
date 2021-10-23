#pragma once
#include "Hero.hpp"

class Ino : public Hero
{
	void dead() override
	{
		CharacterBase::dead();

		// TODO: Support Ino controlled by the player can control other characters
		// if (isPlayer())
		// {
		// 	// Has controlled other hero
		// 	if (getGameLayer()->currentPlayer != this)
		// 	{
		// 		auto other = getGameLayer()->currentPlayer;
		// 		other->changeGroup();
		// 		other->doAI();

		// 		getGameLayer()->currentPlayer = this;
		// 		getGameLayer()->getHudLayer()->updateSkillButtons();
		// 	}
		// }
	}

	void perform() override
	{
		_mainTarget = nullptr;
		findHeroHalf();

		if (_skillChangeBuffValue)
		{
			return;
		}

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

		if (getCoin() >= 500 && !_isControlled && getGameLayer()->_enableGear)
		{
			if (getGearArray().size() == 0)
				setGear(gear06);
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

		if (getMaxHPValue() - getHPValue() >= 3000 &&
			getCoin() >= 50 && !_isHealling && _isCanItem1 && _isArmored)
		{
			setItem(Item1);
		}

		if (_mainTarget && _mainTarget->isNotFlog())
		{
			CCPoint moveDirection;
			CCPoint sp = getDistanceToTarget();

			if (isFreeActionState())
			{
				if (_isCanOugis2 && !_isControlled && getGameLayer()->_isOugis2Game)
				{
					if (abs(sp.x) > 96 || abs(sp.y) > 16)
					{
						moveDirection = ccpNormalize(sp);
						walk(moveDirection);
						return;
					}
					else
					{
						changeSide(sp);
						attack(OUGIS2);
					}

					return;
				}
				else if (_mainTarget->getGP() < 5000 && (_isCanSkill3 || _isCanSkill2))
				{
					if (abs(sp.x) > 96 || abs(sp.y) > 16)
					{
						moveDirection = ccpNormalize(sp);
						walk(moveDirection);
						return;
					}

					if (_isCanSkill2)
					{
						changeSide(sp);
						attack(SKILL2);
					}
					else if (_isCanSkill3)
					{
						changeSide(sp);
						attack(SKILL3);
					}

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
					if (abs(sp.x) > 32 || abs(sp.y) > 32)
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
					else if (_isCanSkill1)
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
				if (_mainTarget->isFlog() && _isCanSkill1)
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

	void resumeAction(float dt) override
	{
		if (!_isArmored)
			return;

		for (auto hero : getGameLayer()->_CharacterArray)
		{
			if (hero->_isControlled)
			{
				hero->_isControlled = false;
				if (hero->isPlayer())
				{
					hero->_isAI = false;
					hero->unschedule(schedule_selector(Ino::setAI));
					getGameLayer()->getHudLayer()->_isAllButtonLocked = false;
				}
				if (isPlayer())
				{
					// auto controlledHero = getGameLayer()->currentPlayer;
					// controlledHero->_isAI = true;
					// controlledHero->doAI();
					// _isControlled = false;

					// getGameLayer()->currentPlayer = this;
					getGameLayer()->controlChar = nullptr;
					// getGameLayer()->getHudLayer()->updateSkillButtons();
				}
				if (_actionState != State::DEAD)
				{
					idle();
				}
				hero->changeGroup();
				hero->setController(nullptr);
			}
		}

		_isArmored = false;
	}

	void setActionResume() override
	{
		unschedule(schedule_selector(Ino::resumeAction));

		for (auto hero : getGameLayer()->_CharacterArray)
		{
			if (hero->_isControlled)
			{
				hero->_isControlled = false;
				hero->changeGroup();
				if (hero->isPlayer())
				{
					hero->unschedule(schedule_selector(Ino::setAI));
					hero->_isAI = false;
					getGameLayer()->getHudLayer()->_isAllButtonLocked = false;
				}
				if (isPlayer())
				{
					getGameLayer()->controlChar = nullptr;
				}
				hero->setController(nullptr);
			}
		}

		_isArmored = false;
	}
};
