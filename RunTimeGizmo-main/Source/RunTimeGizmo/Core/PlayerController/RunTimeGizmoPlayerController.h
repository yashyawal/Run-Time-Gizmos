// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "RunTimeGizmoPlayerController.generated.h"

struct FInputActionValue;
class URunTimeGizmoGameInstance;
class ARunTimeGizmoManager;
/**
 * Process Mouse events for Gizmo control
 */
UCLASS()
class RUNTIMEGIZMO_API ARunTimeGizmoPlayerController : public APlayerController
{
	GENERATED_BODY()
    
public:
	ARunTimeGizmoPlayerController();

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* GizmoInputMappingConetxt;
 
	UPROPERTY(EditDefaultsOnly)
	UInputAction* MouseClickInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* DeselectInputAction;

	UFUNCTION()
	void OnLeftMouseButtonClicked(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void OnMouseClickEnd(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void DisableGizmo();

	UFUNCTION()
	void ProcessClickedActor();
   
	UPROPERTY()
	ARunTimeGizmoManager* RuntimeGizmoManager;



	UPROPERTY()
	bool bIsMouseClickActive;

	UPROPERTY()
	float DistanceFromTraceStart;

	UPROPERTY()
	FVector DragLocation;

	UPROPERTY()
	UStaticMeshComponent* CurrentAxisComponent;

};

