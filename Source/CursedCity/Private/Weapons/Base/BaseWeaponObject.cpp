// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Base/BaseWeaponObject.h"
#include "Player/Character/CursedCityCharacter.h"
#include "Player/Components/WeaponManagerComponent.h"

// Sets default values
ABaseWeaponObject::ABaseWeaponObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	TimeBeforeUsed = 1.f;
}

// Called when the game starts or when spawned
void ABaseWeaponObject::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ABaseWeaponObject::IsAbleToUseWeapon()
{
	return !GetWorld()->GetTimerManager().IsTimerActive(UseWeaponHandle);
}

bool ABaseWeaponObject::UseWeapon()
{
	if(!IsAbleToUseWeapon()) return false;

	bWeaponUsed = true;
	GetWorld()->GetTimerManager().SetTimer(UseWeaponHandle, this, &ABaseWeaponObject::StopRateDelay, TimeBeforeUsed, false);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("USED!"));
	return true;
}

void ABaseWeaponObject::StopRateDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(UseWeaponHandle);
}

void ABaseWeaponObject::StopUseWeapon()
{
	bWeaponUsed = false;
}

void ABaseWeaponObject::InteractionWithObject_Implementation(ACursedCityCharacter* Character)
{
	if(!Character) return;
	Character->GetWeaponManagerComponent()->AddWeapon(this);
}

