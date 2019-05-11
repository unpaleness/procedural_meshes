#pragma once

#include "CoreMinimal.h"
#include "Base.h"
#include "HeavyPlane.generated.h"

UCLASS()
class PROCEDURALMESHES_API AHeavyPlane : public ABase {
	GENERATED_BODY()

public:
	AHeavyPlane();

protected:
	virtual void GenerateMesh() override;
	virtual bool HasChanges() override;
	virtual void InitArrays() override;

protected:
	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Settings", meta = (ClampMin = "2", ClampMax = "256", UIMin = "2", UIMax = "256"))
		int32 VerticesPerSide = 2;

	int32 InnerVerticesPerSide = 0;
};
