// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/WeaponManagerComponent.h"
#include "Player/Character/CursedCityCharacter.h"
#include "Weapons/Base/BaseWeaponObject.h"

// Sets default values for this component's properties
UWeaponManagerComponent::UWeaponManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();	
}

ACursedCityCharacter* UWeaponManagerComponent::GetCharacterOwner() const
{
	return Cast<ACursedCityCharacter>(GetOwner());
}

EWeaponSlot UWeaponManagerComponent::FindSlotForAdd()
{
	if(Weapons.Num() <= 0) return EWeaponSlot::First;

	/** get last enum index */
	int32 const  LastIndex = int32(EWeaponSlot::Second);
	if(LastIndex == Weapons.Num())
	{
		if(!CurrentWeapon) return EWeaponSlot::None;
		return *Weapons.FindKey(CurrentWeapon);
	}

	/** find empty slot */
	int32 TempKey = -1;
	for(auto& ByArray : Weapons)
	{
		if(int32(ByArray.Key) > TempKey) TempKey = int32(ByArray.Key);
	}
	return EWeaponSlot(TempKey + 1);
}

void UWeaponManagerComponent::AddWeapon(ABaseWeaponObject* WeaponObject)
{
	EWeaponSlot const Slot = FindSlotForAdd();
	if(Slot == EWeaponSlot::None) return;
	
	if(Weapons.FindRef(Slot))
	{
		RemoveWeaponFromStorage(Slot);
	}
	Weapons.Add(Slot, WeaponObject);
	WeaponObject->SetActorEnableCollision(false);
	ChangeCurrentWeapon(WeaponObject);
	OnWeaponAdded.Broadcast(Slot, WeaponObject);
}

void UWeaponManagerComponent::RemoveWeaponFromStorage(EWeaponSlot Key)
{
	if(Key == EWeaponSlot::None) return;

	Weapons.Remove(Key);
}

void UWeaponManagerComponent::ChangeCurrentWeapon(ABaseWeaponObject* NewWeapon)
{
	CurrentWeapon = NewWeapon;
	OnCurrentWeaponChanged.Broadcast(NewWeapon);
}
