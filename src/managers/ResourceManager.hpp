#pragma once

#include "../common/CommonEnum.hpp"
#include <Thor/Resources/ResourceHolder.hpp>

template<typename T>
class ResourceManager
{
public:
	ResourceManager() = default;
	ResourceManager(const std::string& filepath, thor::Resources::KnownIdStrategy known = thor::Resources::Reuse);
	const T& operator[](const std::string& key) const;
	
	const T& get(const std::string) const;
	SystemStatus load(const std::string filepath, thor::Resources::KnownIdStrategy known = thor::Resources::Reuse);
	SystemStatus load(const std::string, const std::string filepath, thor::Resources::KnownIdStrategy known = thor::Resources::Reuse);
	SystemStatus unload();
	SystemStatus unload(const std::string);

private:
	thor::ResourceHolder<T, std::string> m_holder;
};

template<typename T>
inline ResourceManager<T>::ResourceManager(const std::string& filepath, thor::Resources::KnownIdStrategy known)
{
}

template<typename T>
inline const T& ResourceManager<T>::operator[](const std::string& key) const
{
	// TODO: insert return statement here
}

template<typename T>
inline const T& ResourceManager<T>::get(const std::string) const
{
	// TODO: insert return statement here
}

template<typename T>
inline SystemStatus ResourceManager<T>::load(const std::string filepath, thor::Resources::KnownIdStrategy known)
{
	// TODO: insert return statement here
}

template<typename T>
inline SystemStatus ResourceManager<T>::load(const std::string, const std::string filepath, thor::Resources::KnownIdStrategy known)
{
	// TODO: insert return statement here
}

template<typename T>
inline SystemStatus ResourceManager<T>::unload()
{
	// TODO: insert return statement here
}

template<typename T>
inline SystemStatus ResourceManager<T>::unload(const std::string)
{
	// TODO: insert return statement here
}
