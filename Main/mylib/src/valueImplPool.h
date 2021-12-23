#pragma once
#include <map>
#include <list>
#include <algorithm>
#include <memory>
template <class ValueType>
class TValueImplPool
{
public:
	typedef std::shared_ptr<ValueType> SharedPtr;
	typedef std::weak_ptr<ValueType> WeakPtr;
	std::map<size_t, std::list<WeakPtr>> buckets;

	SharedPtr GetOrCreate(const typename ValueType::InternalType &internalData)
	{
		size_t hash = ValueType::GetDataHash(internalData);
		auto &bucket = buckets[hash];
		for (auto iter = bucket.begin(); iter != bucket.end();)
		{
			if (SharedPtr s = (*iter).lock())
			{
				if (s->equalTo(internalData))
					return s;
				++iter;
			}
			else
			{
				iter = bucket.erase(iter);
			}
		}
		auto ptr = std::make_shared<ValueType>(internalData);
		bucket.push_back(ptr);
		return ptr;
	}
};