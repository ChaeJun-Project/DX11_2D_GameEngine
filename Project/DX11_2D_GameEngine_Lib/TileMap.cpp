#include "stdafx.h"
#include "TileMap.h"

TileMap::TileMap(const std::string& tile_map_resource_name)
	:IResource(ResourceType::TileMap, tile_map_resource_name)
{
}

TileMap::TileMap(const TileMap& origin)
	: IResource(origin.GetResourceType(), origin.GetResourceName())
{
}

TileMap::~TileMap()
{
}

void TileMap::Update()
{
}

void TileMap::FinalUpdate()
{
}

bool TileMap::SaveToFile(const std::string& tile_map_path)
{
	return false;
}

bool TileMap::LoadFromFile(const std::string& tile_map_path)
{
	return false;
}