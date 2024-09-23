// Fill out your copyright notice in the Description page of Project Settings.


#include "SDCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SDCharacterMovementComponent.h"
#include "SDAttributeSet.h"
#include "Components/InteractorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASDCharacter::ASDCharacter(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<USDCharacterMovementComponent>(CharacterMovementComponentName))
{
	SDCharacterMovementComponent = Cast<USDCharacterMovementComponent>(GetCharacterMovement());
	SDCharacterMovementComponent->bNotifyApex = true;
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Orientation
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Spring arm component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->SetRelativeRotation(FRotator(-40.0f,0.0f,0.0f));
	SpringArmComponent->TargetArmLength = 900.0f;

	// Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	FollowCamera->SetupAttachment(SpringArmComponent);

	//Abilities
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<USDAttributeSet>(TEXT("AttributeSet"));

	//Interaction
	InteractorComponent = CreateDefaultSubobject<UInteractorComponent>(TEXT("InteractionComp"));
	
}

// Called to bind functionality to input
void ASDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASDCharacter::StopJumping()
{
	Super::StopJumping();
	GetSDCharacterMovementComponent()->StopJumping();
}

UAbilitySystemComponent* ASDCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASDCharacter::Move(const FVector2D& Value)
{
	if (Controller)
	{
		const FRotator Rotation = FollowCamera->GetComponentRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Value.Y);
		AddMovementInput(RightDirection, Value.X);
	}
}

void ASDCharacter::Look(const FVector2D& Value)
{
	if (Controller)
	{
		AddControllerPitchInput(Value.Y);
		AddControllerYawInput(Value.X);
	}
}

void ASDCharacter::AddInteractable_Implementation(UObject* Object)
{
	InteractorComponent->AddInteractable(Object);
}

void ASDCharacter::RemoveInteractable_Implementation(UObject* Object)
{
	InteractorComponent->RemoveInteractable(Object);
}

void ASDCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASDCharacter, AbilitySystemComponent);
	DOREPLIFETIME(ASDCharacter, AttributeSet);
}
