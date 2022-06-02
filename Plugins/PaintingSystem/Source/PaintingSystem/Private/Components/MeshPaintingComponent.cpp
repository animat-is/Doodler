// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MeshPaintingComponent.h"

#include "Engine/TextureRenderTarget2D.h"
#include "PaintingSystemFunctionLibrary.h"

UMeshPaintingComponent::UMeshPaintingComponent()
{
	PaintRadius = 16;
	SetUsingAbsoluteLocation(true);
	SetUsingAbsoluteRotation(true);
	SetUsingAbsoluteScale(true);

}

void UMeshPaintingComponent::PreparePaintingData(UStaticMeshComponent* PaintableMesh, FVector NewWorldLocation)
{
	MeshComponent = PaintableMesh;
	SetWorldLocation(NewWorldLocation);
}

FRenderTargetDrawPoint UMeshPaintingComponent::CalculatePointToDraw()
{
	FRenderTargetDrawPoint Result;
	if(!MeshComponent)
	{
		UE_LOG(LogPainting, Warning, TEXT("%s can't draw on mesh. No component matched!"), *GetOwner()->GetName());
		Deactivate();	
		return Result;
	}
	Result.NormalizedPosition = UPaintingSystemFunctionLibrary::RemapWorldLocationToRenderTargetLocalCoords(GetComponentLocation(), MeshComponent);
	Result.NormalizedSize = FVector2d(PaintRadius / RenderTargetToDrawOn->SizeX, PaintRadius / RenderTargetToDrawOn->SizeY);
	Result.Brush = GetPaintingBrushData();
	return Result;
}
