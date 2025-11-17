// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <filesystem>
#include <regex>
#include <string>
#include <unordered_map>

#include "MapChunk.h"
#include "DataStructrues/Vector.h"
#include "Primitives/Rect.h"

// Deserialized version of a map that has been serialized to disk
// Anything that references "Map" is related to immutable data deserialized from disk
// Anything that references "Level" is what's currently in-memory and mutable
class Map
{
    struct WorldData
    {
        std::string MapPatternStr;
        std::regex MapPatternRegex;
    };

    struct TileSetData
    {
        struct TileData
        {
            HashedString TypeStr;
            HashedString Name;
        };
        
        // Path to the .png file that contains the actual tilemap
        std::filesystem::path AssetPath;
        uint16_t AssetWidth;
        uint16_t AssetHeight;

        uint8_t TileWidth;
        uint8_t TileHeight;
        uint16_t TileCount;
        uint8_t Columns;

        Rect IdToRect(uint16_t id) const;

        std::unordered_map<uint16_t, TileData> Tiles;
    };
    
public:
    Map();
    Map(const std::string& levelDir);

    Rect GetSourceRectFromWorldPosition(IntVector cellWorldPosition) const;
    MapCellInfo GetCellInfoFromWorldPosition(IntVector cellWorldPosition) const;
    const std::unordered_map<IntVector, std::shared_ptr<MapChunk>>& GetChunks() const { return Chunks; } 
    
    void LoadData();
    
private:
    std::filesystem::path LevelDirectory;
    std::unordered_map<IntVector, std::shared_ptr<MapChunk>> Chunks;

    WorldData World;
    std::unordered_map<size_t, MapCellInfo> IdToCellTemplate;

    void ParseWorldInfo();
    void ParseTileSets();
    void ParseAllChunks();
};
