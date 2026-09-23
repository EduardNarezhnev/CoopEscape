// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPickupActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "CoopCharacter.h"
#include "CoopGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACoopPickupActor::ACoopPickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionProfileName("OverlapAllDynamic");

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionProfileName("OverlapAllDynamic");
	SphereComponent->SetSphereRadius(100.f);
}

// Called when the game starts or when spawned
void ACoopPickupActor::BeginPlay()
{
	Super::BeginPlay();
	
	if(HasAuthority())
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACoopPickupActor::OnOverlapBegin);
	}
}

void ACoopPickupActor::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
	ACoopCharacter* Character = Cast<ACoopCharacter>(OtherActor);
	if(!Character) return;

	ACoopGameMode* GM = GetWorld()->GetAuthGameMode<ACoopGameMode>();
	if(GM && GM->GetCurrentState() != EGameState::InProgress) return;

	ApplyEffect(Character);

	if(PickupSound)
	{
		UGameplayStatics::PlaySound2D(this, PickupSound);
	}
}

void ACoopPickupActor::ApplyEffect(ACoopCharacter* Character)
{

}