#pragma once

//Interface(순수가상함수만 선언된 클래스)
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

