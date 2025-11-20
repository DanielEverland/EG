#include "TestDirectoryHelper.h"

std::filesystem::path TestDirectoryHelper::RelativeToFullPath(const std::filesystem::path& path)
{
    std::string workingDir = std::filesystem::current_path().generic_string();
    std::string fullPath = workingDir + path.generic_string();
    return fullPath;
}
std::filesystem::path TestDirectoryHelper::GetContentPath(const std::filesystem::path& path)
{
    return RelativeToFullPath("/Content/" + path.generic_string());
}
