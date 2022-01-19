#pragma once

#include "IResource.h"

class TileMap final : public IResource
{
public:
	TileMap(const std::string& tile_map_resource_name);
	explicit TileMap(const TileMap& origin);
	~TileMap();

	void Update();
	void FinalUpdate();

public:
	bool SaveToFile(const std::string& tile_map_path) override;
	bool LoadFromFile(const std::string& tile_map_path) override;

public:
	CLONE(TileMap);

private:


};

