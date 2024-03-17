// Fill out your copyright notice in the Description page of Project Settings.


#include "RunTimeGizmoManager.h"
#include <Materials/MaterialInterface.h>
#include <RuntimeGizmo/RunTimeGizmo/RuntimeGizmoActor.h>
#include <GameFramework/PawnMovementComponent.h>
#include <GameFrameWork/Pawn.h>

// Sets default values
ARunTimeGizmoManager::ARunTimeGizmoManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FPreviousStateOfSelectedMesh::FPreviousStateOfSelectedMesh()
{
}

FPreviousStateOfSelectedMesh::FPreviousStateOfSelectedMesh(bool bIsPhysicsEnabled, uint8 CurrentCollisionProfile , uint8 CurrentMobilityState):bWasPhysicsEnabled(bIsPhysicsEnabled),PreviousCollisionProfile(CurrentCollisionProfile), PreviousMobilityState(CurrentMobilityState)
{
}

bool FPreviousStateOfSelectedMesh::GetWasPhysicsEnabled() const
{
	return bWasPhysicsEnabled;
}

uint8 FPreviousStateOfSelectedMesh::GetPreviousCollisionProfile() const
{
	return PreviousCollisionProfile;
}

uint8 FPreviousStateOfSelectedMesh::GetPreviousMobilityState() const
{
	return PreviousMobilityState;
}

// Called when the game starts or when spawned
void ARunTimeGizmoManager::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		//Bind events with the gizmo 
		RuntimeGizmo = GetWorld()->SpawnActor<ARuntimeGizmoActor>(RuntimeGizmoAsset);
		RuntimeGizmo->SetActorHiddenInGame(true);
		//Set Initial scale of the gizmo
		InitialGizmoScale = RuntimeGizmo->GetActorScale3D();
		if (const auto Pawn = GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			PlayerPawn = Pawn;
			UpdateGizmoScale();
		}
	}
}

// Called every frame
void ARunTimeGizmoManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!PlayerPawn->GetVelocity().IsNearlyZero())
	{
		UpdateGizmoScale();
	}
}

void ARunTimeGizmoManager::SetGizmoVisibility(bool bShouldbeVisible)
{
	RuntimeGizmo->SetActorHiddenInGame(!bShouldbeVisible);
	
}


void ARunTimeGizmoManager::RestorePreviousMeshState()
{
	SelectedMeshComponent->SetOverlayMaterial(nullptr);
	SelectedMeshComponent->SetSimulatePhysics(PreviousMeshState.GetWasPhysicsEnabled());
	SelectedMeshComponent->SetMobility(EComponentMobility::Type(PreviousMeshState.GetPreviousMobilityState()));
	SelectedMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type(PreviousMeshState.GetPreviousCollisionProfile()));
}

void ARunTimeGizmoManager::SaveCurrentMeshState(UMeshComponent* const Mesh)
{
	PreviousMeshState = FPreviousStateOfSelectedMesh(Mesh->IsSimulatingPhysics(), Mesh->GetCollisionEnabled(),Mesh->Mobility);
}

void ARunTimeGizmoManager::ChangeCurrentMeshState(UMeshComponent* const Mesh)
{
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetMobility(EComponentMobility::Movable);
	SelectedMeshComponent = Mesh;
	SelectedActor = Mesh->GetOwner();
	SetGizmoVisibility(true);
	RuntimeGizmo->SetActorLocation(Mesh->GetOwner()->GetActorLocation());
	SelectedMeshComponent->SetOverlayMaterial(OverlayMaterial);
	UpdateGizmoScale();
}

void ARunTimeGizmoManager::SelectforTransform(UMeshComponent* const Mesh)
{
	if (SelectedMeshComponent != Mesh)
	{
		// Initially Selected mesh is None so no need of state restoration
		if (!SelectedMeshComponent)
		{
			SaveCurrentMeshState(Mesh);
			ChangeCurrentMeshState(Mesh);
		}
		else // the selected mesh was previously present and we have selected a new one
		{
			RestorePreviousMeshState();
			SaveCurrentMeshState(Mesh);
			ChangeCurrentMeshState(Mesh);
		}
	}
}

ARuntimeGizmoActor* ARunTimeGizmoManager::GetRunTimeGizmoActor()
{
	return RuntimeGizmo;
}

void ARunTimeGizmoManager::UpdateSelectedActorLocation(const FVector& RelativeLocationOffset)
{
	RuntimeGizmo->GetRootComponent()->AddRelativeLocation(RelativeLocationOffset);
	SelectedActor->GetRootComponent()->AddRelativeLocation(RelativeLocationOffset);
}

void ARunTimeGizmoManager::ClearGizmo()
{
	RestoreCurrentMeshState();
	SetGizmoVisibility(false);
}

UMeshComponent* ARunTimeGizmoManager::GetSelectedMesh() const
{
	return SelectedMeshComponent;
}

AActor* ARunTimeGizmoManager::GetSelectedActor() const
{
	return SelectedActor;
}
void ARunTimeGizmoManager::RestoreCurrentMeshState()
{
	RestorePreviousMeshState();
	SelectedMeshComponent = nullptr;
}

void ARunTimeGizmoManager::UpdateGizmoScale()
{
	if (PlayerPawn)
	{
		float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), RuntimeGizmo->GetActorLocation());
		RuntimeGizmo->SetActorScale3D(InitialGizmoScale * (Distance / 10.0f));
	}
}
