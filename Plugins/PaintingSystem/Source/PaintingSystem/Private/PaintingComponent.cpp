// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingComponent.h"
#include "PaintingSubsystem.h"

DEFINE_LOG_CATEGORY(LogPainting);

UPaintingComponent::UPaintingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	OnComponentActivated.AddDynamic(this, &UPaintingComponent::ActivatePainting);
	OnComponentDeactivated.AddDynamic(this, &UPaintingComponent::DeactivatePainting);
}

void UPaintingComponent::ActivatePainting(UActorComponent* Component, bool bReset)
{
	if(!PaintingSubsystem)
	{
		PaintingSubsystem = GetWorld()->GetSubsystem<UPaintingSubsystem>();
	}
	if(bReset || !PaintingBrushData.IsValid())
	{
		PaintingBrushData = GetPaintingBrushData(); // if was invalid then it will be loaded if available
	}
	if(!PaintingBrushData.IsValid())
	{
		Deactivate();
		return;
	}
	if(bUseOnlyOnceAfterActivation)
	{
		Paint();
		Deactivate();
		return;
	}
}

void UPaintingComponent::DeactivatePainting(UActorComponent* Component)
{
	// invalidate recent draw point to not be considered in future extrapolation
	MostRecentDrawPoint = FRenderTargetDrawPoint();
}

bool UPaintingComponent::ShouldActivate() const
{
	// disable activation in editor
	return Super::ShouldActivate() && (!GetWorld()->IsEditorWorld() || GetWorld()->IsPlayInEditor());
}

void UPaintingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Paint();
}

void UPaintingComponent::PostDrawApproximation(FRenderTargetDrawPoint JustDrawnPoint)
{
	if(!bUseOnlyOnceAfterActivation && bTryApproximateInBetweenPoints)
	{
		if(MostRecentDrawPoint.bIsValid())
		{
			const FVector2d Direction = (JustDrawnPoint.NormalizedPosition - MostRecentDrawPoint.NormalizedPosition);
			const float DistanceFromRecentPoint = Direction.Length();
			const float ApproximatedRadiusOfBothPoints = (JustDrawnPoint.NormalizedSize + MostRecentDrawPoint.NormalizedSize).Length();
			if(DistanceFromRecentPoint > ApproximatedRadiusOfBothPoints / 3 && DistanceFromRecentPoint <= MaxLocalTextureDistanceToTryApproximate)
			{
				const int ApproximationPointsToAdd = ApproximationComplexity * FMath::CeilToInt(DistanceFromRecentPoint / JustDrawnPoint.NormalizedSize.Length());
				for (int i = 0; i < ApproximationPointsToAdd; ++i)
				{
					FRenderTargetDrawPoint ApproximatedPoint = JustDrawnPoint;
					ApproximatedPoint.NormalizedPosition -= (Direction * i / ApproximationPointsToAdd);
					PaintingSubsystem->RegisterPointToDraw(ApproximatedPoint, RenderTargetToDrawOn);
				}
			}
		}
	}
}

FRenderTargetDrawPoint UPaintingComponent::CalculatePointToDraw()
{
	PaintingBrushData.Rotation = GetComponentRotation().Yaw;
	PaintingBrushData.Color = DrawColor;
	
	const float WorldRadius = 10000.f;
	const FVector2D TargetPosition = (FVector2D(GetComponentLocation()) + WorldRadius) / (WorldRadius * 2);
	
	FRenderTargetDrawPoint DrawPoint;
	DrawPoint.NormalizedPosition = TargetPosition;
	DrawPoint.NormalizedSize = FVector2D(PaintRadius / WorldRadius);
	DrawPoint.Brush = PaintingBrushData;
	return DrawPoint;
}

void UPaintingComponent::Paint()
{
	const FRenderTargetDrawPoint DrawPoint = CalculatePointToDraw();
	PaintingSubsystem->RegisterPointToDraw(DrawPoint, RenderTargetToDrawOn);
	PostDrawApproximation(DrawPoint);
	MostRecentDrawPoint = DrawPoint;	
}

FPaintingBrush UPaintingComponent::GetPaintingBrushData()
{
	if(!PaintingBrushData.IsValid())
	{
		if(!PaintingBrushAsset.IsNull())
		{
				PaintingBrushData = PaintingBrushAsset.LoadSynchronous()->BrushData;
		}
	}
	return PaintingBrushData;
}

void UPaintingComponent::ShowRadiusPreview() const
{
	DrawDebugSphere(GetWorld(), GetComponentLocation(), PaintRadius, 12, FColor::Red, false, .5f, 0, 6.f);
}