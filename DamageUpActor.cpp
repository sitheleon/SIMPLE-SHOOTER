// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageUpActor.h"
#include "Gun.h"

// Sets default values
ADamageUpActor::ADamageUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADamageUpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADamageUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageUpActor::TriggerDamageUp(AGun* Gun)
{
	if(Gun)
	{
		Gun->DamageUp();
		UE_LOG(LogTemp, Warning, TEXT("DamageUp function called from DamageUpActor"));
	}
}

