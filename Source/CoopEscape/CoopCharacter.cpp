// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoopHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

// Sets default values
ACoopCharacter::ACoopCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	SetReplicateMovement(true);
	bAlwaysRelevant = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(RootComponent);
    CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
    CameraComponent->bUsePawnControlRotation = true;

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthComponent = CreateDefaultSubobject<UCoopHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ACoopCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if(APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if(DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

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

	if(UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACoopCharacter::Move);
		}
		if(LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACoopCharacter::Look);
		}
		if(JumpAction)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACoopCharacter::StartJump);
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACoopCharacter::StopJump);
		}
	}
}

void ACoopCharacter::Move(const FInputActionValue &Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();
	if(Controller && MoveVector.SizeSquared() > 0.f)
	{
		AddMovementInput(GetActorForwardVector(), MoveVector.X);
		AddMovementInput(GetActorRightVector(), MoveVector.Y);
	}
}

void ACoopCharacter::Look(const FInputActionValue &Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	if(Controller)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
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

void ACoopCharacter::HandleDamage(float DamageAmount)
{
    // Красный экран + звук урона (через BP)
    BP_ShowDamageEffect();
}

void ACoopCharacter::HandleHeal(float HealAmount)
{
    // Зелёный экран + звук лечения (через BP)
    BP_ShowHealEffect();
}

void ACoopCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ACoopCharacter, CollectedItems);
}