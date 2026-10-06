// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopExtractionTrigger.h"
#include "Components/StaticMeshComponent.h"
#include "CoopCharacter.h"
#include "CoopGameMode.h"
#include "CoopGameState.h"
#include "Kismet/GameplayStatics.h"

ACoopExtractionTrigger::ACoopExtractionTrigger()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionProfileName("NoCollision");
}

void ACoopExtractionTrigger::BeginPlay()
{
    Super::BeginPlay();

    if(HasAuthority())
    {
        OnActorBeginOverlap.AddDynamic(this, &ACoopExtractionTrigger::OnOverlapBegin);
    }
}

void ACoopExtractionTrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    if(!HasAuthority()) return;

    ACoopCharacter* Player = Cast<ACoopCharacter>(OtherActor);
    if(!Player) return;

    ACoopGameMode* GM = GetWorld()->GetAuthGameMode<ACoopGameMode>();
    if(!GM || GM->GetCurrentState() != EGameState::InProgress) return;

    ACoopGameState* GS = GetWorld()->GetGameState<ACoopGameState>();
    if(!GS) return;

    if(GS->CollectedItemsCount < GS->MaxKeys)
    {
        UE_LOG(LogTemp, Warning, TEXT("Extraction: Not all keys collected!"));
        return;
    }

    if(AreAllPlayersInZone())
    {
        GM->GameOver(true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Extraction: Not all players in zone!"));
    }
}

bool ACoopExtractionTrigger::AreAllPlayersInZone() const
{
    TArray<AActor*> AllPlayers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoopCharacter::StaticClass(), AllPlayers);

    if(AllPlayers.Num() == 0) return false;

    for(AActor* Player : AllPlayers)
    {
        if(!IsOverlappingActor(Player))
        {
            return false;
        }
    }

    return true;
}