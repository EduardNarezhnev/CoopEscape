// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopHazardActor.h"
#include "CoopHazardConfig.h"
#include "Components/BoxComponent.h"
#include "CoopCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACoopHazardActor::ACoopHazardActor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicateMovement(true);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionProfileName("BlockAllDynamic");

	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollisionComponent->SetupAttachment(RootComponent);
	BoxCollisionComponent->SetCollisionProfileName("OverlapAllDynamic");
	BoxCollisionComponent->SetBoxExtent(FVector(50.f, 50.f, 50.f));
}

void ACoopHazardActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitialLocation = GetActorLocation();

	if(HasAuthority())
	{
		BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACoopHazardActor::OnOverlapBegin);
	}
	if(InactiveMaterial)
	{
		MeshComponent->SetMaterial(0, InactiveMaterial);
	}
}

void ACoopHazardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!HasAuthority()) return;

	AActor* ClosestPlayer = FindClosestPlayer();
	if(!ClosestPlayer)
	{
		if(bIsActive)
		{
			bIsActive = false;
		}
		ReturnToBase(DeltaTime);
		return;
	}

	float Distance = FVector::Dist(GetActorLocation(), ClosestPlayer->GetActorLocation());
	UpdateHazardState(ClosestPlayer, Distance);
}

AActor* ACoopHazardActor::FindClosestPlayer() const
{
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoopCharacter::StaticClass(), Players);

	AActor* Closest = nullptr;
	float ClosestDist = FLT_MAX;

	for(AActor* Player : Players)
	{
		float Dist = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
		if(Dist < ClosestDist)
		{
			ClosestDist = Dist;
			Closest = Player;
		}
	}

	return Closest;
}

void ACoopHazardActor::UpdateHazardState(AActor* ClosestPlayer, float Distance)
{
	float ActivationDist = Config ? Config->ActivationDistance : 500.f;
	float DeactivationDist = Config ? Config->DeactivationDistance : 800.f;

	if(Distance < ActivationDist && !bIsActive)
	{
		bIsActive = true;
		UpdateVisualState();
	}
	else if(Distance > DeactivationDist && bIsActive)
	{
		bIsActive = false;
		UpdateVisualState();
	}

	if(bIsActive)
	{
		MoveTowardsPlayer(ClosestPlayer, GetWorld()->GetDeltaSeconds());
	}
	else
	{
		ReturnToBase(GetWorld()->GetDeltaSeconds());
	}
}

void ACoopHazardActor::MoveTowardsPlayer(AActor* Player, float DeltaTime)
{
	if(!Player) return;

	float Speed = Config ? Config->MoveSpeed : 150.f;
	FVector Direction = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	AddActorWorldOffset(Direction * Speed * DeltaTime, true);
}

void ACoopHazardActor::ReturnToBase(float DeltaTime)
{
	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), InitialLocation, DeltaTime, 2.f);
	SetActorLocation(NewLocation);
}

void ACoopHazardActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!HasAuthority()) return;
	if(!bIsActive || !bCanApplyDamage) return;

	ACoopCharacter* Player = Cast<ACoopCharacter>(OtherActor);
	if(!Player) return;

	ApplyDamageToPlayer(Player);
}

void ACoopHazardActor::ApplyDamageToPlayer(AActor* Player)
{
	if(!Config) return;

	UGameplayStatics::ApplyDamage(Player, Config->DamageAmount, nullptr, this, UDamageType::StaticClass());

	bCanApplyDamage = false;
	GetWorldTimerManager().SetTimer(DamageCooldownTimer, this, &ACoopHazardActor::ResetDamageCooldown, Config->DamageCooldown, false);
}

void ACoopHazardActor::ResetDamageCooldown()
{
    bCanApplyDamage = true;
}

void ACoopHazardActor::OnRep_IsActive()
{
    UpdateVisualState();
}

void ACoopHazardActor::UpdateVisualState()
{
	if(bIsActive && ActiveMaterial)
	{
		MeshComponent->SetMaterial(0, ActiveMaterial);
	}
	else if(!bIsActive && InactiveMaterial)
	{
		MeshComponent->SetMaterial(0, InactiveMaterial);
	}
}

void ACoopHazardActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ACoopHazardActor, bIsActive);
}