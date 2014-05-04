#include "MouseKeyboardInterface.h"
#include <cstdio>

MouseKeyboardInterface::MouseKeyboardInterface() : InputInterface()
{
}

MouseKeyboardInterface::~MouseKeyboardInterface()
{
	delete status;
}

void MouseKeyboardInterface::SDLupdate(SDL_Event* Event)
{
	switch(Event->type)
	{
		case SDL_ACTIVEEVENT: {
            switch(Event->active.state) {
                case SDL_APPMOUSEFOCUS: {
                    if ( Event->active.gain )
					{
						OutputDebugString("Mouse Focus");
					}
                    else
					{
						OutputDebugString("Mouse Blur");							
					}
                    break;
                }
                case SDL_APPINPUTFOCUS: {
                    if ( Event->active.gain )
					{
						OutputDebugString("Input Focus");
					}
                    else
					{
						OutputDebugString("Input Blur");					
					}
                    break;
                }
                case SDL_APPACTIVE:    {
                    if ( Event->active.gain )
					{
						OutputDebugString("Active Focus");								
					}
                    else
					{
						OutputDebugString("Active Blur");								
					}
                    break;
                }
            }
            break;
        }
 
        case SDL_KEYDOWN: {
			OutputDebugString("Key Down: ");
			char buffer[16];
			sprintf_s(buffer, "%c", Event->key.keysym.unicode);
			OutputDebugString(buffer);
			OutputDebugString("\n");

            break;
        }
 
        case SDL_MOUSEMOTION: {
			OutputDebugString("Mouse Motion: ");
			char buffer[16];
			sprintf_s(buffer, "%d", Event->motion.x);
			OutputDebugString(buffer);
			OutputDebugString(",");
			sprintf_s(buffer, "%d", Event->motion.y);
			OutputDebugString(buffer);
			OutputDebugString(",");
			sprintf_s(buffer, "%d", Event->motion.xrel);
			OutputDebugString(buffer);
			OutputDebugString(",");
			sprintf_s(buffer, "%d", Event->motion.yrel);
			OutputDebugString(buffer);
			OutputDebugString("\n");

            break;
        }
 
        case SDL_MOUSEBUTTONDOWN:
		{
            switch(Event->button.button)
			{
                case SDL_BUTTON_LEFT:
				{
					OutputDebugString("Left Button: ");
					char buffer[16];
					sprintf_s(buffer, "%d", Event->button.x);
					OutputDebugString(buffer);
					OutputDebugString(",");
					sprintf_s(buffer, "%d", Event->button.y);
					OutputDebugString(buffer);
					OutputDebugString("\n");

                    break;
                }
				default:
				{
					// Do nothing
				}
            }
            break;
        }

		default:
		{
			// Do nothing
		}
	}
}

void MouseKeyboardInterface::update()
{
}