// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Base/RangeWeaponObjectBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

ARangeWeaponObjectBase::ARangeWeaponObjectBase(const FObjectInitializer& ObjectInitializer)
{
	WeaponAmmoData.CurrentAmmoInStorage = WeaponAmmoData.MaxAmmoInStorage = 10;
	WeaponAmmoData.CurrentAmmoInWeapon = WeaponAmmoData.MaxAmmoInWeapon = 5;
}

bool ARangeWeaponObjectBase::IsAbleToUseWeapon()
{
	return Super::IsAbleToUseWeapon() && WeaponAmmoData.CurrentAmmoInWeapon > 0;
}

bool ARangeWeaponObjectBase::UseWeapon()
{
	Super::UseWeapon();

	return true;
}

void ARangeWeaponObjectBase::StopUseWeapon()
{
	Super::StopUseWeapon();
}

void ARangeWeaponObjectBase::StopRateDelay()
{
	Super::StopRateDelay();
}

void ARangeWeaponObjectBase::GetSpawnProjectileData(FVector& Location, FRotator& Rotate, const FName& SocketName)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetInstigatorController());
	if(!PlayerController) return;

	/** get viewport size */
	int32 Ox, Oy;
	PlayerController->GetViewportSize(Ox, Oy);

	/** get center viewport */
	Ox /= 2;
	Oy /= 2;

	/** get deproject screen position */
	FVector WorldLocation, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(Ox, Oy, WorldLocation, WorldDirection);

	/** Spawn single line trace for help find rotation on spawn projectile and ignored actors */
	FCollisionQueryParams Param;
	Param.AddIgnoredActor(GetOwner());
	FHitResult OutHit;
	GetWorld()->LineTraceSingleByChannel(OutHit, SkeletalMeshComponent->GetSocketLocation(SocketName),(WorldDirection * 99999.f) + WorldLocation, ECC_Visibility, Param);

	Location = PlayerController->GetCharacter()->GetMesh()->GetSocketLocation(SocketName);
	OutHit.GetActor() ? Rotate = UKismetMathLibrary::FindLookAtRotation(Location, OutHit.ImpactPoint)
	: Rotate = UKismetMathLibrary::FindLookAtRotation(Location, OutHit.TraceEnd);
}

