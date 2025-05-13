#include "IDataStoreSerializer.hpp"

IDataStoreSerializer::IDataStoreSerializer()
    : RELATIVE_PATH(std::filesystem::current_path())
{}

IDataStoreSerializer::IDataStoreSerializer(std::filesystem::path path)
    : RELATIVE_PATH(path)
{}

std::filesystem::path IDataStoreSerializer::resolvePath(std::string path)
{
    std::filesystem::path finalPath = RELATIVE_PATH;

    while (path.substr(0, 3) == "../")
    {
        finalPath = finalPath.parent_path();
        path = path.substr(3);
    }

    return finalPath.append(path);
}
