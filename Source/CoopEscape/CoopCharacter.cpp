#include "CoopCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoopHealthComponent.h"
#include "CoopPlayerController.h"
#include "CoopGameMode.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

ACoopCharacter::ACoopCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	SetReplicatingMovement(true);
	bAlwaysRelevant = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(RootComponent);
    CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
    CameraComponent->bUsePawnControlRotation = true;

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthComponent = CreateDefaultSubobject<UCoopHealthComponent>(TEXT("HealthComponent"));
}

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

void ACoopCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(APlayerController* PC = Cast<APlayerController>(NewController))
	{
		PC->EnableInput(PC);
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
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
	if(!HasAuthority()) return;
	
	if(APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->DisableInput(PC);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(ACoopGameMode* GM = GetWorld()->GetAuthGameMode<ACoopGameMode>())
	{
		GM->GameOver(false);
	}
}

void ACoopCharacter::HandleDamage(float DamageAmount)
{
	if(ACoopPlayerController* PC = Cast<ACoopPlayerController>(GetController()))
	{
		PC->Client_ShowScreenEffect(FLinearColor(1, 0, 0, 1), 0.3f);
	}
}

void ACoopCharacter::HandleHeal(float HealAmount)
{
    if(ACoopPlayerController* PC = Cast<ACoopPlayerController>(GetController()))
	{
		PC->Client_ShowScreenEffect(FLinearColor(0, 1, 0, 1), 0.3f);
	}
}

void ACoopCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACoopCharacter, CollectedItems);
	DOREPLIFETIME(ACoopCharacter, bIsDangerNear);
}