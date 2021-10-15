#pragma once
#include "IComponent.h"

class Script : public IComponent
{
public:
    Script();
    ~Script() = default;

    void Update() override;
    virtual void FinalUpdate() final;

public:
    virtual Script* Clone() = 0;

public: 
    //Object Side
    const GameObjectSideState& GetObjectSideState() const { return this->m_game_object_side_state; }
    void SetObjectSideState(const GameObjectSideState& object_side_state) { this->m_game_object_side_state = object_side_state; }

private:
    //Object Side
    GameObjectSideState m_game_object_side_state = GameObjectSideState::Right;
};

