#pragma once
#include "app.h"
#include "Renderer.h"

class DirectXApp :
	public App
{
public:
	DirectXApp(void);
	DirectXApp(bool fullScreen);
	virtual ~DirectXApp(void);

	virtual bool onInit();
	virtual void onRender();

protected:
	Renderer* render;

};

