#pragma once

#include "CoreMinimal.h"
#include "PaintingTypes.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogPainting, Log, All);

/*
 * Data about texture used to paint on render targets. Allows to separate different methods of painting
 */
USTRUCT(BlueprintType)
struct FPaintingBrush// : public FTableRowBase
{
	GENERATED_BODY()

	FPaintingBrush() { }

	// material to paint with
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* Material = nullptr;

	// Color used to multiply brush texture
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color = FColor::White;

	// Rotation applied when painting brush to render target
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rotation = 0.f;

	bool IsValid() const
	{
		return Material != nullptr ;
	}
};

/*
 * Data required to draw on any render target texture
 */
USTRUCT(BlueprintType)
struct FRenderTargetDrawPoint// : public FTableRowBase
{
	GENERATED_BODY()
	
	FRenderTargetDrawPoint() { }

	// brush applied while painting - specifying how to paint the texture
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPaintingBrush Brush;

	// (0-1) point where to draw on the render target
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D NormalizedPosition;
	// (0-1) percentage of the render target to fill
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D NormalizedSize;

	bool bIsValid() const { return Brush.IsValid(); }
};