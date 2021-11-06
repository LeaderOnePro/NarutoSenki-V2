#pragma once
#include "cocos2d.h"
#include "Utils/CCDeprecated.h"

using namespace cocos2d;

inline Sequence *newSequence(FiniteTimeAction *pAction1, FiniteTimeAction *pAction2)
{
	return Sequence::createWithTwoActions(pAction1, pAction2);
}

inline Sequence *newSequence(FiniteTimeAction *pAction1, FiniteTimeAction *pAction2, FiniteTimeAction *pAction3)
{
	return Sequence::create(pAction1, pAction2, pAction3, nullptr);
}

inline Sequence *newSequence(FiniteTimeAction *pAction1, FiniteTimeAction *pAction2, FiniteTimeAction *pAction3, FiniteTimeAction *pAction4)
{
	return Sequence::create(pAction1, pAction2, pAction3, pAction4, nullptr);
}

inline Sequence *newSequence(FiniteTimeAction *pAction1, FiniteTimeAction *pAction2, FiniteTimeAction *pAction3, FiniteTimeAction *pAction4, FiniteTimeAction *pAction5)
{
	return Sequence::create(pAction1, pAction2, pAction3, pAction4, pAction5, nullptr);
}

inline Sequence *newSequence(FiniteTimeAction *pAction1, FiniteTimeAction *pAction2, FiniteTimeAction *pAction3, FiniteTimeAction *pAction4, FiniteTimeAction *pAction5, FiniteTimeAction *pAction6)
{
	return Sequence::create(pAction1, pAction2, pAction3, pAction4, pAction5, pAction6, nullptr);
}
