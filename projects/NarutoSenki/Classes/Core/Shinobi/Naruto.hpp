#pragma once
#include "Hero.hpp"
#include "Shinobi/Bunshin/NarutoClone.hpp"
#include "Shinobi/Bunshin/SageNarutoClone.hpp"
#include "Shinobi/Bunshin/RikudoNarutoClone.hpp"
#include "Kuchiyose/Kurama.hpp"

class Naruto : public Hero
{
	void setID(const string &name, Role role, Group group) override
	{
		Hero::setID(name, role, group);

		match_char_exp3(HeroEnum::Naruto, setAIHandler(Naruto::perform),
						HeroEnum::SageNaruto, setAIHandler(Naruto::perform_SageNaruto),
						HeroEnum::RikudoNaruto, setAIHandler(Naruto::perform_RikudoNaruto));
	}

	void changeAction() override
	{
		match_char_exp3(HeroEnum::Naruto, changeAction_Naruto(),
						HeroEnum::SageNaruto, changeAction_SageNaruto(),
						HeroEnum::RikudoNaruto, changeAction_RikudoNaruto());
	}

	void resumeAction(float dt) override
	{
		match_char_exp3(HeroEnum::Naruto, resumeAction_Naruto(dt),
						HeroEnum::SageNaruto, resumeAction_SageNaruto(dt),
						HeroEnum::RikudoNaruto, resumeAction_RikudoNaruto(dt));
	}

	Hero *createClone(int cloneTime) override
	{
		match_char_exp3(HeroEnum::Naruto, return createClone_Naruto(cloneTime),
						HeroEnum::SageNaruto, return createClone_SageNaruto(cloneTime),
						HeroEnum::RikudoNaruto, return createClone_RikudoNaruto(cloneTime));
		return nullptr;
	}

	// Naruto

	void perform() override
	{
		_mainTarget = nullptr;
		findHeroHalf();

		tryBuyGear(gear03, gear07, gear02);

		if (needBackToTowerToRestoreHP() ||
			needBackToDefendTower())
			return;

		if (_mainTarget && _mainTarget->isNotFlog())
		{
			Vec2 moveDirection;
			Vec2 sp = getDistanceToTarget();

			if (isFreeActionState())
			{
				if (_isCanOugis2 && !_isControlled && getGameLayer()->_isOugis2Game && !_isArmored)
				{
					changeSide(sp);
					attack(OUGIS2);
					return;
				}
				else if (_isCanSkill2 && !_isArmored)
				{
					changeSide(sp);
					attack(SKILL2);
					return;
				}
				else if (enemyCombatPoint > friendCombatPoint && abs(enemyCombatPoint - friendCombatPoint) > 3000 && !_isHealing && !_isArmored && !_isControlled)
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
					else if (abs(sp.x) < 32 && (_isCanSkill1 || _isCanOugis1 || _isCanSkill3))
					{
						stepBack();
						return;
					}

					if (_isCanOugis1 && !_isControlled && _mainTarget->getGP() < 5000 && !_isArmored)
					{
						changeSide(sp);
						attack(OUGIS1);
					}
					else if (_isCanGear03)
					{
						useGear(gear03);
					}
					else if (_isCanSkill3 && !_isArmored && _mainTarget->getGP() < 5000)
					{
						changeSide(sp);
						attack(SKILL3);
					}
					else if (_isCanSkill1 && !_isArmored && _mainTarget->getGP() < 5000)
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
				if (_isCanSkill2 && !_isArmored)
				{
					changeSide(sp);
					attack(SKILL2);
					return;
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

	inline void changeAction_Naruto()
	{
		setIdleAction(createAnimation(skillSPC1Array, 5.0f, true, false));
		setWalkAction(createAnimation(skillSPC2Array, 10.0f, true, false));
		setNAttackAction(createAnimation(skillSPC3Array, 10.0f, false, true));
		setKnockDownAction(createAnimation(skillSPC4Array, 10.0f, false, true));

		setWalkSpeed(320);
		_originSpeed = 320;

		_isOnlySkillLocked = true;

		setTempAttackValue1(getNAttackValue());
		setNAttackValue(960);

		_nAttackRangeX = _spcAttackRangeX3;
		_nAttackRangeY = _spcAttackRangeY3;
		_originNAttackType = _nAttackType;
		_nAttackType = _spcAttackType3;

		_gardValue += 5000;
		_isArmored = true;

		lockOugisButtons();
	}

	inline void resumeAction_Naruto(float dt)
	{
		setIdleAction(createAnimation(idleArray, 5.0f, true, false));
		setWalkAction(createAnimation(walkArray, 10.0f, true, false));
		setNAttackAction(createAnimation(nattackArray, 10.0f, false, true));

		_isOnlySkillLocked = false;

		unlockOugisButtons();

		setWalkSpeed(224);
		_originSpeed = 224;
		if (hasTempAttackValue1())
		{
			setNAttackValue(getTempAttackValue1());
			setTempAttackValue1(0);
		}
		hasArmorBroken = false;

		_nAttackRangeX = 16;
		_nAttackRangeY = 48;
		_nAttackType = _originNAttackType;
		_gardValue -= 5000;
		_isArmored = false;

		if (_actionState != State::DEAD)
		{
			_actionState = State::WALK;
			knockDown();
		}

		setKnockDownAction(createAnimation(knockDownArray, 10.0f, false, true));
		CharacterBase::resumeAction(dt);
	}

	inline Hero *createClone_Naruto(int cloneTime)
	{
		auto clone = createCloneHero<NarutoClone>(getName());
		return clone;
	}

	// Naruto Sage

	void perform_SageNaruto()
	{
		_mainTarget = nullptr;
		findHeroHalf();

		tryBuyGear(gear03, gear07, gear02);

		if (needBackToTowerToRestoreHP() ||
			needBackToDefendTower())
			return;

		if (_mainTarget && _mainTarget->isNotFlog())
		{
			Vec2 moveDirection;
			Vec2 sp = getDistanceToTarget();

			if (isFreeActionState())
			{
				if (_isCanOugis2 && !_isControlled && getGameLayer()->_isOugis2Game && !_isArmored)
				{
					changeSide(sp);
					attack(OUGIS2);
					return;
				}
				else if (_isCanSkill2 && !_isArmored)
				{
					changeSide(sp);
					attack(SKILL2);
					return;
				}
				else if (_isCanSkill3 && !_isArmored && _mainTarget->getGP() < 5000)
				{
					changeSide(sp);
					attack(SKILL3);
					return;
				}
				else if (enemyCombatPoint > friendCombatPoint && abs(enemyCombatPoint - friendCombatPoint) > 3000 && !_isHealing && !_isArmored && !_isControlled)
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
					else if (abs(sp.x) < 32 && _isCanSkill1)
					{
						stepBack();
						return;
					}
					if (_isCanOugis1 && !_isControlled && _mainTarget->getGP() < 5000 && !_isArmored)
					{
						changeSide(sp);
						attack(OUGIS1);
					}
					else if (_isCanGear03)
					{
						useGear(gear03);
					}
					else if (_isCanSkill1 && !_isArmored && _mainTarget->getGP() < 5000)
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
				if (_isCanSkill2 && !_isArmored)
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

	inline void changeAction_SageNaruto()
	{
		_gardValue += 5000;
		_isArmored = true;
		hasArmorBroken = true;
		_isOnlySkillLocked = true;

		setIdleAction(createAnimation(skillSPC4Array, 5.0f, true, false));
		setWalkAction(createAnimation(skillSPC2Array, 10.0f, true, false));
		setNAttackAction(createAnimation(skillSPC3Array, 10.0f, false, true));

		setWalkSpeed(320);
		_originSpeed = 320;

		lockOugisButtons();
	}

	inline void resumeAction_SageNaruto(float dt)
	{
		setIdleAction(createAnimation(idleArray, 5.0f, true, false));
		setWalkAction(createAnimation(walkArray, 10.0f, true, false));
		setNAttackAction(createAnimation(nattackArray, 10.0f, false, true));

		_isOnlySkillLocked = false;

		unlockOugisButtons();

		setWalkSpeed(224);
		_originSpeed = 224;

		_gardValue -= 5000;
		_isArmored = false;
		hasArmorBroken = false;

		if (_actionState != State::DEAD)
		{
			_actionState = State::WALK;
			setKnockDownAction(createAnimation(skillSPC5Array, 10.0f, false, true));
			knockDown();
			setKnockDownAction(createAnimation(knockDownArray, 10.0f, false, true));
		}
		else if (hasTempAttackValue1())
		{
			setNAttackValue(getTempAttackValue1());
			setTempAttackValue1(0);
		}
		CharacterBase::resumeAction(dt);
	}

	inline Hero *createClone_SageNaruto(int cloneTime)
	{
		auto clone = createCloneHero<SageNarutoClone>(getName());
		clone->setSkill1Action(clone->createAnimation(clone->skillSPC1Array, 10.0f, false, true));
		clone->setSAttackValue1(getSAttackValue1());
		clone->setSAttackType1(_spcAttackType1);
		clone->_sAttackCD1 = _spcAttackCD1;
		return clone;
	}

	// Naruto Rikutou

	void perform_RikudoNaruto()
	{
		_mainTarget = nullptr;
		findHeroHalf();

		tryBuyGear(gear03, gear07, gear02);

		if (needBackToTowerToRestoreHP() ||
			needBackToDefendTower())
			return;

		if (_mainTarget && _mainTarget->isNotFlog())
		{
			Vec2 moveDirection;
			Vec2 sp = getDistanceToTarget();

			if (isFreeActionState())
			{
				if (_isCanOugis2 && !_isControlled && getGameLayer()->_isOugis2Game && !_isArmored)
				{
					changeSide(sp);
					attack(OUGIS2);
					return;
				}
				else if (_isCanSkill2 && !_isArmored)
				{
					changeSide(sp);
					attack(SKILL2);
					return;
				}
				else if (_isCanSkill3 && !_isArmored && _mainTarget->getGP() < 5000)
				{
					changeSide(sp);
					attack(SKILL3);
					return;
				}
				else if (_isCanSkill1 && !_isArmored && _mainTarget->getGP() < 5000)
				{
					changeSide(sp);
					attack(SKILL1);
					return;
				}
				else if (_isCanGear03)
				{
					useGear(gear03);
				}
				else if (enemyCombatPoint > friendCombatPoint && abs(enemyCombatPoint - friendCombatPoint) > 3000 && !_isHealing && !_isArmored && !_isControlled)
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
					if (_isCanOugis1 && !_isControlled && _mainTarget->getGP() < 5000 && !_isArmored)
					{
						changeSide(sp);
						attack(OUGIS1);
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
				if (_isCanSkill1 && !_isArmored && _mainTarget->isFlog())
				{
					changeSide(sp);
					attack(SKILL1);
				}
				else if (_isCanSkill2 && !_isArmored)
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

	inline void changeAction_RikudoNaruto()
	{
		_gardValue += 5000;
		_isArmored = true;
		hasArmorBroken = true;
		_isOnlySkillLocked = true;

		setIdleAction(createAnimation(skillSPC4Array, 5.0f, true, false));
		setWalkAction(createAnimation(skillSPC2Array, 10.0f, true, false));
		setNAttackAction(createAnimation(skillSPC3Array, 10.0f, false, true));

		if (getName() == HeroEnum::RikudoNaruto)
		{
			setTempAttackValue1(getNAttackValue());
			setNAttackValue(560);
		}

		setWalkSpeed(320);
		_originSpeed = 320;

		lockOugisButtons();
	}

	inline void resumeAction_RikudoNaruto(float dt)
	{
		setIdleAction(createAnimation(idleArray, 5.0f, true, false));
		setWalkAction(createAnimation(walkArray, 10.0f, true, false));
		setNAttackAction(createAnimation(nattackArray, 10.0f, false, true));

		_isOnlySkillLocked = false;

		unlockOugisButtons();

		setWalkSpeed(224);
		_originSpeed = 224;

		_gardValue -= 5000;
		_isArmored = false;
		hasArmorBroken = false;

		if (_actionState != State::DEAD)
		{
			_actionState = State::WALK;
			idle();
		}

		if (hasTempAttackValue1())
		{
			setNAttackValue(getTempAttackValue1());
			setTempAttackValue1(0);
		}
		CharacterBase::resumeAction(dt);
	}

	inline Hero *createClone_RikudoNaruto(int cloneTime)
	{
		auto clone = createCloneHero<Kurama>(SummonEnum::Kurama);
		clone->setGP(5000);
		clone->_isArmored = true;
		clone->setWalkSpeed(320);
		clone->_originSpeed = 320;
		clone->hasArmorBroken = true;
		return clone;
	}

private:
	string _originNAttackType;
};
