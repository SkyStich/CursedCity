// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponManagerComponent.generated.h"

class ACursedCityCharacter;
class ABaseWeaponObject;

UENUM(BlueprintType)
enum class EWeaponSlot : uint8 
{
	None,
	First,
	Second
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponAdded, EWeaponSlot, Slot, ABaseWeaponObject*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentWeaponChanged, ABaseWeaponObject*, Weapon);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEDCITY_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	EWeaponSlot FindSlotForAdd();

	/** get character owner helper */
	ACursedCityCharacter* GetCharacterOwner() const;

	/**
	 * replace current weapon on new weapon
	 *
	 * @param NewWeapon New currentWeapon
	 */
	void ChangeCurrentWeapon(ABaseWeaponObject* NewWeapon);

public:	
	// Sets default values for this component's properties
	UWeaponManagerComponent();

	/** return the current used weapon that the player uses */
	UFUNCTION(BlueprintPure, Category = "WeaponManager|Get")
	ABaseWeaponObject* GetCurrentWeapon() const { return CurrentWeapon; }

	/**
	 * Remove old weapon from storage
	 *
	 * @Key Key for remove
	 */
	void RemoveWeaponFromStorage(EWeaponSlot Key);

	/**
	 * Add new weapon on slot
	 * 
	 * @param Slot key for add
	 * @param WeaponObject weapon for add
	 */
	void AddWeapon(ABaseWeaponObject* WeaponObject);

	/** return current weapon array */
	TMap<EWeaponSlot, ABaseWeaponObject*> GetWeapons() const { return Weapons; }

public:

	/** it is executed when a new weapon is added to the inventory */
	UPROPERTY(BlueprintAssignable, Category = "WeaponManager|Delegates")
	FWeaponAdded OnWeaponAdded;

	UPROPERTY(BlueprintAssignable, Category = "WeaponManager|Delegates")
	FCurrentWeaponChanged OnCurrentWeaponChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	/** stores all the weapons that the player has with him */
	UPROPERTY()
	TMap<EWeaponSlot, ABaseWeaponObject*> Weapons;

	/** The current used weapon that the player uses */
	UPROPERTY()
	ABaseWeaponObject* CurrentWeapon;
};
