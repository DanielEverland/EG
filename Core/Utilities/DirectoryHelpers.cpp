#include "DirectoryHelpers.h"

#include "CoreFramework/EngineStatics.h"

std::filesystem::path DirectoryHelpers::RelativeToFullPath(const std::filesystem::path& path)
{
    std::string workingDir = EngineStatics::BootDir;
    std::string fullPath = workingDir + path.generic_string();
    return fullPath;
}
std::filesystem::path DirectoryHelpers::GetContentPath(const std::filesystem::path& path)
{
    return RelativeToFullPath(/*"Content/" +*/ path.generic_string());

}
