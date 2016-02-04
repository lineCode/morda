#include "App.hpp"

#include "resources/ResSTOB.hpp"

#include <utki/debug.hpp>
#include <papki/FSFile.hpp>
#include <papki/RootDirFile.hpp>



using namespace morda;



utki::IntrusiveSingleton<App>::T_Instance App::instance;



void App::render(){
	//TODO: render only if needed?
	
//	TRACE(<< "App::Render(): invoked" << std::endl)
	if(!this->rootWidget){
		TRACE(<< "App::Render(): root widget is not set" << std::endl)
		return;
	}
	
	Render::clearColor();
	
	if(this->windowParams.buffers.get(WindowParams::EBuffers::DEPTH)){
		Render::clearDepth();
	}
	if(this->windowParams.buffers.get(WindowParams::EBuffers::STENCIL)){
		Render::clearStencil();
	}
	
	Render::setCullEnabled(true);
	
	morda::Matr4r m;
	m.identity();
	m.translate(-1, -1);
	m.scale(Vec2r(2.0f).compDivBy(this->curWinRect.d));
	
	ASSERT(this->rootWidget)
	
	if(this->rootWidget->needsRelayout()){
		this->rootWidget->relayoutNeeded = false;
		this->rootWidget->layOut();
	}
	
	this->rootWidget->renderInternal(m);
	
	Render::swapFrameBuffers();
}



void App::updateWindowRect(const morda::Rectr& rect){
	if(this->curWinRect == rect){
		return;
	}
	
	this->curWinRect = rect;

	TRACE(<< "App::UpdateWindowRect(): this->curWinRect = " << this->curWinRect << std::endl)
	Render::setViewport(kolme::Recti(
			int(this->curWinRect.p.x),
			int(this->curWinRect.p.y),
			int(this->curWinRect.d.x),
			int(this->curWinRect.d.y)
		));
	
	if(!this->rootWidget){
		return;
	}
	
	this->rootWidget->resize(this->curWinRect.d);
}



void App::handleMouseMove(const morda::Vec2r& pos, unsigned id){
	if(!this->rootWidget){
		return;
	}
	
	if(this->rootWidget->isInteractive()){
		this->rootWidget->setHovered(this->rootWidget->rect().overlaps(pos), id);
		this->rootWidget->onMouseMove(this->nativeWindowToRootCoordinates(pos), id);
	}
}



void App::handleMouseButton(bool isDown, const morda::Vec2r& pos, Widget::EMouseButton button, unsigned pointerID){
	if(!this->rootWidget){
		return;
	}

	if(this->rootWidget->isInteractive()){
		this->rootWidget->setHovered(this->rootWidget->rect().overlaps(pos), pointerID);
		this->rootWidget->onMouseButton(isDown, this->nativeWindowToRootCoordinates(pos), button, pointerID);
	}
}



void App::handleMouseHover(bool isHovered, unsigned pointerID){
	if(!this->rootWidget){
		return;
	}
	
	this->rootWidget->setHovered(isHovered, pointerID);
}

void App::handleKeyEvent(bool isDown, EKey keyCode){
	//		TRACE(<< "HandleKeyEvent(): is_down = " << is_down << " is_char_input_only = " << is_char_input_only << " keyCode = " << unsigned(keyCode) << std::endl)

	if(auto w = this->focusedWidget.lock()){
		//			TRACE(<< "HandleKeyEvent(): there is a focused widget" << std::endl)
		w->onKeyInternal(isDown, keyCode);
	}else{
		//			TRACE(<< "HandleKeyEvent(): there is no focused widget, passing to rootWidget" << std::endl)
		if(this->rootWidget){
			this->rootWidget->onKeyInternal(isDown, keyCode);
		}
	}
}




#if M_OS_NAME != M_OS_NAME_ANDROID && M_OS_NAME != M_OS_NAME_IOS
std::unique_ptr<papki::File> App::createResourceFileInterface(const std::string& path)const{
	return utki::makeUnique<papki::FSFile>(path);
}

void App::showVirtualKeyboard()noexcept{
	TRACE(<< "App::ShowVirtualKeyboard(): invoked" << std::endl)
	//do nothing
}



void App::hideVirtualKeyboard()noexcept{
	TRACE(<< "App::HideVirtualKeyboard(): invoked" << std::endl)
	//do nothing
}
#endif


#if (M_OS == M_OS_LINUX && M_OS_NAME != M_OS_NAME_ANDROID) || M_OS == M_OS_MACOSX

App::ResMan::ResMan(){
	//mount default resource pack
	
#	ifndef DEBUG
	unsigned soname = 
#	include "../soname.txt"
		;
#	endif
		
	std::vector<std::string> paths;
	
#	if M_OS_NAME == M_OS_NAME_IOS || M_OS_NAME == M_OS_NAME_ANDROID
		paths.push_back("respack/");
#	else
#		ifdef DEBUG
		paths.push_back("../../respack/");
#		else
		{
			std::stringstream ss;
			ss << "/usr/local/share/morda/res" << soname << "/";
			paths.push_back(ss.str());
		}
		{
			std::stringstream ss;
			ss << "/usr/share/morda/res" << soname << "/";
			paths.push_back(ss.str());
		}
#		endif
#	endif

	for(const auto& s : paths){
		try{
//			TRACE(<< "s = " << s << std::endl)
			auto fi = morda::App::inst().createResourceFileInterface(s);
			ASSERT(fi)
			this->mountResPack(*fi);
		}catch(papki::Exc& e){
			continue;
		}
		break;
	}
}

#endif



void App::setFocusedWidget(const std::shared_ptr<Widget> w){
	if(auto prev = this->focusedWidget.lock()){
		prev->isFocused_var = false;
		prev->onFocusedChanged();
	}
	
	this->focusedWidget = w;
	
	if(w){
		w->isFocused_var = true;
		w->onFocusedChanged();
	}
}


App::Inflater::Inflater(){
	try{
		auto t = morda::App::inst().resMan.load<ResSTOB>("morda_gui_definitions");
		
		if(t->chain()){
			this->inflate(*t->chain());
		}
		
	}catch(ResourceManager::Exc&){
		//ignore
		TRACE(<< "App::Inflater::Inflater(): could not load morda_gui_definitions" << std::endl)
	}
}
