#pragma once

#include "CoreMinimal.h"
#include "PaintingTypes.h"
#include "Engine/DataAsset.h"
#include "BrushAsset.generated.h"

UCLASS()
class PAINTINGSYSTEM_API UBrushAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPaintingBrush BrushData;
};
