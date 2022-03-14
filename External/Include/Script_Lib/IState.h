#pragma once

//Interface(���������Լ��� ����� Ŭ����)
class IState
{
protected:
     IState() = default;
     virtual ~IState() = default;

public:
     virtual void Enter() = 0;
     virtual void Update() = 0;
     virtual void Exit() = 0;
};

