
template<typename Resource, typename Identifier>
ResourceHolder<Resource, Identifier>::ResourceHolder()
	: resourceMap_()
{
}


//-----------------------------------------------------------------------------

template<typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& path)
{
	if(resourceMap_.find(id) != resourceMap_.end())
		return;
	
	std::unique_ptr<Resource> resource (new Resource());
	assert(resource->loadFromFile(path));

	addResource(id, std::move(resource));
}


template<typename Resource, typename Identifier>
template<typename... Args>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& path, Args... args)
{
	if(resourceMap_.find(id) != resourceMap_.end())
		return;
	
	std::unique_ptr<Resource> resource (new Resource());
	assert(resource->loadFromFile(path, args...));

	addResource(id, std::move(resource));
}


//-----------------------------------------------------------------------------

template<typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
	auto found = resourceMap_.find(id);
	assert(found != resourceMap_.end());

	return *(found->second);
}

template<typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto found = resourceMap_.find(id);
	assert(found != resourceMap_.end());

	return *(found->second);
}


//-----------------------------------------------------------------------------

template<typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::addResource(Identifier id, std::unique_ptr<Resource> resource)
{
	auto inserted = resourceMap_.emplace(id, std::move(resource));
	assert(inserted.second);
}
