/**
 * @author Ivan Gagis <igagis@gmail.com>
 */


#pragma once

#include "../Widget.hpp"


namespace morda{

class BlurGlass : virtual public Widget{
public:
	BlurGlass(const stob::Node* chain = nullptr);
	
	BlurGlass(const BlurGlass&) = delete;
	BlurGlass& operator=(const BlurGlass&) = delete;
	
	
	void render(const morda::Matr4r& matrix) const override;
	
private:

};

}
