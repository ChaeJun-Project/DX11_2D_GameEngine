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

public:
    CLONE(Animator);

private:

};

