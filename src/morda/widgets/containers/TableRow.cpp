#include "TableRow.hpp"


using namespace morda;


TableRow::TableRow(const stob::Node* chain) :
		Widget(chain),
		HorizontalContainer(chain)
{}


TableRow::LayoutParams::LayoutParams(const stob::Node* chain) :
		HorizontalContainer::LayoutParams(chain),
		modifiedParams(nullptr)
{}



Widget::LayoutParams& TableRow::getLayoutParams_internal(Widget& w){
	auto lp = dynamic_cast<LayoutParams*>(&this->HorizontalContainer::getLayoutParams_internal(w));
	
	if(!lp){
		throw morda::Exc("TableRow: layout parameters can only be of type TableRow::LayoutParams, other type found");
	}
	
	return lp->modifiedParams;
}



TableRow::LayoutParams& TableRow::getTableRowLayoutParams(Widget& w) {
	auto lp = dynamic_cast<LayoutParams*>(&this->HorizontalContainer::getLayoutParams_internal(w));
	if(!lp){
		throw morda::Exc("TableRow: layout parameters can only be of type TableRow::LayoutParams, other type found");
	}
	return *lp;
}
