// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/StreamableManager.h"
#include "UObject/NoExportTypes.h"
#include "GameSingletonClass.generated.h"

/**
 * 
 */
UCLASS()
class CURSEDCITY_API UGameSingletonClass : public UObject
{
	GENERATED_BODY()

public:

	static UGameSingletonClass& Get();

	FStreamableManager AssetLoader;
};
