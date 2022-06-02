// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingSubsystem.h"

#include "Engine/Canvas.h"
#include "Kismet/KismetRenderingLibrary.h"

DECLARE_STATS_GROUP(TEXT("Painting"),STATGROUP_Painting, STATCAT_Advanced);

void UPaintingSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PaintCurrentStack();
}

TStatId UPaintingSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UPaintingSubsystem, STATGROUP_Painting);
}

void UPaintingSubsystem::RegisterPointToDraw(FRenderTargetDrawPoint NewPoint, UTextureRenderTarget2D* RenderTargetToDrawOn)
{
	if(!RenderTargetToDrawOn)
	{
		UE_LOG(LogAssetData, Warning, TEXT("Missing render target"));
		return;
	}
	TArray<FRenderTargetDrawPoint>* ExistingPoints = MappedDrawPoints.Find(RenderTargetToDrawOn);
	if(ExistingPoints && ExistingPoints->Num() > 0)
	{
		ExistingPoints->Add(NewPoint);
	}
	else
	{
		MappedDrawPoints.FindOrAdd(RenderTargetToDrawOn, {NewPoint} );
	}
	
}

void UPaintingSubsystem::DrawPointOnCanvas(UCanvas* LocalCanvas, FVector2d LocalCanvasSize, FRenderTargetDrawPoint DrawPoint)
{
	FVector2D DrawSize = DrawPoint.NormalizedSize * LocalCanvasSize;
	if(DrawPoint.Brush.Material)
	{
		LocalCanvas->K2_DrawMaterial(
			DrawPoint.Brush.Material,
			(DrawPoint.NormalizedPosition * LocalCanvasSize) - (DrawSize/2),
			DrawSize,
			FVector2D(0,0),
			FVector2D(1,1),
			DrawPoint.Brush.Rotation,
			FVector2D(0.5f, 0.5f)
		);
	}
}

void UPaintingSubsystem::PaintCurrentStack()
{
	// some optimizations (like distance filter, or maybe no more than N points in a single frame should be done here behore drawing all mapped points)
	for (auto MappedPoint : MappedDrawPoints)
	{
		UCanvas* LocalCanvas;
		FVector2d LocalCanvasSize;
		FDrawToRenderTargetContext LocalContext;
		UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, MappedPoint.Key, LocalCanvas, LocalCanvasSize, LocalContext);
		for (auto DrawPoint : MappedPoint.Value)
		{
			DrawPointOnCanvas(LocalCanvas, LocalCanvasSize, DrawPoint);
		}
		UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, LocalContext);
	}
	MappedDrawPoints.Empty();
}

