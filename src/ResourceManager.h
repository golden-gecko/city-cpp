#pragma once

#include "Precompiled.h"

template <typename Base>
class BaseFactory
{
public:
	virtual Base* create(const std::string& name) = 0;
	virtual void destroy(const std::string& name) = 0;
	virtual void destroy(Base* base) = 0;
};

template <typename Base, typename Type>
class ResourceFactory : public BaseFactory<Base>
{
protected:
	std::list<Type*> mList;
public:
	Base* create(const std::string& name)
	{
		Type* type = new Type(name);

		mList.push_back(type);

		return type;
	}

	Base* get(const std::string& name)
	{
		for (std::list<Type*>::iterator itr = mList.begin(); itr != mList.end(); itr++)
			if ((*itr)->getName() == name)
				return *itr;

		return NULL;
	}

	void destroy(const std::string& name)
	{
	}

	void destroy(Base* base)
	{
		mList.remove(static_cast<Type*>(base));
	}
};

template <typename Base>
class ResourceManager
{
protected:
	std::map<std::string, BaseFactory<Base>*> mMap;
public:
	ResourceManager(const std::string& type = "")
	{
		if (type != "")
		{
			registerType<Base>(type);
		}
	}

	~ResourceManager()
	{
		std::map<std::string, BaseFactory<Base>*>::iterator itr;

		for (itr = mMap.begin(); itr != mMap.end(); itr++)
			delete itr->second;
	}

	template <typename Type>
	void registerType(const std::string& type)
	{
		mMap[type] = new ResourceFactory<Base, Type>;
	}

	Base* create(const std::string& type, const std::string& name)
	{
		return mMap[type]->create(name);
	}

	Base* get(const std::string& type, const std::string& name)
	{
		return mMap[type]->get(name);
	}

	void destroy(const std::string& name)
	{
		std::map<std::string, BaseFactory<Base>*>::iterator itr;

		for (itr = mMap.begin(); itr != mMap.end(); itr++)
			itr->second->destroy(name);
	}

	void destroy(const std::string& type, const std::string& name)
	{
		mMap[type]->destroy(name);
	}

	void destroy(Base* base)
	{
		std::map<std::string, BaseFactory<Base>*>::iterator itr;

		for (itr = mMap.begin(); itr != mMap.end(); itr++)
			itr->second->destroy(base);
	}

	void destroy(const std::string& type, Base* base)
	{
		mMap[type]->destroy(base);
	}
};