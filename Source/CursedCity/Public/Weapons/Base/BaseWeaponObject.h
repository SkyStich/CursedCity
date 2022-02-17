// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponObject.generated.h"

UCLASS(Abstract)
class CURSEDCITY_API ABaseWeaponObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeaponObject();

	/** causes the logic of using weapons. Return true if weapon be used */
	virtual bool UseWeapon();
	
	/** causes the logic of stopping the use of weapons */
	virtual void StopUseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** puts the weapon in a state where it is ready for use */
	virtual void StopRateDelay();

	/** checks the terms of use of the weapon . Returns true if the weapon can be used */
	virtual bool IsAbleToUseWeapon();

protected:

	UPROPERTY()
	bool bWeaponUsed;

private:

	/** the timer is responsible for the transition of weapons from not ready to use to ready */
	UPROPERTY()
	FTimerHandle UseWeaponHandle;

	/** the time it takes for the weapon to return to its ready-to-use state */
	UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
	float TimeBeforeUsed;
};
