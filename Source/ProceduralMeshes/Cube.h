#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "Cube.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProceduralMeshes, Log, All)

UCLASS()
class PROCEDURALMESHES_API ACube : public AActor {
	GENERATED_BODY()

public:
	ACube();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	void GenerateMesh();
	void AddTriangle(int32 V1, int32 V2, int32 V3);
	void CheckChanges();
	void InitArrays();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
		UProceduralMeshComponent* Mesh;

	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Settings")
		float Size;

	TArray<int32> Triangles;
	TArray<FVector2D> UVs;
	float InnerSize = 0.0f;
};
