// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunTimeGizmoManager.generated.h"
class ARuntimeGizmoActor;
class UMaterialInterface;
class APawn;

struct FPreviousStateOfSelectedMesh
{
public:
	FPreviousStateOfSelectedMesh();
	FPreviousStateOfSelectedMesh(bool bIsPhysicsEnabled, uint8 CurrentCollisionProfile , uint8 CurrentMobilityState);
	bool GetWasPhysicsEnabled() const;
	uint8 GetPreviousCollisionProfile() const;
	uint8 GetPreviousMobilityState() const;

private:
	bool bWasPhysicsEnabled;
	uint8 PreviousCollisionProfile;
	uint8 PreviousMobilityState;
};

UCLASS()
class RUNTIMEGIZMO_API ARunTimeGizmoManager : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	ARunTimeGizmoManager();

	UFUNCTION()
	UMeshComponent* GetSelectedMesh() const;

	UFUNCTION()
	AActor* GetSelectedActor() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void SetGizmoVisibility(bool bShouldbeVisible);
	UFUNCTION()
	void SelectforTransform(UMeshComponent* const Mesh);
	UFUNCTION()
	ARuntimeGizmoActor* GetRunTimeGizmoActor();
    
	//Changes the root components location of the selected actor and the gizmo upon updating the mouse position 
	UFUNCTION()
	void UpdateSelectedActorLocation(const FVector& RelativeLocationOffset);

	UFUNCTION()
	void ClearGizmo();



private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARuntimeGizmoActor> RuntimeGizmoAsset;

	UPROPERTY()
	ARuntimeGizmoActor* RuntimeGizmo;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* OverlayMaterial;

	UPROPERTY()
	AActor* SelectedActor;

	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY()
	UMeshComponent* SelectedMeshComponent;

	UPROPERTY()
	UMaterialInterface* PreviousOverlayMaterial;
	
	FPreviousStateOfSelectedMesh PreviousMeshState;
	
	UPROPERTY()
	FVector InitialGizmoScale;

	UFUNCTION()
	void RestorePreviousMeshState();

	UFUNCTION()
	void RestoreCurrentMeshState();

	UFUNCTION()
	void UpdateGizmoScale();

	UFUNCTION()
	void SaveCurrentMeshState(UMeshComponent* const Mesh);

	UFUNCTION()
	void ChangeCurrentMeshState(UMeshComponent* const Mesh);
};
