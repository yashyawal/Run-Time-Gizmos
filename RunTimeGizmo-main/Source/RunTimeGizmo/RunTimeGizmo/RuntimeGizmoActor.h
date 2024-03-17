// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeGizmoActor.generated.h"

UCLASS()
class RUNTIMEGIZMO_API ARuntimeGizmoActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARuntimeGizmoActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	UStaticMeshComponent* GetTranslationXAxis() const;

	UFUNCTION()
	UStaticMeshComponent* GetTranslationYAxis() const;

	UFUNCTION()
	UStaticMeshComponent* GetTranslationZAxis() const;



private:

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* TranslationXAxis;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* TranslationYAxis;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* TranslationZAxis;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* TranslationRoot;
};
