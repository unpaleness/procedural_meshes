#pragma once

#include "CoreMinimal.h"
#include "Base.h"
#include "Cube.generated.h"

UCLASS()
class PROCEDURALMESHES_API ACube : public ABase {
	GENERATED_BODY()

public:
	ACube();

protected:
	virtual void InitArrays() override;
};
