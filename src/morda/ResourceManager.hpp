/**
 * @author Ivan Gagis <igagis@gmail.com>
 */

#pragma once

#include <map>

#include <utki/Shared.hpp>
#include <papki/File.hpp>

#include <stob/dom.hpp>

#include "Exc.hpp"


namespace morda{



class App;
class Resource;



class ResourceManager{
	friend class morda::App;
	friend class Resource;
	
	std::map<const std::string, std::weak_ptr<Resource>> resMap;

	class ResPackEntry{
	public:
		ResPackEntry() = default;
		
		//For MSVC compiler, it does not generate move constructor automatically
		ResPackEntry(ResPackEntry&& r){
			this->fi = std::move(r.fi);
			this->resScript = std::move(r.resScript);
		}

		std::unique_ptr<const papki::File> fi;
		std::unique_ptr<const stob::Node> resScript;
	};

	typedef std::vector<ResPackEntry> T_ResPackList;

	//list of mounted resource packs
	T_ResPackList resPacks;


	class FindInScriptRet{
	public:
		FindInScriptRet(ResPackEntry& resPack, const stob::Node& element) :
				rp(resPack),
				e(element)
		{}

		ResPackEntry& rp;
		const stob::Node& e;
	};

	FindInScriptRet findResourceInScript(const std::string& resName);

	template <class T> std::shared_ptr<T> findResourceInResMap(const char* resName);

	//Add resource to resources map
	void addResource(const std::shared_ptr<Resource>& res, const stob::Node& node);

private:
	ResourceManager() = default;

public:
	class Exc : public morda::Exc{
	public:
		Exc(const std::string& message) :
				morda::Exc(message)
		{}
	};

	//if fi does not point to res script, then "main.res.stob" is assumed.
	void mountResPack(const papki::File& fi);

	template <class T> std::shared_ptr<T> load(const char* resName);
	
private:
};



//base class for all resources
class Resource : virtual public utki::Shared{
	friend class ResourceManager;
protected:
	//this can only be used as a base class
	Resource(){}
public:
	virtual ~Resource()noexcept{}
};



template <class T> std::shared_ptr<T> ResourceManager::findResourceInResMap(const char* resName){
	auto i = this->resMap.find(resName);
	if(i != this->resMap.end()){
		if(auto r = (*i).second.lock()){
			return std::dynamic_pointer_cast<T>(std::move(r));
		}
	}
	return nullptr;//no resource found with given name, return invalid reference
}



template <class T> std::shared_ptr<T> ResourceManager::load(const char* resName){
//	TRACE(<< "ResMan::Load(): enter" << std::endl)
	if(auto r = this->findResourceInResMap<T>(resName)){
//		TRACE(<< "ResManHGE::Load(): resource found in map" << std::endl)
		return r;
	}

//	TRACE(<< "ResMan::Load(): searching for resource in script..." << std::endl)
	FindInScriptRet ret = this->findResourceInScript(resName);
	ASSERT(ret.rp.fi)

//	TRACE(<< "ResMan::Load(): resource found in script" << std::endl)

	if(!ret.e.child()){
		throw Exc("ResourceManager::Load(): resource description is empty");
	}
	
	auto resource = T::load(*ret.e.child(), *ret.rp.fi);

	this->addResource(resource, ret.e);

//	TRACE(<< "ResMan::LoadTexture(): exit" << std::endl)
	return resource;
}







}//~namespace
