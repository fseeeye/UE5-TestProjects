// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorSpawnAndSimGameMode.h"
#include "EditorSpawnAndSimHUD.h"
#include "EditorSpawnAndSimCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEditorSpawnAndSimGameMode::AEditorSpawnAndSimGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AEditorSpawnAndSimHUD::StaticClass();
}
