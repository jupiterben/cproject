#pragma once
#include <map>
#include <list>
#include <algorithm>

template<class ImplType, class InternalType = typename ImplType::InternalType>
class ImplPool
{
public:
	typedef std::shared_ptr<ImplType> SharedPtr;
    typedef std::weak_ptr<ImplType> WeakPtr;
	std::map<size_t, std::list<WeakPtr>> buckets;

	SharedPtr GetOrCreate(const InternalType & internalData)
	{
		size_t hash = ImplType::hashCode(internalData);
		auto &bucket = buckets[hash];
		for (auto iter = bucket.begin(); iter != bucket.end();)
		{
			if (SharedPtr s = (*iter).lock())
			{
				if (s->equal(internalData))
					return s;
				++iter;
			}
			else
			{
				iter = bucket.erase(iter);
			}
		}
		auto ptr = std::make_shared<ImplType>(internalData);
		bucket.push_back(ptr);
		return ptr;
	}
};