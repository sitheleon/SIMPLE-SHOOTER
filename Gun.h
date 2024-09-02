// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	//AMMO EMPTY SOUND
	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleEmptySound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	bool GunTrace(FHitResult &Hit, FVector ShotDirection);

	AController* GetOwnerController() const;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	void DamageUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float MaxAmmo = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ammo")
	float Ammo;
	
	/// AMMO
	float GetAmmoPercent() const;

	
	// Ammo Recharge
	void RechargeAmmo(float Amount);


	UFUNCTION()
	float GetMaxAmmo(){return MaxAmmo;}

	UFUNCTION()
	float GetAmmo(){return Ammo;}

	
	virtual void PullTrigger();


};
