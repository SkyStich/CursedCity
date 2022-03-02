// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Base/RangeWeaponObjectBase.h"
#include "BowWeaponBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class CURSEDCITY_API ABowWeaponBase : public ARangeWeaponObjectBase
{
	GENERATED_BODY()

public:

	ABowWeaponBase(const FObjectInitializer& ObjectInitializer);

protected:

	void SpawnBowProjectile();

	virtual bool UseWeapon() override;
	virtual void StopUseWeapon() override;
	virtual bool IsAbleToUseWeapon() override;
	virtual void StopRateDelay() override;

	UFUNCTION()
	virtual void ChargingComplete();

protected:

	FTimerHandle BowChargingHandle;
	bool bBowIsReadyToUse;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseBowProjectile> BowProjectileClass;
};
