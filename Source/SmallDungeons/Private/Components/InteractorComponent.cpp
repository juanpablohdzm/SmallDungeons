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
	
	IInteractable::Execute_Interact(InteractablesQueue[0], this);
}

void UInteractorComponent::AddInteractable(UObject* Object)
{
	const IInteractable* Interactable = Cast<IInteractable>(Object);
	if (Interactable || InteractablesQueue.Contains(Object))
	{
		return;
	}

	InteractablesQueue.Add(Object);
	OnInteractableChanged.Broadcast();
}

void UInteractorComponent::RemoveInteractable(UObject* Object)
{
	if (!Object)
	{
		return;
	}
	InteractablesQueue.Remove(Object);
	OnInteractableChanged.Broadcast();
}

UObject* UInteractorComponent::GetInteractable()
{
	return InteractablesQueue.Num() > 0 ? InteractablesQueue[0] : nullptr;
}

