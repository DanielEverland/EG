#include "Navigation.h"

#include <queue>
#include <unordered_set>

#include "Level.h"
#include "NavRequest.h"
#include "Components/NavigableComponent.h"

NavResult Navigation::TryCalculatePath(const NavRequest& request) const
{
    typedef std::pair<IntVector, double> Node;
        
    std::shared_ptr<NavigationGraphTraverser> traverser = request.Traverser;
    assert(traverser.get() != nullptr);
    
    NavResult result;
    result.Succeeded = false;

    uint8_t _;
    bool targetPositionNavigable = traverser->TryGetTraversalDifficulty(request.TargetPosition, request.TargetPosition, _);
    bool startPositionNavigable = traverser->TryGetTraversalDifficulty(request.StartPosition, request.StartPosition, _);
    if (!targetPositionNavigable || !startPositionNavigable)
    {
        return result;
    }
    
    static auto comp = [](const Node& a, const Node& b)
    {
        return a.second > b.second;
    };
    std::priority_queue<Node, std::vector<Node>, decltype(comp)> openNodes;
    std::unordered_map<IntVector, uint16_t> costs;
    std::unordered_map<IntVector, IntVector> parents;
    int32_t iterations = 0;

    static IntVector neighborPositions[] =
    {
        IntVector(1, 0, 0),
        IntVector(0, 1, 0),
        IntVector(-1, 0, 0),
        IntVector(0, -1, 0),

        IntVector(1, 1, 0),
        IntVector(-1, -1, 0),
        IntVector(-1, 1, 0),
        IntVector(1, -1, 0),
        
        IntVector(0, 0, 1),
        IntVector(0, 0, -1),
    };
    
    openNodes.emplace(request.StartPosition, 0);
    while (!openNodes.empty())
    {
        if (++iterations >= MaxIterations)
            break;
        
        Node curr = openNodes.top();
        openNodes.pop();

        IntVector currentPosition = curr.first;
        IntVector targetDiff = request.TargetPosition - currentPosition;
        if (abs(targetDiff.X) <= request.PermittedTargetDistances.X && abs(targetDiff.Y) <= request.PermittedTargetDistances.Y && abs(targetDiff.Z) <= request.PermittedTargetDistances.Z)
        {
            std::list<IntVector>& pathList = result.Path;
            pathList.clear();
            do
            {
                pathList.emplace_back(currentPosition);
                currentPosition = parents[currentPosition];
            }
            while (pathList.back() != request.StartPosition);
            pathList.reverse();
            result.Succeeded = true;
            return result;
        }

        for (const IntVector& offset : neighborPositions)
        {
            IntVector neighborPos = currentPosition + offset;

            if (!request.Traverser->IsValidMove(request.EntityToMove, currentPosition, neighborPos))
                continue;

            uint8_t neighborTraversalDifficulty = 0;
            bool succeeded = request.Traverser->TryGetTraversalDifficulty(currentPosition, neighborPos, neighborTraversalDifficulty);
            if (!succeeded)
                continue;

            assert(neighborTraversalDifficulty != 0);

            uint16_t neighborActualCost = costs[currentPosition] + neighborTraversalDifficulty;
            auto neighborCostEntry = costs.find(neighborPos);
            if (neighborCostEntry == costs.end() || neighborCostEntry->second > neighborActualCost)
            {
                costs[neighborPos] = neighborActualCost;
                parents[neighborPos] = currentPosition;
                
                double h = GetHeuristic(neighborPos, request.TargetPosition);
                double f = h + neighborActualCost;
                openNodes.emplace(neighborPos, f);
            }
        }
    }
    return result;
}

double Navigation::GetHeuristic(const IntVector& pos, const IntVector& target)
{
    int32_t dx = std::abs(pos.X - target.X);
    int32_t dy = std::abs(pos.Y - target.Y);
    int32_t dz = std::abs(pos.Z - target.Z);
    return dx + dy + dz;
}
