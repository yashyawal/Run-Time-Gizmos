#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RunTimeGizmoGameInstance.generated.h"
class ARunTimeGizmoManager;
/**
 * Handles global states and properties 
 */
UCLASS()
class RUNTIMEGIZMO_API URunTimeGizmoGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	ARunTimeGizmoManager* GetRuntimeGizmoManager();

protected:
	void Init() override;

	void Shutdown() override;

	void OnStart() override;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARunTimeGizmoManager> RuntimeGizmoManagerAsset;

	UPROPERTY()
	ARunTimeGizmoManager* RuntimeGizmoManager;

};
