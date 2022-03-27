// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "MyDropActor.generated.h"

UCLASS()
class EDITORSPAWNANDSIM_API AMyDropActor : public ATriggerBox
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyDropActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
public:
	UFUNCTION(CallInEditor, Category = "Editor")
	void SpawnOneActor();

	UFUNCTION(CallInEditor, Category = "Editor")
	void ClearSpawnActors();

	UFUNCTION(CallInEditor, Category = "Editor")
	void StartSimulate();

	UFUNCTION(BlueprintCallable, Category = "Editor")
	static void SaveSimulate();

	UFUNCTION(BlueprintCallable, Category = "Editor")
	static void SpawnOneActorInSim();

private:
	void NativeSpawnOneActorInSim();
#endif //WITH_EDITOR

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Editor")
	TSubclassOf<AActor> SpawnTargetClass;

	UPROPERTY(VisibleAnywhere, Category = "Editor")
	TArray<AActor*> SpawnTargetActors;
#endif //WITH_EDITORONLY_DATA
};
