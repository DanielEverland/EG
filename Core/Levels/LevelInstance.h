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
        };
        
        // Path to the .png file that contains the actual tilemap
        std::filesystem::path AssetPath;
        uint16_t AssetWidth;
        uint16_t AssetHeight;

        uint8_t TileWidth;
        uint8_t TileHeight;
        uint16_t TileCount;
        uint8_t Columns;

        std::unordered_map<uint16_t, TileData> Tiles;
    };
    
public:
    LevelInstance();
    LevelInstance(const std::string& levelDir);

    /*Rect GetSourceRectFromTileIdx(const std::string& tileSetName, uint16_t localIdx);
    TileSetData::TileData* TryGetTileDataFromIdx(const std::string& tileSetName, uint16_t localIdx);*/
    
    void LoadData();
    
private:
    std::filesystem::path LevelDirectory;
    std::unordered_map<IntVector, std::shared_ptr<Chunk>> Chunks;

    WorldData World;
    std::unordered_map<size_t, TileSetData> LoadedTileSets;

    void ParseWorldInfo();
    void ParseTileSets();
    void ParseAllChunks();
};
