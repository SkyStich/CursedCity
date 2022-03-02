// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Base/BowWeaponBase.h"
#include "Actors/Projectiles/Ammo/BaseBowProjectile.h"

ABowWeaponBase::ABowWeaponBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bBowIsReadyToUse = false;
}

bool ABowWeaponBase::IsAbleToUseWeapon()
{
	return Super::IsAbleToUseWeapon() && !GetWorld()->GetTimerManager().IsTimerActive(BowChargingHandle) && !bBowIsReadyToUse;
}

bool ABowWeaponBase::UseWeapon()
{
	if(!IsAbleToUseWeapon()) return false;
	
	GetWorld()->GetTimerManager().SetTimer(BowChargingHandle, this, &ABowWeaponBase::ChargingComplete, 1.f);
		
#if UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("StartUseBow"));
#endif

	return true;
}

void ABowWeaponBase::ChargingComplete()
{
	GetWorld()->GetTimerManager().ClearTimer(BowChargingHandle);
	bBowIsReadyToUse = true;
#if UE_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("ChargingComplete"));
#endif
}

void ABowWeaponBase::StopRateDelay()
{
	Super::StopRateDelay();

#if UE_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("StopRateDelay"));
#endif
}

void ABowWeaponBase::StopUseWeapon()
{
	if(bBowIsReadyToUse)
	{
		SpawnBowProjectile();
		GetWorld()->GetTimerManager().SetTimer(UseWeaponHandle, this, &ABowWeaponBase::StopRateDelay, 1.f);
		Super::StopUseWeapon();
		bBowIsReadyToUse = false;
	}
}

void ABowWeaponBase::SpawnBowProjectile()
{
	FVector Location;
	FRotator Rotation;
	GetSpawnProjectileData(Location, Rotation);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = GetInstigator();
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto const Projectile = GetWorld()->SpawnActor<ABaseBowProjectile>(BowProjectileClass, Location, Rotation, SpawnParameters);
}