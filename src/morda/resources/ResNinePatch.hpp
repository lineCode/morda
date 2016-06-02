/**
 * @author Ivan Gagis <igagis@gmail.com>
 */

#pragma once

#include "../config.hpp"

#include "../ResourceManager.hpp"

#include "ResImage.hpp"

namespace morda{

class ResNinePatch : public Resource{
	friend class ResourceManager;
	
	const std::shared_ptr<ResImage> image;
	
	Sidesr borders;
	
public:
	const std::shared_ptr<ResImage> lt, t, rt, l, m, r, lb, b, rb;//TODO: remove
	
	ResNinePatch(const ResNinePatch&) = delete;
	ResNinePatch& operator=(const ResNinePatch&) = delete;
	
	ResNinePatch(const std::shared_ptr<ResImage> image, Sidesr borders) :
			image(std::move(image)),
			borders(borders)
	{}
	
	ResNinePatch(
			Sidesr borders,
			const std::shared_ptr<ResImage>& lt,
			const std::shared_ptr<ResImage>& t,
			const std::shared_ptr<ResImage>& rt,
			const std::shared_ptr<ResImage>& l,
			const std::shared_ptr<ResImage>& m,
			const std::shared_ptr<ResImage>& r,
			const std::shared_ptr<ResImage>& lb,
			const std::shared_ptr<ResImage>& b,
			const std::shared_ptr<ResImage>& rb
		) :
			borders(borders),
			lt(lt),
			t(t),
			rt(rt),
			l(l),
			m(m),
			r(r),
			lb(lb),
			b(b),
			rb(rb)
	{}
	
	class ImageMatrix :
			public std::array<std::array<std::shared_ptr<ResImage>, 3>, 3>,
			public utki::Shared
	{
		friend class ResNinePatch;
		
		std::weak_ptr<const ResNinePatch> parent;
	
		real mul;//for erasing from the cache
	public:
		ImageMatrix(std::array<std::array<std::shared_ptr<ResImage>, 3>, 3>&& l, std::shared_ptr<const ResNinePatch> parent, real mul);
		
		~ImageMatrix()noexcept;
	};
	
	std::shared_ptr<ImageMatrix> get(Sidesr borders)const;
	
	const decltype(borders)& getBorders()const noexcept{
		return this->borders;
	}
private:
	mutable std::map<real, std::weak_ptr<ImageMatrix>> cache;
	
	static std::shared_ptr<ResNinePatch> load(const stob::Node& chain, const papki::File& fi);
};

}
