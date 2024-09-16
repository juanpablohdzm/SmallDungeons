// Fill out your copyright notice in the Description page of Project Settings.


#include "SDCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SDCharacterMovementComponent.h"

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
	SpringArmComponent->bUsePawnControlRotation = true;

	// Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	FollowCamera->SetupAttachment(SpringArmComponent);

	// Jump
	JumpGravityModifier = 1.0f;
	DefaultGravityScale = 1.0f;
}

// Called when the game starts or when spawned
void ASDCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASDCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(10, 0.1f, FColor::Magenta, FString::Format(TEXT("Current value of gravity: {0}"), {GetSDCharacterMovementComponent()->GravityScale}));
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
	if (GetSDCharacterMovementComponent()->IsFalling())
	{
		ModifyJumpGravity(JumpGravityModifier);
	}
}

void ASDCharacter::NotifyJumpApex()
{
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Blue, TEXT("Notify jump apex"));
	Super::NotifyJumpApex();
	ModifyJumpGravity(JumpGravityModifier);
}

void ASDCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	if (!bPressedJump || !GetSDCharacterMovementComponent()->IsFalling())
	{
		ModifyJumpGravity(DefaultGravityScale);
		GetSDCharacterMovementComponent()->bNotifyApex = true;
	}
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
}

void ASDCharacter::ModifyJumpGravity(float Value) const
{
	GetSDCharacterMovementComponent()->GravityScale = Value;
}

void ASDCharacter::Move(const FVector2D& Value)
{
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
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

