// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Launcher.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ALauncher : public AGun
{
	GENERATED_BODY()

public:
	virtual void PullTrigger() override;

	
};
