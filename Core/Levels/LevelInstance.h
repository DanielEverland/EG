// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <filesystem>
#include <regex>
#include <string>
#include <unordered_map>

#include "Chunk.h"
#include "DataStructrues/Vector.h"
#include "Primitives/Rect.h"

class LevelInstance
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
    LevelInstance();
    LevelInstance(const std::string& levelDir);

    Rect GetSourceRectFromWorldPosition(IntVector cellWorldPosition) const;
    CellInfo GetCellInfoFromWorldPosition(IntVector cellWorldPosition) const;
    const std::unordered_map<IntVector, std::shared_ptr<Chunk>>& GetChunks() const { return Chunks; } 
    
    void LoadData();
    
private:
    std::filesystem::path LevelDirectory;
    std::unordered_map<IntVector, std::shared_ptr<Chunk>> Chunks;

    WorldData World;
    std::unordered_map<size_t, CellInfo> IdToCellTemplate;

    void ParseWorldInfo();
    void ParseTileSets();
    void ParseAllChunks();
};
