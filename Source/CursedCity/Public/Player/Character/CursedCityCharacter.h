// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CursedCityCharacter.generated.h"

class UWeaponManagerComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS(config=Game)
class ACursedCityCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	/** Call on pressed start weapon used (left mouse button default) */
	UFUNCTION()
	void StartUseWeapon();

	/** Call on released stop weapon used (left mouse button default) */
	UFUNCTION()
	void StopUseWeapon();

	UFUNCTION()
	FHitResult GetHitResultForInteraction();

	UFUNCTION()
	void PressInteraction();

public:
	
	ACursedCityCharacter();

	/** returns WeaponManagerComponent subobject */
	FORCEINLINE UWeaponManagerComponent* GetWeaponManagerComponent() const { return WeaponManagerComponent; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	
public:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

private:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WeaponManager, meta = (AllowPrivateAccess = "true"))
	UWeaponManagerComponent* WeaponManagerComponent;
};

