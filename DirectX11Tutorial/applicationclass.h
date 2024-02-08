#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
//#ifndef _APPLICATIONCLASS_H_
//#define _APPLICATIONCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "textureshaderclass.h"
#include "fontshaderclass.h"
#include "fontclass.h"
#include "textclass.h"
#include "fpsclass.h"
#include "inputclass.h"
#include "spriteclass.h"
#include "timerclass.h"
//#include "modelclass.h"
//#include "lightshaderclass.h"
//#include "lightclass.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;


////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////

//Содержит всю графическую функциональность
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(InputClass*);

private:
	bool Render(float);
	bool UpdateFps();
	bool UpdateMouseStrings(int, int, bool);
	bool ShowPressedKey(unsigned char keyboardSymbol);
	
private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	//ModelClass* m_Model;
	//LightShaderClass* m_LightShader;
	//LightClass* m_Light;
	//LightClass* m_PointLights;
	//int m_numLights;

	TextureShaderClass* m_TextureShader;
	SpriteClass* m_Sprite;
	TimerClass* m_Timer;

	FontShaderClass* m_FontShader;
	FontClass* m_Font;
	TextClass* m_TextStrings;
	TextClass* m_MouseStrings;
	TextClass* m_KeyboardSymbol;
	FpsClass* m_Fps;
	TextClass* m_FpsString;
	int m_previousFps;
};

//#endif
