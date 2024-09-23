// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractorComponent.h"

#include "Interfaces/Interactable.h"


UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractorComponent::Interact()
{
	if (InteractablesQueue.IsEmpty())
	{
		return;
	}

	UObject* First;
	InteractablesQueue.Peek(First);
	IInteractable::Execute_Interact(Cast<UObject>(First), this);
}

void UInteractorComponent::SetInteractable_Implementation(UObject* Object)
{
	if (!Object->Implements<IInteractable>())
	Interactable = Object;
}

IInteractable* UInteractorComponent::GetInteractable_Implementation()
{
	return Interactable;
}

