// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/WeaponActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/SoulAbilitySysBPLib.h"
#include "SoulCharacterBase.h"
#include "DrawDebugHelpers.h"


// Sets default values
AWeaponActor::AWeaponActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComp");
    MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
    Super::BeginPlay();

    OwnerRef = Cast<ASoulCharacterBase>(GetInstigator());
    
    check(OwnerRef);
    check(GearInfo);
}

void AWeaponActor::CheckCollision()
{
    PrevVecs = CurrVecs;
    for (int i = GearInfo->BladeStartLength; i <= GearInfo->BladeTail; i += 20)
    {
        const int32 size_ = (i - GearInfo->BladeStartLength) / 10;

        CurrVecs[size_] = GetActorLocation() + i * GetActorUpVector();

        DrawTraceLine(PrevVecs[size_], CurrVecs[size_], bEnableDrawTraceLine);
    }
}

void AWeaponActor::DrawTraceLine(FVector prevVec_, FVector currVec_, bool bDrawTraceLine)
{
    TArray<FHitResult> Hits;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(OwnerRef);

    const bool bIsHit = GetWorld()->LineTraceMultiByChannel(Hits, prevVec_, currVec_, ECC_Pawn, QueryParams);
    if (bIsHit)
    {
        if (bDrawTraceLine)
            DrawDebugLine(GetWorld(), prevVec_, currVec_, FColor::Green, 0, 2.f, 0, 1.f);

        for (const auto& Hit : Hits)
        {
            ASoulCharacterBase* TargetPawn = Cast<ASoulCharacterBase>(Hit.GetActor());
            if (TargetPawn
                && ASoulCharacterBase::IsInRivalFaction(OwnerRef, TargetPawn)
                && TryExcludeActor(TargetPawn))
            {
                ApplyEventBackToGA(TargetPawn, Hit);
            }
        }
    }
    else if (bDrawTraceLine)
        DrawDebugLine(GetWorld(), prevVec_, currVec_, FColor::Red, 0, 2.f, 0, 1.f);
}

/*
* Exclude the actor which takes the impact
* Return true if the Actor has not been impacted
*/
bool AWeaponActor::TryExcludeActor(AActor* HitActor)
{
    if (MyTargets.Contains(HitActor))
    {
        return 0;
    }
    MyTargets.Add(HitActor);
    return 1;
}

void AWeaponActor::StartSwing(const float& InDmgMulti)
{
    bIsTracingCollision = 1;

    DmgMultiplier = InDmgMulti;

    CurrVecs.Reset();
    MyTargets.Empty();

    if (GearInfo->SwingSound)
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), GearInfo->SwingSound, GetActorLocation());

    for (int i = GearInfo->BladeStartLength; i <= GearInfo->BladeTail; i += 10)
    {
        CurrVecs.Add(GetActorLocation() + i * GetActorUpVector());
    }
}

void AWeaponActor::EndSwing()
{
    bIsTracingCollision = 0;
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsTracingCollision)
        CheckCollision();
}
