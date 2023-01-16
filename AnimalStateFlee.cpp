#include "AnimalStateFlee.h"
#include "PathFinder.h"
#include "Vector2DFloat.h"

AnimalStateFlee::AnimalStateFlee(AnimalStateManager* manager, HumanAgentBase* humanAgent)
{
	this->manager = manager;
	this->humanAgent = humanAgent;

	pathInitialized = false;
}

AnimalStateFlee::~AnimalStateFlee()
{
}

void AnimalStateFlee::EnterState()
{
	pathInitialized = false;
	// printf("Flee State Entered\n");
}

void AnimalStateFlee::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	int animalX = manager->animal->GetGridXPos();
	int animalY = manager->animal->GetGridYPos();
	int humanX = humanAgent->GetGridXPos();
	int humanY = humanAgent->GetGridYPos();

	if (targetPath.empty())
	{
		if (PathFinder::Dist(animalX, animalY, humanX, humanY) > (int) (manager->animal->GetSightRange() * 1.5))
		{
			// can no longer see target, resume normal wandering
			// printf("Resuming wandering state %d %d\n", PathFinder::Dist(animalX, animalY, humanX, humanY), (int) (manager->animal->GetSightRange() * 1.5));
			manager->ChangeState(manager->wanderState);
		}
		else
		{
			// can still see target, generate a new flee path
			// printf("Still see human, run!! %d %d\n", PathFinder::Dist(animalX, animalY, humanX, humanY), (int)(manager->animal->GetSightRange() * 1.5));
			UpdatePath(manager, level, humanAgent);
		}
	}
	else if (!pathInitialized) 
	{
		UpdatePath(manager, level, humanAgent);
		pathInitialized = true;
	} 
	else while (manager->animal->CanMove() && !targetPath.empty())
	{
		DirectionType nextDirection = targetPath.back();
		targetPath.pop_back();

		bool success = manager->animal->Move(nextDirection, false, level);
		if (!success)
		{
			UpdatePath(manager, level, humanAgent);
			break;
		}
	}
}

void AnimalStateFlee::PostEvent(AnimalEventType eventType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
}

void AnimalStateFlee::UpdatePath(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent)
{
	
	Vector2DFloat humanCoords((float) humanAgent->GetGridXPos(), (float) humanAgent->GetGridYPos());
	Vector2DFloat thisCoords((float) manager->animal->GetGridXPos(), (float) manager->animal->GetGridYPos());

	if (abs(thisCoords.x() - humanCoords.x()) > Config::LEVEL_SIZE)
		printf("WARNING - AnimalStateFlee::UpdatePath() Abnormally high X coord difference %f %f\n", thisCoords.x(), humanCoords.x());
	if (abs(thisCoords.y() - humanCoords.y()) > Config::LEVEL_SIZE)
		printf("WARNING - AnimalStateFlee::UpdatePath() Abnormally high Y coord difference %f %f\n", thisCoords.y(), humanCoords.y());
	if (thisCoords.x() - humanCoords.x() == 0 && thisCoords.y() - humanCoords.y() == 0) 
	{
		printf("WARNING - AnimalStateFlee::UpdatePath() X/Y coords overlap, returning this function early...\n");
		return;
	}

	Vector2DFloat unitVector((float) (thisCoords.x() - humanCoords.x()), (float) (thisCoords.y() - humanCoords.y()));
	unitVector.normalize();

	int fleeDistance = (int) (manager->animal->GetSightRange() * 1.5);

	// try running straight away, but if not possible (off the map or target tile is a 'blocking' tile), slowly rotate the unit vector and try again
	bool foundPath = false;
	for (float d = 0; d < 120; d += 5) 
	{
		if (d == 0) 
		{
			int fleeX = (int) thisCoords.x() + (int) (unitVector.x() * fleeDistance);
			int fleeY = (int) thisCoords.y() + (int) (unitVector.y() * fleeDistance);

			if (TryNewPath(level, manager->animal->GetGridXPos(), manager->animal->GetGridYPos(), fleeX, fleeY))
				break;
		}
		else 
		{
			Vector2DFloat unitVectorRotatedPos(unitVector.x(), unitVector.y());
			unitVectorRotatedPos.rotate(d);

			int fleePosX = (int)thisCoords.x() + (int)(unitVectorRotatedPos.x() * fleeDistance);
			int fleePosY = (int)thisCoords.y() + (int)(unitVectorRotatedPos.y() * fleeDistance);

			if (TryNewPath(level, manager->animal->GetGridXPos(), manager->animal->GetGridYPos(), fleePosX, fleePosY))
				break;

			Vector2DFloat unitVectorRotatedNeg(unitVector.x(), unitVector.y());
			unitVectorRotatedNeg.rotate(-d);

			int fleeNegX = (int)thisCoords.x() + (int)(unitVectorRotatedNeg.x() * fleeDistance);
			int fleeNegY = (int)thisCoords.y() + (int)(unitVectorRotatedNeg.y() * fleeDistance);

			if (TryNewPath(level, manager->animal->GetGridXPos(), manager->animal->GetGridYPos(), fleeNegX, fleeNegY))
				break;
		}
	}
}

bool AnimalStateFlee::TryNewPath(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], int startX, int startY, int endX, int endY)
{
	if (startX < 0 || startX > Config::LEVEL_SIZE - 1 ||
		startY < 0 || startY > Config::LEVEL_SIZE - 1 ||
		endX < 0 || endX > Config::LEVEL_SIZE - 1 ||
		endY < 0 || endY > Config::LEVEL_SIZE - 1)
		return false;

	level[endX][endY]->debug = true;

	if (!level[endX][endY]->IsWalkable(false))
		return false;

	targetPath = PathFinder::GetPath(level, startX, startY, endX, endY);
	if (!targetPath.empty())
		return true;
	else
		return false;
}

