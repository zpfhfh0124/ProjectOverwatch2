// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectOverwatchPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "PlayerHUD.h"
#include "ProjectOverwatchCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "ProjectOverwatch.h"
#include "Widgets/Input/SVirtualJoystick.h"

AProjectOverwatchPlayerController::AProjectOverwatchPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AProjectOverwatchCameraManager::StaticClass();
}

void AProjectOverwatchPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// GT----------------------------------------------
	// HUD
	if (IsLocalPlayerController() && HUDWidgetClass && !HUDWidget)
	{
		HUDWidget = CreateWidget<UPlayerHUD>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToPlayerScreen(0); // Or AddToViewport()
		}
		else
		{
			UE_LOG(LogProjectOverwatch, Error, TEXT("PlayerController HUD 부착 실패!."));
		}
	}
	// ------------------------------------------------
	
	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogProjectOverwatch, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AProjectOverwatchPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
	
}

bool AProjectOverwatchPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}
