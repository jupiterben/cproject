#pragma once
#include <map>
#include <list>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <deque>
#include <mutex>

template <class ValueType>
class TValueImplPool
{
public:
	mutable std::mutex _mtx;
	typedef std::shared_ptr<ValueType> SharedPtr;
	typedef std::weak_ptr<ValueType> WeakPtr;
	std::unordered_map<size_t, std::list<WeakPtr>> buckets;
	std::list<SharedPtr> recentValues;

	inline void addRecent(const SharedPtr &value)
	{
		recentValues.push_back(value);
		while (recentValues.size() > 20)
			recentValues.pop_front();
	}

	SharedPtr GetOrCreate(const typename ValueType::InternalType &internalData)
	{
		std::lock_guard<std::mutex> l(_mtx);
		size_t hash = std::hash<ValueType::InternalType>{}(internalData);
		auto &bucket = buckets[hash];
		for (auto iter = bucket.begin(); iter != bucket.end();)
		{
			if (SharedPtr s = (*iter).lock())
			{
				if (s->equalTo(internalData))
				{
					addRecent(s);
					return s;
				}
				++iter;
			}
			else
			{
				iter = bucket.erase(iter);
			}
		}
		auto ptr = std::make_shared<ValueType>(internalData, hash);
		bucket.push_back(ptr);
		addRecent(ptr);
		return ptr;
	}
};