#pragma once

#include "IResource.h"

//TODO: Object 설계 후 사용할지 결정
class ResourceManager final : public Singleton<ResourceManager>
{
	SINGLETON(ResourceManager);

	ResourceManager() = default;
	~ResourceManager();

public:

private:
     

};

