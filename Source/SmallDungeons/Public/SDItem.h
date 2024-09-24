﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SDCharacter.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "SDItem.generated.h"

class USphereComponent;
class UAGR_ItemComponent;

UCLASS()
class SMALLDUNGEONS_API ASDItem : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASDItem();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void Interact_Implementation(UObject* OtherInstigator) override;

	virtual void PostInitializeComponents() override;
	
protected:
	UFUNCTION()
	virtual void OnItemPickedUpNative(const UAGR_InventoryComponent* InventoryComponent);
	
	UFUNCTION()
	virtual void OnItemDroppedNative(const UAGR_InventoryComponent* InventoryComponent, const FVector& DropLocation);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	USphereComponent* InteractionCollider;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, meta = (AllowPrivateAccess = true))
	UAGR_ItemComponent* ItemComponent;
};
