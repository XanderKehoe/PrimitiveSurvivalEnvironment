#include "AnimalStateBase.h"

bool AnimalStateBase::CanSeeHuman(AnimalStateManager* manager, HumanAgentBase* humanAgent, bool alreadySeen)
{
	bool seeHuman = false;
	int modifiedSightRange = manager->animal->GetSightRange();

	if (humanAgent->IsOnStartVisibility())
		return false;

	if (humanAgent->IsHidingInBush() && !alreadySeen)
		return false;

	if (humanAgent->IsSneaking() && !alreadySeen)
		modifiedSightRange /= 2;
	else if (alreadySeen)
		modifiedSightRange *= 2;

	if (manager->animal->IsWithinSightRange(modifiedSightRange, humanAgent))
		seeHuman = true;

	return seeHuman;
}