#pragma once

#include "IState.h"

class Z_Ready final : public IState
{
public:
    Z_Ready();
    ~Z_Ready();

public:
    void Enter() override;
    void Update() override;
    void Exit() override;

private:
    
};

class Z_Idle final : public IState
{
public:
	Z_Idle();
	~Z_Idle();

public:
    void Enter() override;
    void Update() override;
    void Exit() override;

private:
};