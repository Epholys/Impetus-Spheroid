#ifndef RESOURCE_HOLDER_HPP
#define RESOURCE_HOLDER_HPP


#include <string>
#include <map>
#include <memory>

#include "framework/Assertion.hpp"

template<typename Resource, typename Identifier>
class ResourceHolder
{
public:
	ResourceHolder();

	void load(Identifier id, const std::string& path);

	template<typename... Args>
	void load(Identifier id, const std::string& path, Args... args);

	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;

private:
	void addResource(Identifier id, std::unique_ptr<Resource> resource);
	
private:
	std::map<Identifier, std::unique_ptr<Resource>> resourceMap_;
};

#include "framework/ResourceHolder.ipp"



	
#endif // RESOURCE_HOLDER_HPP
