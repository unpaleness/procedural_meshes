#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "Base.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProceduralMeshes, Log, All)

UCLASS()
class PROCEDURALMESHES_API ABase : public AActor {
	GENERATED_BODY()

public:
	ABase();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	virtual void GenerateMesh();
	virtual bool HasChanges();
	virtual void InitArrays();
	void AddTriangle(int32 V1, int32 V2, int32 V3);

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
		UProceduralMeshComponent* Mesh;
	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "ProceduralMesh|Settings")
		float Size = 10.0f;

	float InnerSize = 0.0f;
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UVs;
};
