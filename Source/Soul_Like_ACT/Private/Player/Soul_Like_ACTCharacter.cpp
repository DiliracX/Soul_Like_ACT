// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Player/Soul_Like_ACTCharacter.h"
#include "Player/AnimManager.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/InputComponent.h"
#include "Player/LockTargetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ASoul_Like_ACTCharacter

ASoul_Like_ACTCharacter::ASoul_Like_ACTCharacter()
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
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1000.f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
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

	// Arrow Component for Target Lock Component
	TargetLockArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("TargetLockArrow"));
	TargetLockArrow->SetupAttachment(RootComponent);
	TargetLockArrow->bAbsoluteRotation = 1;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	AnimManager = CreateDefaultSubobject<UAnimManager>(TEXT("AnimManager"));

	TargetLockingComponent = CreateDefaultSubobject<ULockTargetComponent>(TEXT("TargetLockingComponent"));

	Faction = EActorFaction::Player;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASoul_Like_ACTCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &ASoul_Like_ACTCharacter::UseLMB);
	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &ASoul_Like_ACTCharacter::UseRMB);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASoul_Like_ACTCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASoul_Like_ACTCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASoul_Like_ACTCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASoul_Like_ACTCharacter::LookUpAtRate);
}


void ASoul_Like_ACTCharacter::ResetRotation()
{
	SetActorRotation(FRotator{ GetActorRotation().Pitch, GetInstigator()->GetViewRotation().Yaw,GetActorRotation().Roll });
}

void ASoul_Like_ACTCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASoul_Like_ACTCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASoul_Like_ACTCharacter::UseLMB()
{
	if (Weapon)
	{
		FString DebugMessage;
		AnimManager->TryUseDequeMotion(EActionType::Attack, 0, DebugMessage);
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, DebugMessage);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "No Weapon");
	}
}

void ASoul_Like_ACTCharacter::UseRMB()
{
	FString DebugMessage;
	AnimManager->TryUseDequeMotion(EActionType::Dodge, 0, DebugMessage);
}

void ASoul_Like_ACTCharacter::BeginPlay()
{
	Super::BeginPlay();

	TargetLockingComponent->InitComponent(TargetLockArrow);
}

void ASoul_Like_ACTCharacter::MoveForward(float Value)
{
	//Axis Value for AnimManager
	ForwardAxisValue = Value;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		if (TargetLockingComponent->GetIsTargetingEnabled())
		{
			Value *= 0.625f;
		}

		AddMovementInput(Direction, Value);
	}
}

void ASoul_Like_ACTCharacter::MoveRight(float Value)
{
	//Get axis value for AnimManager
	RightAxisValue = Value;

	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction

		if (TargetLockingComponent->GetIsTargetingEnabled())
		{
			Value *= 0.625f;
		}

		AddMovementInput(Direction, Value);
	}
}
