// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <filesystem>
#include <regex>
#include <string>
#include <unordered_map>

#include "json.hpp"
#include "MapChunk.h"
#include "DataStructrues/Vector.h"
#include "Logging/Logger.h"
#include "Primitives/Rect.h"

// Deserialized version of a map that has been serialized to disk
// Anything that references "Map" is related to immutable data deserialized from disk
// Anything that references "Level" is what's currently in-memory and mutable
class Map
{
    struct TileSetReference
    {
        std::string Source;
        uint32_t MinGid;
        uint32_t MaxGid;
    };
    
    class TileSetReferences
    {
    public:
        void AddRef(std::string source, uint32_t firstGid);
        TileSetReference GetTileset(uint32_t id) const;
        
    private:
        std::vector<TileSetReference> References;
    };
    
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
    TileInfo GetCellInfoFromWorldPosition(IntVector cellWorldPosition) const;
    const std::unordered_map<IntVector, std::shared_ptr<MapChunk>>& GetChunks() const { return Chunks; } 
    
    void LoadData();
    
private:
    typedef std::unordered_map<size_t, TileInfo> TileSetContainer;
    
    std::filesystem::path LevelDirectory;
    std::unordered_map<IntVector, std::shared_ptr<MapChunk>> Chunks;

    WorldData World;
    std::unordered_map<std::string, TileSetContainer> TilesetNameToContainer;

    void ParseWorldInfo();
    void ParseTileSets();
    void ParseAllChunks();
    void ParseObjectLayer(nlohmann::json& chunkData, const IntVector2D& mapPos, const TileSetReferences& tileSetReferences);
    void ParseTileLayer(nlohmann::json& chunkData, const IntVector2D& mapPos, const TileSetReferences& tileSetReferences);
    bool TryGetMapCell(const TileSetReferences& tileSetReferences, uint32_t id, TileInfo& info);
    std::shared_ptr<MapChunk> GetOrCreateChunk(const IntVector& chunkPos);

    template<class ValueType, typename... Args>
    bool TryGetValue(nlohmann::json& data, const std::string& key, ValueType& value, spdlog::format_string_t<Args...> fmt, Args &&...args)
    {
        auto entry = data.find(key);
        if (entry == data.end())
        {
            Logger::Log(Engine, Warning, fmt, std::forward<Args>(args)...);
            return false;
        }
        value = entry.value();
        return true;
    }

    template<class ValueType, typename... Args>
    requires std::is_integral_v<ValueType>
    bool TryGetIntegerValue(nlohmann::json& data, const std::string& key, ValueType& value, spdlog::format_string_t<Args...> fmt, Args &&...args)
    {
        auto entry = data.find(key);
        if (entry == data.end() || !entry->is_number_integer())
        {
            Logger::Log(Engine, Warning, fmt, std::forward<Args>(args)...);
            return false;
        }
        value = entry.value();
        return true;
    }
};
