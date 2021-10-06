#pragma once
#include "IObject.h"
class MainCamera : public IObject
{
public:
	MainCamera();
	~MainCamera();

	void Initialize() override;

	void Update() override;
	void FinalUpdate() override;

	void Render() override;
};

