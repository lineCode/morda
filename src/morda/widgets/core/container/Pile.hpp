#pragma once


#include "Container.hpp"


namespace morda{


/**
 * @brief Pile container widget.
 * Pile container is a container which lays out its children in a pile, each widget is centered inside of the container.
 * From GUI script it can be instantiated as 'Pile'.
 */
class Pile : public Container{
public:
	Pile(const Pile&) = delete;
	Pile& operator=(const Pile&) = delete;
	
	/**
	 * @brief Constructor.
	 * @param chain - STOB chain describing the widget.
	 */
	Pile(const stob::Node* chain = nullptr);
	
public:	
	morda::Vec2r measure(const morda::Vec2r& quotum)const override;

	void layOut() override;
};


}
