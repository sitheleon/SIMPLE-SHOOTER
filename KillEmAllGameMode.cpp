// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShotterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if(PlayerController != nullptr)
    {
      EndGame(false); // Lost
    }
    

       //Iterates AI numbers (Counts AI Numbers)
    for(AShotterAIController* Controller : TActorRange<AShotterAIController>(GetWorld()))
    {
        if(!Controller->IsDead())
        {
            return; // Game not over yet, AI Still ALive
        }
    }

    //플레이어 win
    EndGame(true);


}



void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{   
        //for loop for everycontroller on the World, saved on Controller variable
    for(AController* Controller: TActorRange<AController>(GetWorld()))
    {
                        //컨트롤러가 플레이어인지(TRUE=플레이어) == 같을 시 플레이어는 승리한 것으로 간주
        bool bIsWinner= Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}
