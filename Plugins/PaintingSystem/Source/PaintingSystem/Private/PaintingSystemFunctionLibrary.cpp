// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingSystemFunctionLibrary.h"

FVector2d UPaintingSystemFunctionLibrary::RemapWorldLocationToRenderTargetLocalCoords(FVector WorldLocation,
	UStaticMeshComponent* MeshComponent)
{
	if(!MeshComponent)
	{
		return FVector2d::ZeroVector;
	}
	FVector Min;
	FVector Max;
	MeshComponent->GetLocalBounds(Min, Max);
	const FVector Extent = MeshComponent->GetComponentScale() * (Max - Min) / 2;
	const FVector OffsetFromStart = WorldLocation - MeshComponent->GetComponentLocation();
	const FVector LocallyMappedOffset = MeshComponent->GetComponentRotation().GetInverse().RotateVector(OffsetFromStart);

	FVector2d Result;
	Result.X = FMath::GetMappedRangeValueClamped(FVector2f(-Extent.X, Extent.X), {0,1}, LocallyMappedOffset.X);
	Result.Y = FMath::GetMappedRangeValueClamped(FVector2f(-Extent.Y, Extent.Y), {0,1}, LocallyMappedOffset.Y);
	return Result;
}
