#pragma once
#include <stddef.h>
class String;
class IImpl
{
public:
	virtual ~IImpl() {}
};

class IValueImpl : public IImpl
{
public:
	virtual String toString() const = 0;
	virtual size_t getHash() const = 0;
};

//////////////////////////////////////////////////////////////////////////
template <class IType>
class TValueImpl : public IValueImpl
{
public:
	typedef IType InternalType;

	TValueImpl(const InternalType &d, size_t hash)
		: internalData(d), _hash(hash) {}
	inline const InternalType &internal() const { return internalData; }
	virtual size_t getHash() const { return _hash; }
	virtual bool equalTo(const InternalType &d) const
	{
		return std::equal_to<InternalType>{}(internalData, d);
	}
protected:
	const InternalType internalData;
	size_t _hash;
};

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
	typedef TValueImplPool<ValueType> _MyType;
	typedef std::shared_ptr<ValueType> SharedPtr;
	typedef std::weak_ptr<ValueType> WeakPtr;
public:
	mutable std::mutex _mtx;
	std::unordered_map<size_t, std::list<WeakPtr>> buckets;
	std::list<SharedPtr> recentValues;
public:
	static _MyType& GetInstance()
	{
		static _MyType instance;
		return instance;
	}

	inline void addRecent(const SharedPtr &value)
	{
		recentValues.push_back(value);
		while (recentValues.size() > 20)
			recentValues.pop_front();
	}

	template <class InitType>
	SharedPtr GetOrCreate(const typename InitType &internalData)
	{
		std::lock_guard<std::mutex> l(_mtx);
		size_t hash = std::hash<InitType>{}(internalData);
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