#pragma once
#include "inputinterface.h"
#include "SDLHandler.h"
class AdjustableInterface :
	public InputInterface, public SDLHandler
{
public:
	AdjustableInterface(InputInterface* inputinterface, App* app) : SDLHandler(app), input(inputinterface), t(Eigen::Vector3d::Zero()), selected(false) {;}
	~AdjustableInterface(void);
	virtual void update() { input->update(); }
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
protected:
	virtual InputStatus _getStatus(void);
private:
	AdjustableInterface();
	Eigen::Vector3d t;
	InputInterface* input;
	bool selected;
};

