// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoopHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACoopCharacter::ACoopCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	SetReplicateMovement(true);
	//SetNetUpdateFrequency(30.f);
	//SetMinNetUpdateFrequency(10.f);
	bAlwaysRelevant = true;
	//SetNetCullDistanceSquared(100000.f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(RootComponent);
    CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
    CameraComponent->bUsePawnControlRotation = true;

	//GetMesh()->SetVisibility(false);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthComponent = CreateDefaultSubobject<UCoopHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ACoopCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if(HealthComponent)
	{
        HealthComponent->OnDamageTaken.AddDynamic(this, &ACoopCharacter::HandleDamage);
        HealthComponent->OnHealed.AddDynamic(this, &ACoopCharacter::HandleHeal);
		HealthComponent->OnHealthDepleted.AddDynamic(this, &ACoopCharacter::HandleDeath);
	}
}

// Called to bind functionality to input
void ACoopCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACoopCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACoopCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &ACoopCharacter::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &ACoopCharacter::LookUp);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACoopCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACoopCharacter::StopJump);
}

void ACoopCharacter::MoveForward(float Value)
{
    AddMovementInput(GetActorForwardVector(), Value);
}

void ACoopCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ACoopCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void ACoopCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void ACoopCharacter::StartJump()
{
    Jump();
}

void ACoopCharacter::StopJump()
{
    StopJumping();
}

float ACoopCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(HealthComponent)
	{
		HealthComponent->TakeDamage(DamageAmount);
		return DamageAmount;
	}
	return 0.f;
}

void ACoopCharacter::HandleDeath()
{
	if(APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->DisableInput(PC);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACoopCharacter::HandleDamage()
{
    // Красный экран + звук урона (через BP)
    BP_ShowDamageEffect();
}

void ACoopCharacter::HandleHeal()
{
    // Зелёный экран + звук лечения (через BP)
    BP_ShowHealEffect();
}

void ACoopCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ACoopCharacter, CollectedItems);
}