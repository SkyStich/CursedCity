// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Base/BaseWeaponObject.h"
#include "RangeWeaponObjectBase.generated.h"

USTRUCT(BlueprintType)
struct FWeaponAmmoData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
	int32 MaxAmmoInWeapon;
	
	UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
	int32 MaxAmmoInStorage;
	
	UPROPERTY()
	int32 CurrentAmmoInWeapon;

	UPROPERTY()
	int32 CurrentAmmoInStorage;
};

UCLASS(Abstract)
class CURSEDCITY_API ARangeWeaponObjectBase : public ABaseWeaponObject
{
	GENERATED_BODY()

public:

	ARangeWeaponObjectBase(const FObjectInitializer& ObjectInitializer);

protected:

	virtual bool UseWeapon() override;
	virtual void StopUseWeapon() override;
	virtual bool IsAbleToUseWeapon() override;
	virtual void StopRateDelay() override;

	/** find rotate and location for spawn projectile */
	void GetSpawnProjectileData(FVector& Location, FRotator& Rotate, const FName& SocketName = "Muzzle");

private:

	UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
	FWeaponAmmoData WeaponAmmoData;
};
