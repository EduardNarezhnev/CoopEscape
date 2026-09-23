// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopHealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCoopHealthComponent::UCoopHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UCoopHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;	
}

void UCoopHealthComponent::OnRep_Health()
{
	// Called on clients when CurrentHealth changes
	// HUD update
}

void UCoopHealthComponent::TakeDamage(float Damage)
{
	if(!GetOwner()->HasAuthority()) return;
	if(Damage <= 0.f || CurrentHealth <= 0.f) return;

	CurrentHealth = FMath::Max(0.f, CurrentHealth - Damage);

	OnDamageTaken.Broadcast(Damage);

	if(CurrentHealth <= 0.f)
	{
		OnHealthDepleted.Broadcast();
	}
}

void UCoopHealthComponent::Heal(float Amount)
{
	if(!GetOwner()->HasAuthority()) return;
	if(Amount <= 0.f || CurrentHealth >= MaxHealth) return;

	CurrentHealth = FMath::Min(MaxHealth, CurrentHealth + Amount);

	OnHealed.Broadcast(Amount);
}

float UCoopHealthComponent::GetHealthPercent() const
{
	return MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f;
}

void UCoopHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCoopHealthComponent, CurrentHealth);
}