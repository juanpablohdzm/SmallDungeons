﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SDItem.h"
#include "AGR_Inventory_Runtime/Public/Inventory/Components/AGR_ItemComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/Interactor.h"
#include "Interfaces/InventoryHolder.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogSDItem, Log, All)

// Sets default values
ASDItem::ASDItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	InteractionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollider"));
	InteractionCollider->SetupAttachment(GetRootComponent());
	InteractionCollider->SetSphereRadius(180.0f);
	InteractionCollider->SetGenerateOverlapEvents(true);
	InteractionCollider->SetCollisionObjectType(ECC_EngineTraceChannel1);
	
	ItemComponent = CreateDefaultSubobject<UAGR_ItemComponent>(TEXT("ItemComp"));
	ItemComponent->bIsStackable = true;
	ItemComponent->MaxStackCount = 99;
}

void ASDItem::Interact_Implementation(UObject* OtherInstigator)
{
	if (OtherInstigator->IsA<IInteractor>())
	{
		IInteractor::Execute_RemoveInteractable(OtherInstigator, this);
	}

	if (OtherInstigator->IsA<IInventoryHolder>())
	{
		if (const UAGR_InventoryComponent* InventoryComp = IInventoryHolder::Execute_GetInventory(OtherInstigator))
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
}

void ASDItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASDItem, ItemComponent);
}
