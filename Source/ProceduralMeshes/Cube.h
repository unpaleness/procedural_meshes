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
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	virtual void GenerateMesh() override;
	virtual void CheckChanges() override;
	virtual void InitArrays() override;
	virtual void InitVertices() override;

protected:
	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Settings")
		float Size = 10.0f;

	float InnerSize = 0.0f;
};
