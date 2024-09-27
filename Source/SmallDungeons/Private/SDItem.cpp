// Fill out your copyright notice in the Description page of Project Settings.


#include "SDItem.h"
#include "AGR_Inventory_Runtime/Public/Inventory/Components/AGR_ItemComponent.h"
#include "AGR_Inventory_Runtime/Public/Inventory/Libs/AGR_InventoryFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Interfaces/Interactor.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogSDItem, Log, All)

// Sets default values
ASDItem::ASDItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(MeshComponent);

	InteractionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollider"));
	InteractionCollider->SetupAttachment(GetRootComponent());
	InteractionCollider->SetSphereRadius(180.0f);
	InteractionCollider->SetGenerateOverlapEvents(true);
	
	ItemComponent = CreateDefaultSubobject<UAGR_ItemComponent>(TEXT("ItemComp"));
	ItemComponent->bIsStackable = true;
	ItemComponent->MaxStackCount = 99;
}

void ASDItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ItemComponent->OnItemPickedUp.AddDynamic(this, &ASDItem::OnItemPickedUpNative);
	ItemComponent->OnItemDropped.AddDynamic(this, &ASDItem::OnItemDroppedNative);
}

void ASDItem::OnItemPickedUpNative(const UAGR_InventoryComponent* InventoryComponent)
{
	InteractionCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASDItem::OnItemDroppedNative(const UAGR_InventoryComponent* InventoryComponent, const FVector& DropLocation)
{
	InteractionCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASDItem::Interact_Implementation(AActor* OtherInstigator)
{
	IInteractor::Execute_RemoveInteractable(OtherInstigator, this);
	if (const UAGR_InventoryComponent* InventoryComp = UAGR_InventoryFunctionLibrary::GetInventoryComponent(OtherInstigator))
	{
		bool success;
		FString errorMsg;
		ItemComponent->PickUpItem(success, errorMsg, InventoryComp);

		if (!success)
		{
			UE_LOG(LogSDItem, Log, TEXT("Unable to pickup item: %s"), *errorMsg);
		}
	}
}

void ASDItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASDItem, ItemComponent);
}
