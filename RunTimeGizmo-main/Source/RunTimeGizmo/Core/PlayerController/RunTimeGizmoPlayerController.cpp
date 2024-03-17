// Fill out your copyright notice in the Description page of Project Settings.


#include "RunTimeGizmoPlayerController.h"
#include <Engine/GameViewportClient.h>
#include <Engine/GameEngine.h>
#include <Components/StaticMeshComponent.h>
#include <Components/MeshComponent.h>
#include<Components/PrimitiveComponent.h>
#include <RuntimeGizmo/Core/GameInstance/RunTimeGizmoGameInstance.h>
#include <RunTimeGizmo/RunTimeGizmo/RunTimeGizmoManager.h>
#include <RuntimeGizmo/RunTimeGizmo/RuntimeGizmoActor.h>
#include <Kismet/KismetMathLibrary.h>

ARunTimeGizmoPlayerController::ARunTimeGizmoPlayerController()
{

}
void ARunTimeGizmoPlayerController::BeginPlay()
{
    Super::BeginPlay();
    bShowMouseCursor = true;
    if (const auto GameInstance = Cast<URunTimeGizmoGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (auto GizmoManager = GameInstance->GetRuntimeGizmoManager())
        {
            RuntimeGizmoManager = GizmoManager;
        }
    }
}

void ARunTimeGizmoPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (bIsMouseClickActive && CurrentAxisComponent)
    {
        FVector2D CurrentMousePosition;
        UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();

        if (!ViewportClient)
        {
            return; 
        }

        ViewportClient->GetMousePosition(CurrentMousePosition);

        // Get the world location and direction from the screen space mouse coordinates
        FVector WorldLocation, WorldDirection;
        if (DeprojectScreenPositionToWorld(CurrentMousePosition.X, CurrentMousePosition.Y, WorldLocation, WorldDirection))
        {
            //Getting rid of the excess offset 
            FVector ConvertedMousePositionFromWorldToComponentSpace = UKismetMathLibrary::InverseTransformLocation(FTransform(DragLocation), WorldLocation + WorldDirection * DistanceFromTraceStart);
            FVector MovementDirectionDelta = ConvertedMousePositionFromWorldToComponentSpace - RuntimeGizmoManager->GetRunTimeGizmoActor()->GetActorLocation();
            const FString ComponentTag = CurrentAxisComponent->ComponentTags[0].ToString();

            if (ComponentTag.Equals("TranslationZAxis"))
            {
                MovementDirectionDelta.X = MovementDirectionDelta.Y = 0.0f;
            }
            if (ComponentTag.Equals("TranslationXAxis"))
            {
                MovementDirectionDelta.Y = MovementDirectionDelta.Z = 0.0f;
            }
            if (ComponentTag.Equals("TranslationYAxis"))
            {
                MovementDirectionDelta.X = MovementDirectionDelta.Z = 0.0f;
            }

            if (auto GizmoManager = RuntimeGizmoManager)
            {
                GizmoManager->UpdateSelectedActorLocation(MovementDirectionDelta); 
            }
        }
    }
}

void ARunTimeGizmoPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    // Get the local player subsystem
    if (const auto LocalPlayer = GetLocalPlayer())
    {
        // Clear out existing mapping, and add our mapping
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(GizmoInputMappingConetxt, 0);

            if (UEnhancedInputComponent* EnhancedPlayerInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
            {
                EnhancedPlayerInputComponent->BindAction(MouseClickInputAction, ETriggerEvent::Started, this, &ARunTimeGizmoPlayerController::OnLeftMouseButtonClicked);
                EnhancedPlayerInputComponent->BindAction(MouseClickInputAction, ETriggerEvent::Completed, this, &ARunTimeGizmoPlayerController::OnMouseClickEnd);
                EnhancedPlayerInputComponent->BindAction(DeselectInputAction, ETriggerEvent::Started, this, &ARunTimeGizmoPlayerController::DisableGizmo);
            }
        }
    }
}

void ARunTimeGizmoPlayerController::OnLeftMouseButtonClicked(const FInputActionValue& InputActionValue)
{
    bIsMouseClickActive = true;
    ProcessClickedActor();
}

void ARunTimeGizmoPlayerController::OnMouseClickEnd(const FInputActionValue& InputActionValue)
{
    bIsMouseClickActive = false;
    CurrentAxisComponent = nullptr;
}

void ARunTimeGizmoPlayerController::DisableGizmo()
{
    RuntimeGizmoManager->ClearGizmo();
}

void ARunTimeGizmoPlayerController::ProcessClickedActor()
{
    UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();

    if (!ViewportClient)
    {
        return;
    }

    // Get the mouse position in screen space
    FVector2D MousePosition;
    ViewportClient->GetMousePosition(MousePosition);


    // Get the world location and direction from the screen space mouse coordinates
    FVector WorldLocation, WorldDirection;
    if (DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection))
    {
        FVector EndLocation = WorldLocation + (WorldDirection * 10000);

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams(FName(TEXT("Raycast")), true, this);
        ECollisionChannel TraceChannel = ECC_Visibility; // Replace with the appropriate collision channel.

        if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, EndLocation, TraceChannel, CollisionParams))
        {
            AActor* HitActor = HitResult.GetActor();
            UPrimitiveComponent* HitComponent = HitResult.GetComponent();
            // Check if the hit component is a StaticMeshComponent
            if (HitComponent && HitComponent->IsA<UStaticMeshComponent>() && !HitActor->IsA<ARuntimeGizmoActor>())
            {
                UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(HitComponent);
                // Now you have access to the StaticMeshComponent, and you can interact with it as needed.
                // For example, you can get the static mesh itself:
                UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
                if (RuntimeGizmoManager)
                {
                    RuntimeGizmoManager->SelectforTransform(StaticMeshComponent);
                }
            }
            else if (HitActor->IsA<ARuntimeGizmoActor>())
            {
                CurrentAxisComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
                DistanceFromTraceStart = HitResult.Distance;
                DragLocation = UKismetMathLibrary::InverseTransformLocation(FTransform(HitResult.GetComponent()->GetComponentLocation()), HitResult.Location);
            }
        }
    }
}