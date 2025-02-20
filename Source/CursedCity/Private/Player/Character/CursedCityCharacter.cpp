// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/Character/CursedCityCharacter.h"
#include "DrawDebugHelpers.h"
#include "Player/Components/WeaponManagerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapons/Base/BaseWeaponObject.h"

//////////////////////////////////////////////////////////////////////////
// ACursedCityCharacter

ACursedCityCharacter::ACursedCityCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	WeaponManagerComponent = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManagerComponent"));
}

void ACursedCityCharacter::BeginPlay()
{
	Super::BeginPlay();

	WeaponManagerComponent->OnCurrentWeaponChanged.AddDynamic(this, &ACursedCityCharacter::OnNewCurrentWeapon);
}

void ACursedCityCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Interaction", IE_Released, this, &ACursedCityCharacter::PressInteraction);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACursedCityCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACursedCityCharacter::MoveRight);
	
	PlayerInputComponent->BindAction("UseWeapon", IE_Pressed, this, &ACursedCityCharacter::StartUseWeapon);
	PlayerInputComponent->BindAction("UseWeapon", IE_Released, this, &ACursedCityCharacter::StopUseWeapon);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACursedCityCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACursedCityCharacter::LookUpAtRate);
}

void ACursedCityCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACursedCityCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACursedCityCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACursedCityCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ACursedCityCharacter::StartUseWeapon()
{
	ABaseWeaponObject* CurrentWeapon = WeaponManagerComponent->GetCurrentWeapon();
	if(CurrentWeapon)
	{
		CurrentWeapon->UseWeapon();
	}
}

void ACursedCityCharacter::StopUseWeapon()
{
	ABaseWeaponObject* CurrentWeapon = WeaponManagerComponent->GetCurrentWeapon();
	if(CurrentWeapon)
	{
		CurrentWeapon->StopUseWeapon();
	}
}

FHitResult ACursedCityCharacter::GetHitResultForInteraction()
{
	/** @todo rewrite the logic for finding an object to interact with it */

	FVector const TraceStart = FollowCamera->GetComponentLocation();
	FVector const TraceEnd = FollowCamera->GetForwardVector() * 1200.f + TraceStart;

	FHitResult OutHit;

	GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECC_Visibility);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 1.f);
	return OutHit;
}

void ACursedCityCharacter::PressInteraction()
{
	FHitResult const OutHit = GetHitResultForInteraction();
	if(!OutHit.GetActor()) return;
	if(!OutHit.GetActor()->GetClass()->ImplementsInterface(UInteractionWithObjectInterface::StaticClass())) return;

	IInteractionWithObjectInterface::Execute_InteractionWithObject(OutHit.GetActor(), this);
}	

void ACursedCityCharacter::OnNewCurrentWeapon(ABaseWeaponObject* Weapon)
{
	if(Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,"SKT_RightWeaponPosition");
	}
}
