#pragma once

#include "stdafx.h"

class IObject : public DX11Obejct
{
public:
    IObject();
    virtual ~IObject() = default;

 public:
     virtual void Update() = 0;
     virtual void Render() = 0;

public:
     

protected:
     std::string object_name;
     std::string object_tag;
};