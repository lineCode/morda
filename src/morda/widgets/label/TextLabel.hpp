#pragma once


#include "../core/Widget.hpp"
#include "../../resources/ResFont.hpp"
#include "../base/TextWidget.hpp"



namespace morda{

/**
 * @brief Text label widget.
 * This widget shows text.
 * From GUI script it can be instantiated as "TextLabel".
 */
class TextLabel : public SingleLineTextWidget{
	
	
public:
	TextLabel(const stob::Node* chain = nullptr);
	
public:
	~TextLabel()noexcept{}
	
	void render(const morda::Matr4r& matrix)const override;
};



}//~namespace
