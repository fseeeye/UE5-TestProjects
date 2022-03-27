// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDropActor.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Editor/EditorEngine.h"
#include "Editor/UnrealEdEngine.h"
#include "Components/BoxComponent.h"
#include "UnrealEdGlobals.h"
#include "LevelEditor.h"
#include "Kismet/GameplayStatics.h"
#endif //WITH_EDITOR

// Sets default values
AMyDropActor::AMyDropActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyDropActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyDropActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#if WITH_EDITOR

void AMyDropActor::SpawnOneActor()
{
	if (!SpawnTargetClass)
	{
		return;
	}

	UWorld* editorWorld = GEditor->GetEditorWorldContext().World();
	
	// Generate random nearby position
	FVector originLocation = GetActorLocation();
	UBoxComponent* BoxCollisionComponent = CastChecked<UBoxComponent>(GetCollisionComponent());
	FVector boxExtent = BoxCollisionComponent->GetScaledBoxExtent();
	FBox box = FBox::BuildAABB(originLocation, boxExtent);
	FVector pos = FMath::RandPointInBox(box);
	pos.Z = box.Max.Z;

	AActor* newActor = editorWorld->SpawnActor(SpawnTargetClass, &pos);
	SpawnTargetActors.Add(newActor);
}

void AMyDropActor::ClearSpawnActors()
{
	for (AActor* actor : SpawnTargetActors)
	{
		actor->Destroy();
	}

	SpawnTargetActors.Empty();
}

// ref: UnrealEd\Private\Kismet2\DebuggerCommands.cpp - Simulate_Clicked()
void AMyDropActor::StartSimulate()
{
	// Is a simulation session already running?  If so, do nothing
	if (GEditor->PlayWorld != NULL && GUnrealEd->bIsSimulatingInEditor)
	{
		return;
	}

	FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));

	TSharedPtr<IAssetViewport> ActiveLevelViewport = LevelEditorModule.GetFirstActiveViewport();
	if (ActiveLevelViewport.IsValid())
	{
		// Start a new simulation session!
		if (GEditor->PlayWorld == NULL)
		{
			// SetLastExecutedPlayMode
			ULevelEditorPlaySettings* PlaySettings = GetMutableDefault<ULevelEditorPlaySettings>();
			PlaySettings->LastExecutedPlayModeType = PlayMode_Simulate;

			FPropertyChangedEvent PropChangeEvent(ULevelEditorPlaySettings::StaticClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(ULevelEditorPlaySettings, LastExecutedPlayModeType)));
			PlaySettings->PostEditChangeProperty(PropChangeEvent);

			PlaySettings->SaveConfig();


			FRequestPlaySessionParams SessionParams;
			SessionParams.WorldType = EPlaySessionWorldType::SimulateInEditor;
			SessionParams.DestinationSlateViewport = ActiveLevelViewport;

			GUnrealEd->RequestPlaySession(SessionParams);
		}
	}
}

// ref: Editor\LevelEditor\Private\LevelEditorActions.cpp - OnKeepSimulationChanges()
void AMyDropActor::SaveSimulate()
{
	UWorld* pieWorld = GEditor->GetPIEWorldContext() ? GEditor->GetPIEWorldContext()->World() : nullptr;
	UWorld* editorWorld = GEditor->GetEditorWorldContext().World();

	TArray<AActor*> dropActorsInEditor;
	UGameplayStatics::GetAllActorsOfClass(editorWorld, AMyDropActor::StaticClass(), dropActorsInEditor);

	for (AActor* actor : dropActorsInEditor)
	{
		AMyDropActor* dropActorInEditor = Cast<AMyDropActor>(actor);

		TArray<AActor*> actorsSpawnedInPIE;
		UGameplayStatics::GetAllActorsOfClass(pieWorld, dropActorInEditor->SpawnTargetClass, actorsSpawnedInPIE);

		for (AActor* actorSpawnedInPIE : actorsSpawnedInPIE)
		{
			// Find our counterpart actor
			AActor* counterpartActorInEditorWorld = EditorUtilities::GetEditorWorldCounterpartActor(actorSpawnedInPIE);

			if (counterpartActorInEditorWorld != nullptr)
			{
				const auto CopyOptions = (EditorUtilities::ECopyOptions::Type)(
					EditorUtilities::ECopyOptions::CallPostEditChangeProperty |
					EditorUtilities::ECopyOptions::CallPostEditMove |
					EditorUtilities::ECopyOptions::OnlyCopyEditOrInterpProperties |
					EditorUtilities::ECopyOptions::FilterBlueprintReadOnly);
				const int32 CopiedPropertyCount = EditorUtilities::CopyActorProperties(actorSpawnedInPIE, counterpartActorInEditorWorld, CopyOptions);
			}
			else
			{
				FVector pos = actorSpawnedInPIE->GetActorLocation();
				AActor* childActor = editorWorld->SpawnActor(dropActorInEditor->SpawnTargetClass, &pos);
				dropActorInEditor->SpawnTargetActors.Add(childActor);
			}
		}
	}
}

void AMyDropActor::SpawnOneActorInSim()
{
	TArray<AActor*> outDropActors;
	UGameplayStatics::GetAllActorsOfClass(GEditor->EditorWorld, AMyDropActor::StaticClass(), outDropActors);

	for (AActor* dropActor : outDropActors)
	{
		AMyDropActor* myDropActor = Cast<AMyDropActor>(dropActor);
		myDropActor->NativeSpawnOneActorInSim();
	}
}

void AMyDropActor::NativeSpawnOneActorInSim()
{
	if (!SpawnTargetClass)
	{
		return;
	}

	UWorld* pieWorld = GEditor->GetPIEWorldContext()->World();

	// Generate random nearby position
	FVector originLocation = GetActorLocation();
	UBoxComponent* BoxCollisionComponent = CastChecked<UBoxComponent>(GetCollisionComponent());
	FVector boxExtent = BoxCollisionComponent->GetScaledBoxExtent();
	FBox box = FBox::BuildAABB(originLocation, boxExtent);
	FVector pos = FMath::RandPointInBox(box);
	pos.Z = box.Max.Z;

	AActor* newActor = pieWorld->SpawnActor(SpawnTargetClass, &pos);
	SpawnTargetActors.Add(newActor);
}

#endif //WITH_EDITOR
