
template<typename T>
auto DifficultyManager::selectDataIndex(std::vector<T> v)
{
	return std::min(v.size()-1, std::size_t(phaseNumber_));
}
