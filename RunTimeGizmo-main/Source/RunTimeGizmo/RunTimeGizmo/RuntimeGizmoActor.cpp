// Fill out your copyright notice in the Description page of Project Settings.


#include "RuntimeGizmoActor.h"

// Sets default values
ARuntimeGizmoActor::ARuntimeGizmoActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (!Root)
	{
		Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
		RootComponent = Root;
	}
	if (!TranslationXAxis)
	{
		TranslationXAxis = CreateDefaultSubobject<UStaticMeshComponent>("TranslationXAxis");
		TranslationXAxis->SetupAttachment(RootComponent);

	}
	if (!TranslationYAxis)
	{
		TranslationYAxis = CreateDefaultSubobject<UStaticMeshComponent>("TranslationYAxis");
		TranslationYAxis->SetupAttachment(RootComponent);
	}
	if (!TranslationZAxis)
	{
		TranslationZAxis = CreateDefaultSubobject<UStaticMeshComponent>("TranslationZAxis");
		TranslationZAxis->SetupAttachment(RootComponent);

	}
	if (!TranslationRoot)
	{
		TranslationRoot = CreateDefaultSubobject<UStaticMeshComponent>("TranslationRoot");
		TranslationRoot->SetupAttachment(RootComponent);

	}
}

// Called when the game starts or when spawned
void ARuntimeGizmoActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARuntimeGizmoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent* ARuntimeGizmoActor::GetTranslationXAxis() const
{
	return TranslationXAxis;
}

UStaticMeshComponent* ARuntimeGizmoActor::GetTranslationYAxis() const
{
	return TranslationYAxis;
}

UStaticMeshComponent* ARuntimeGizmoActor::GetTranslationZAxis() const
{
	return TranslationZAxis;
}

