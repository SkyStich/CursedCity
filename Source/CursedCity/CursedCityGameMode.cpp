// Copyright Epic Games, Inc. All Rights Reserved.

#include "CursedCityGameMode.h"
#include "Player/Character/CursedCityCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACursedCityGameMode::ACursedCityGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
