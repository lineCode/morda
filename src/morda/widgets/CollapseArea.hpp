#pragma once

#include "core/Widget.hpp"
#include "core/container/LinearContainer.hpp"
#include "core/container/Pile.hpp"

namespace morda{
class CollapseArea :
		virtual public Widget,
		private Column
{
	std::shared_ptr<Pile> contentArea;
	std::shared_ptr<Pile> title_v;
public:
	CollapseArea(const stob::Node* chain = nullptr);
	
	Pile& content()noexcept{
		ASSERT(this->contentArea)
		return *this->contentArea;
	}
	
	Pile& title()noexcept{
		ASSERT(this->title_v)
		return *this->title_v;
	}
};
}
