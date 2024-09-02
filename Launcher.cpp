// Fill out your copyright notice in the Description page of Project Settings.

// BP_Shotgun

#include "Launcher.h"
#include "Kismet/GameplayStatics.h"

//언리얼 5.1이상은 include for FPointDamageEvent Struct.
#include "Engine/DamageEvents.h" 


void ALauncher::PullTrigger()
{
	
	UE_LOG(LogTemp, Warning, TEXT("SHOTGUN , TRIGGERED"));
	
	//Ammo
	if(Ammo < 1)
	{
		UGameplayStatics::SpawnSoundAttached(MuzzleEmptySound, Mesh, TEXT("MuzzleFlashSocket"));
		// Play empty sound
	}


	if(Ammo > 0){
	
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));


	//
	Ammo -= 5;
	UE_LOG(LogTemp, Error, TEXT("Left Ammo %f"), Ammo);	
	
	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);

	if(bSuccess)
	{	
		
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect , Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		
		AActor* HitActor = Hit.GetActor();
		
		if(HitActor != nullptr)
		{	
			AController *OwnerController = GetOwnerController();
			//struct
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);

			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}			//shootercharacter.cpp에 있는 함수
	}
	
	}
}
