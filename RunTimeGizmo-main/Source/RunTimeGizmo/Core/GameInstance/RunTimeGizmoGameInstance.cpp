#include "RunTimeGizmoGameInstance.h"
#include<RunTimeGizmo/RunTimeGizmo/RunTimeGizmoManager.h>

void URunTimeGizmoGameInstance::Init()
{
	Super::Init();
}
void URunTimeGizmoGameInstance::OnStart()
{
	//Ensures that the objects are being created after the map is getting loaded 

	Super::OnStart();

	if (!RuntimeGizmoManager)
	{
		RuntimeGizmoManager = GetWorld()->SpawnActor<ARunTimeGizmoManager>(RuntimeGizmoManagerAsset);
	}
}
void URunTimeGizmoGameInstance::Shutdown()
{
	Super::Shutdown();
	RuntimeGizmoManager = nullptr;
}
ARunTimeGizmoManager* URunTimeGizmoGameInstance::GetRuntimeGizmoManager()
{
	if (!RuntimeGizmoManager)
	{
		RuntimeGizmoManager = GetWorld()->SpawnActor<ARunTimeGizmoManager>(RuntimeGizmoManagerAsset);
	}
	return RuntimeGizmoManager;
}

