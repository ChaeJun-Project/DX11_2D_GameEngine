#pragma once

#include "IComponent.h"

class Animator final : public IComponent
{
public:
	Animator();
	explicit Animator(const Animator& origin);
	~Animator();

	void Update() override;
	void FinalUpdate() override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
    CLONE(Animator);

private:

};

