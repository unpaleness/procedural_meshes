#pragma once

#include "CoreMinimal.h"
#include "Base.h"
#include "ProceduralMeshComponent.h"
#include "Cube.generated.h"

UCLASS()
class PROCEDURALMESHES_API ACube : public ABase {
	GENERATED_BODY()

public:
	ACube();

protected:
	virtual void GenerateMesh() override;
	virtual bool HasChanges() override;
	virtual void InitArrays() override;

protected:
	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Settings")
		float Size = 10.0f;

	float InnerSize = 0.0f;
};
