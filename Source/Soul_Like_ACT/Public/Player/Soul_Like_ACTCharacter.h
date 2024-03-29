// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulCharacterBase.h"
#include "Soul_Like_ACTCharacter.generated.h"

UCLASS(config=Game)
class ASoul_Like_ACTCharacter : public ASoulCharacterBase
{
protected:
    GENERATED_BODY()

    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Component)
    class UActionSysManager* ActionSysManager;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
    class UArrowComponent* TargetLockArrow;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
    class ULockTargetComponent* TargetLockingComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
    class UInventoryManager* InventoryManager;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = AI)
    class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource;

protected:
    virtual void BeginPlay() override;

    virtual void PossessedBy(AController* NewController) override;

public:
    static const float BattleMovementScale;
    static const float TravelMovementScale;

    ASoul_Like_ACTCharacter();

    virtual void Tick(float DeltaTime) override;

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    float BaseLookUpRate;

    float ForwardAxisValue, RightAxisValue;
    float LeanAmount_Char, LeanSpeed_Char;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float LeanAmount_Anim;

    bool bIsLeftMouseButtonPressed;

protected:
    //Tick------------------------------
    UFUNCTION(BlueprintCallable)
    void MoveForward(float Value);
    UFUNCTION(BlueprintCallable)
    void MoveRight(float Value);
    UFUNCTION(BlueprintCallable)
    void MakeMove();

    void TurnAtRate(float Rate);

    void LookUpAtRate(float Rate);

    void ZoomCamera(float Rate);

    UFUNCTION(BlueprintCallable)
    void CalculateLeanValue(float TurnValue);
    //----------------------------------

    void PredictMovement(FVector& DirectionVec, float& Degree);

    UFUNCTION(BlueprintImplementableEvent)
    void DegreeToMovementMultiplier(const float& Degree, float& Multiplier);

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /**
     * The function is called when some attack movements have root motion enabled and direction offsets.
     * It overrides the direction with controller's rotation or by calling lock-target component;
     */
    virtual void ForceOverrideFacingDirection(float Alpha) override;
    
public:
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns FollowCamera subobject **/
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    FORCEINLINE class UActionSysManager* GetActionSysManager() const { return ActionSysManager; }

    UFUNCTION(BlueprintCallable)
    void ResetRotation();

    UFUNCTION(BlueprintCallable)
    AWeaponActor* EquipGear(TSubclassOf<AWeaponActor> WeaponClassRef, bool bShowTracelines);

    UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outp", BlueprintInternalUseOnly))
    void GetMyPlayerController(class ASoulPlayerController*& MyController, EIsControllerValid& Outp);

    UFUNCTION(BlueprintCallable)
    class UInventoryManager* GetInventoryManager() const;

    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly))
    static void GetPlayer(UWorld* InWorld, bool& Successful, ASoulPlayerController*& SoulPlayerController,
                          ASoul_Like_ACTCharacter*& SoulCharacter, UInventoryManager*& SoulInventoryManager);

    virtual void GetMovementMode(ESoulMovementMode& MovementMode) const override;

    friend UActionSysManager;
};
