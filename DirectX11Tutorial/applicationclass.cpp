////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
	: m_Direct3D(nullptr), m_Camera(nullptr), 
	//m_Model(nullptr), m_LightShader(nullptr), m_Light(nullptr), m_PointLights(nullptr), 
	//m_TextureShader(nullptr), m_Sprite(nullptr), m_Timer(nullptr),
	m_FontShader(nullptr), m_Font(nullptr), m_TextString1(nullptr), m_TextString2(nullptr), m_TextString3(nullptr), m_Fps(nullptr), m_FpsString(nullptr), m_MouseStrings(nullptr)
{
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	//char modelFilename[128];
	//char textureFilename[128];

	//char spriteFilename[128];
	char mouseString1[32], mouseString2[32], mouseString3[32];
	char testString1[256], testString2[256], testString3[256];
	char fpsString[32];
	bool result;

	// Create and initialize the Direct3D object.
	m_Direct3D = new D3DClass;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();

	// Create and initialize the font shader object.
	m_FontShader = new FontShaderClass;

	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the font object.
	m_Font = new FontClass;

	result = m_Font->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), 0);
	if (!result)
	{
		return false;
	}

	// Set the strings we want to display.
	strcpy_s(testString1, "X(<o_o>)X");
	strcpy_s(testString2, "Crab!");
	strcpy_s(testString3, "(HI HI HI!)");

	// Create and initialize the first text object.
	m_TextString1 = new TextClass;

	int strWidth = m_Font->GetSentencePixelLength(testString1);
	int strHeight = m_Font->GetFontHeight() / 2;;
	
	int widthCenterPos = (screenWidth / 2) - (strWidth / 2);
	int heightCenterPos = (screenHeight / 2) - (strHeight / 2);
	
	result = m_TextString1->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 256, m_Font, testString1, widthCenterPos, heightCenterPos, 1.0f, 0.2f, 0.0f);
	if (!result)
	{
		return false;
	}

	// Create and initialize the second text object.
	m_TextString2 = new TextClass;

	result = m_TextString2->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 256, m_Font, testString2, 300, 150, 0.0f, 0.5f, 1.0f);
	if (!result)
	{
		return false;
	}

	// Create and initialize the third text object.
	m_TextString3 = new TextClass;

	result = m_TextString3->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 256, m_Font, testString3, 300, 200, 0.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	// -------------------------------------------- FPS COUNTER ---------------------------------------------------
	// Create and initialize the fps object.
	m_Fps = new FpsClass();

	m_Fps->Initialize();

	// Set the initial fps and fps string.
	m_previousFps = -1;
	strcpy_s(fpsString, "Fps: 0");

	// Create and initialize the text object for the fps string.
	m_FpsString = new TextClass;

	result = m_FpsString->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, fpsString, 10, 10, 0.0f, 1.0f, 0.0f);
	if (!result)
	{
		return false;
	}

	//INPUT INFORMATION (MOUSE)
	// Set the initial mouse strings.
	strcpy_s(mouseString1, "Mouse X: 0");
	strcpy_s(mouseString2, "Mouse Y: 0");
	strcpy_s(mouseString3, "Mouse Button: No");

	// Create and initialize the text objects for the mouse strings.
	m_MouseStrings = new TextClass[3];

	result = m_MouseStrings[0].Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, mouseString1, 10, 10, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_MouseStrings[1].Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, mouseString1, 10, 35, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_MouseStrings[2].Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, mouseString1, 10, 60, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}


	// ---------------------------------------- 2D rendering with sprites --------------------------------------------
	//// Create and initialize the texture shader object.
	//m_TextureShader = new TextureShaderClass;

	//result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
	//	return false;
	//}

	//// Set the file name of the bitmap file.
	//strcpy_s(spriteFilename, "data/sprite_data_01.txt");
	////strcpy_s(bitmapFilename, "data/dwsample.tga");

	//// Create and initialize the bitmap object.
	//m_Sprite = new SpriteClass;

	//result = m_Sprite->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, spriteFilename, 50, 50);
	//if (!result)
	//{
	//	return false;
	//}

	//// Create and initialize the timer object.
	//m_Timer = new TimerClass;

	//result = m_Timer->Initialize();
	//if (!result)
	//{
	//	return false;
	//}


	// --------------------------------------------- 3D rendering with light ----------------------------------------------------
	//// Set the file name of the model.
	//strcpy_s(modelFilename, "data/sphere.txt"); //Имя файла модели

	//// Set the name of the texture file that we will be loading. //Имя файла текстуры
	//strcpy_s(textureFilename, "data/stone01.tga");

	//// Create and initialize the model object.
	////m_Model = new ModelClass;

	//result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
	//	return false;
	//}

	//// Create and initialize the light shader object.
	//m_LightShader = new LightShaderClass;

	//result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
	//	return false;
	//}

	//// Create and initialize the light object.
	//m_Light = new LightClass;

	//m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	//m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	//m_Light->SetDirection(1.0f, -1.0f, 1.0f);
	//m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	//m_Light->SetSpecularPower(32.0f);


	//// Set the number of lights we will use.
	//m_numLights = 5;

	//// Create and initialize the light objects array.
	//m_PointLights = new LightClass[m_numLights];

	//// Manually set the color and position of each light.
	//m_PointLights[0].SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);  // Red
	//m_PointLights[0].SetPosition(-2.0f, 2.0f, -1.0f);

	//m_PointLights[1].SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);  // Green
	//m_PointLights[1].SetPosition(2.0f, 2.0f, -1.0f);

	//m_PointLights[2].SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);  // Blue
	//m_PointLights[2].SetPosition(-2.0f, -2.0f, -1.0f);

	//m_PointLights[3].SetDiffuseColor(0.0f, 1.0f, 1.0f, 1.0f);  // White
	//m_PointLights[3].SetPosition(2.0f, -2.0f, -1.0f);

	//m_PointLights[4].SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);  // White
	//m_PointLights[4].SetPosition(0.0f, 1.0f, 0.0f);

	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the text objects for the mouse strings.
	if (m_MouseStrings)
	{
		m_MouseStrings[0].Shutdown();
		m_MouseStrings[1].Shutdown();
		m_MouseStrings[2].Shutdown();

		delete[] m_MouseStrings;
		m_MouseStrings = nullptr;
	}

	// Release the text object for the fps string.
	if (m_FpsString)
	{
		m_FpsString->Shutdown();
		delete m_FpsString;
		m_FpsString = nullptr;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = nullptr;
	}

	// Release the text string objects.
	if (m_TextString3)
	{
		m_TextString3->Shutdown();
		delete m_TextString3;
		m_TextString3 = nullptr;
	}
	
	if (m_TextString2)
	{
		m_TextString2->Shutdown();
		delete m_TextString2;
		m_TextString2 = nullptr;
	}

	if (m_TextString1)
	{
		m_TextString1->Shutdown();
		delete m_TextString1;
		m_TextString1 = nullptr;
	}

	// Release the font object.
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = nullptr;
	}

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = nullptr;
	}

	//// Release the timer object.
	//if (m_Timer)
	//{
	//	delete m_Timer;
	//	m_Timer = nullptr;
	//}

	//// Release the sprite object.
	//if (m_Sprite)
	//{
	//	m_Sprite->Shutdown();
	//	delete m_Sprite;
	//	m_Sprite = nullptr;
	//}

	//// Release the texture shader object.
	//if (m_TextureShader)
	//{
	//	m_TextureShader->Shutdown();
	//	delete m_TextureShader;
	//	m_TextureShader = nullptr;

	//}

	//// Release the light objects.
	//if (m_PointLights)
	//{
	//	delete[] m_PointLights;
	//	m_PointLights = nullptr;
	//}

	//// Release the light object.
	//if (m_Light)
	//{
	//	delete m_Light;
	//	m_Light = nullptr;
	//}

	//// Release the light shader object.
	//if (m_LightShader)
	//{
	//	m_LightShader->Shutdown();
	//	delete m_LightShader;
	//	m_LightShader = nullptr;
	//}

	//// Release the model object.
	//if (m_Model)
	//{
	//	m_Model->Shutdown();
	//	delete m_Model;
	//	m_Model = nullptr;
	//}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}

	return;
}


bool ApplicationClass::Frame(InputClass* Input)
{
	bool result;
	static float rotation = 0.0f;
	float frameTime;

	int mouseX, mouseY;
	bool mouseDown;

	// Check if the user pressed escape and wants to exit the application.
	if (Input->IsEscapePressed())
	{
		return false;
	}

	// Get the location of the mouse from the input object,
	Input->GetMouseLocation(mouseX, mouseY);

	// Check if the mouse has been pressed.
	mouseDown = Input->IsMousePressed();

	// Update the mouse strings each frame.
	result = UpdateMouseStrings(mouseX, mouseY, mouseDown);
	if (!result)
	{
		return false;
	}

	// Update the rotation variable each frame.
	/*rotation -= 0.0174532925f * 0.25f;
	if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}*/

	// Update the system stats.
	//m_Timer->Frame();

	// Get the current frame time.
	//frameTime = m_Timer->GetTime();

	// Update the sprite object using the frame time.
	//m_Sprite->Update(frameTime);

	//static int x = 300;
	//static int y = 300;

	//m_Sprite->SetRenderLocation(x, y);

	//m_Sprite->ResizeBitMap(x, y);

	/*if (static_cast<unsigned int>(frameTime) % 2 == 0)
	{
		x += 1;
		y += 1;
	}*/

	/*x += 1;
	y += 1;*/

	/*if (x >= 500)
	{
		x = 0;
		y = 0;
	}*/

	//---------------------------------------------  FPS COUNTER -----------------------------
	// Update the frames per second each frame.
	result = UpdateFps();
	if (!result)
	{
		return false;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}


bool ApplicationClass::Render(float rotation)
{
	//XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	//XMMATRIX rotateMatrix, translateMatrix, scaleMatrix, srMatrix;
	//XMFLOAT4 pointLightDiffuseColor[5], pointLightPosition[5];
	int i;

	XMMATRIX worldMatrix, viewMatrix, orthoMatrix; //rotateMatrix;

	bool result;


	// Clear the buffers to begin the scene. //CLEAR BG COLOR
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	//m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	//rotation -= 0.0174532925f * 180.0f;
	//rotateMatrix = XMMatrixRotationZ(rotation);  // Build the rotation matrix.
	//worldMatrix = XMMatrixMultiply(worldMatrix, rotateMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	//Enable alpha blending for 2D text rendering.
	m_Direct3D->EnableAlphaBlending();

	// Render the first text string using the font shader.
	m_TextString1->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_TextString1->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		m_Font->GetTexture(), m_TextString1->GetPixelColor());
	if (!result)
	{
		return false;
	}

	// Render the second text string using the font shader.
	m_TextString2->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_TextString2->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		m_Font->GetTexture(), m_TextString2->GetPixelColor());
	if (!result)
	{
		return false;
	}

	// Render the third text string using the font shader.
	m_TextString3->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_TextString3->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		m_Font->GetTexture(), m_TextString3->GetPixelColor());
	if (!result)
	{
		return false;
	}

	// ------------------------------------------------------------ FPS COUNTER ------------------------------------------------------------
	// Render the fps text string using the font shader.
	m_FpsString->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_FpsString->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		m_Font->GetTexture(), m_FpsString->GetPixelColor());
	if (!result)
	{
		return false;
	}

	// Render the mouse text strings using the font shader.
	for (i = 0; i < 3; i++)
	{
		m_MouseStrings[i].Render(m_Direct3D->GetDeviceContext());

		result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_MouseStrings[i].GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
			m_Font->GetTexture(), m_MouseStrings[i].GetPixelColor());
		if (!result)
		{
			return false;
		}
	}


	//// Put the sprite vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//result = m_Sprite->Render(m_Direct3D->GetDeviceContext());
	//if (!result)
	//{
	//	return false;
	//}

	//// Render the sprite with the texture shader.
	//result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Sprite->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Sprite->GetTexture());
	//if (!result)
	//{
	//	return false;
	//}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	//Disable alpha blending now that 2D text rendering is complete.
	m_Direct3D->DisableAlphaBlending();

	// --------------------------------------------- 3D rendering with light ----------------------------------------------------
	//// Get the world, view, and projection matrices from the camera and d3d objects.
	//m_Direct3D->GetWorldMatrix(worldMatrix);
	//m_Camera->GetViewMatrix(viewMatrix);
	//m_Direct3D->GetProjectionMatrix(projectionMatrix);

	//// Get the light properties.
	////for (i = 0; i < m_numLights; i++)
	//for (i = 3; i < 4; i++)
	//{
	//	// Create the diffuse color array from the four light colors.
	//	pointLightDiffuseColor[i] = m_PointLights[i].GetDiffuseColor();

	//	// Create the light position array from the four light positions.
	//	pointLightPosition[i] = m_PointLights[i].GetPosition();
	//}

	////rotation = 0.0f;
	//scaleMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);  // Build the scaling matrix.
	//rotateMatrix = XMMatrixRotationY(rotation);  // Build the rotation matrix.
	//translateMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);  // Build the translation matrix.

	//// Multiply the scale, rotation, and translation matrices together to create the final world transformation matrix. //Трансформ матрицы
	//srMatrix = XMMatrixMultiply(scaleMatrix, rotateMatrix);
	//worldMatrix = XMMatrixMultiply(srMatrix, translateMatrix);

	//// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//m_Model->Render(m_Direct3D->GetDeviceContext());

	//// Render the model using the light shader.
	//result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
	//	m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
	//	pointLightDiffuseColor, pointLightPosition);
	//if (!result)
	//{
	//	return false;
	//}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

bool ApplicationClass::UpdateFps()
{
	int fps;
	char tempString[16], finalString[16];
	float red, green, blue;
	bool result;


	// Update the fps each frame.
	m_Fps->Frame();

	// Get the current fps.
	fps = m_Fps->GetFps();

	// Check if the fps from the previous frame was the same, if so don't need to update the text string.
	if (m_previousFps == fps)
	{
		return true;
	}

	// Store the fps for checking next frame.
	m_previousFps = fps;

	// Truncate the fps to below 100,000.
	if (fps > 99999)
	{
		fps = 99999;
	}

	// Convert the fps integer to string format.
	sprintf_s(tempString, "%d", fps);

	// Setup the fps string.
	strcpy_s(finalString, "Fps: ");
	strcat_s(finalString, tempString);

	// If fps is 60 or above set the fps color to green.
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 60 set the fps color to yellow.
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 30 set the fps color to red.
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	int strWidth = m_Font->GetSentencePixelLength(finalString);

	// Update the sentence vertex buffer with the new string information.
	result = m_FpsString->UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, 800 - strWidth - 10, 10, red, green, blue);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::UpdateMouseStrings(int mouseX, int mouseY, bool mouseDown)
{
	char tempString[16], finalString[32];
	bool result;


	// Convert the mouse X integer to string format.
	sprintf_s(tempString, "%d", mouseX);

	// Setup the mouse X string.
	strcpy_s(finalString, "Mouse X: ");
	strcat_s(finalString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = m_MouseStrings[0].UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, 10, 10, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	// Convert the mouse Y integer to string format.
	sprintf_s(tempString, "%d", mouseY);

	// Setup the mouse Y string.
	strcpy_s(finalString, "Mouse Y: ");
	strcat_s(finalString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = m_MouseStrings[1].UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, 10, 35, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	// Setup the mouse button string.
	if (mouseDown)
	{
		strcpy_s(finalString, "Mouse Button: Yes");
	}
	else
	{
		strcpy_s(finalString, "Mouse Button: No");
	}

	// Update the sentence vertex buffer with the new string information.
	result = m_MouseStrings[2].UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, 10, 60, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	return true;
}