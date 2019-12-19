// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{

    auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();

    if (!ensure(controlledPawn->FindComponentByClass<UPatrolRoute>()))
        return EBTNodeResult::Failed;

    auto patrolPoints = controlledPawn->FindComponentByClass<UPatrolRoute>()->GetPatrolPoints();

    if (patrolPoints.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No patrol points found"));
        return EBTNodeResult::Failed;
    }

    auto blackBoardComp = OwnerComp.GetBlackboardComponent();
    auto indexNum = blackBoardComp->GetValueAsInt(indexKey.SelectedKeyName);
    blackBoardComp->SetValueAsObject(waypointKey.SelectedKeyName, patrolPoints[indexNum]);

    auto nextIndex = (indexNum + 1) % patrolPoints.Num();
    blackBoardComp->SetValueAsInt(indexKey.SelectedKeyName, nextIndex);

    return EBTNodeResult::Succeeded;
}