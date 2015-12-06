#include "TextField.hpp"

#include "containers/BlackHoleContainer.hpp"

#include "../App.hpp"


using namespace morda;



TextField::TextField(const stob::Node* chain) :
		Widget(chain),
		ti(utki::makeShared<TextInput>(chain))
{
	this->setNinePatch(morda::App::inst().resMan.load<ResNinePatch>("morda_npt_textfield"));
	
	
	auto bhc = utki::makeShared<BlackHoleContainer>();
	bhc->setShrink(kolme::Vec2b(true, false));
	bhc->add(this->ti);
	
	{
		auto& lp = bhc->getLayoutParams(*this->ti);
		lp.dim.x = Widget::LayoutParams::D_Max;
		lp.dim.y = Widget::LayoutParams::D_Min;
	}
	
	this->content().add(bhc);
	
	{
		auto& lp = this->content().getLayoutParams(*bhc);

		lp.dim.x = Widget::LayoutParams::D_Max;
		lp.dim.y = Widget::LayoutParams::D_Min;
	}
}
