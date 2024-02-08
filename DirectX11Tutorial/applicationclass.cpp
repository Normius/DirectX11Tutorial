////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
	: m_Direct3D(nullptr), m_Camera(nullptr), m_Model(nullptr), 
	//m_LightShader(nullptr), m_Light(nullptr), m_PointLights(nullptr)
	m_AlphaMapShader(nullptr)
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
	char modelFilename[128];
	char textureFilename1[128];
	char textureFilename2[128];
	char textureFilename3[128];
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
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
	//m_Camera->Render();

	// Create and initialize the multitexture shader object.
	m_AlphaMapShader = new AlphaMapShaderClass;

	result = m_AlphaMapShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the multitexture shader object.", L"Error", MB_OK);
		return false;
	}

	// Set the file name of the model.
	strcpy_s(modelFilename, "data/square.txt"); //Имя файла модели

	// Set the file name of the textures.
	strcpy_s(textureFilename1, "data/stone01.tga");
	strcpy_s(textureFilename2, "data/dirt01.tga");
	strcpy_s(textureFilename3, "data/alpha01.tga");

	// Create and initialize the model object.
	m_Model = new ModelClass;

	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename1, textureFilename2, textureFilename3);
	if (!result)
	{
		return false;
	}

	// --------------------------------------- 3D LIGHT -----------------------------------------------
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
	// Release the multitexture shader object.
	if (m_AlphaMapShader)
	{
		m_AlphaMapShader->Shutdown();
		delete m_AlphaMapShader;
		m_AlphaMapShader = nullptr;
	}

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

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = nullptr;
	}

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


bool ApplicationClass::Frame()
{
	bool result;
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation -= 0.0174532925f * 0.25f;
	if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}
	//rotation = 0.0f;
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
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMMATRIX rotateMatrix, translateMatrix, scaleMatrix, srMatrix;
	bool result;
	//XMFLOAT4 pointLightDiffuseColor[5], pointLightPosition[5];
	//int i;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// --------------------------------- 3D light --------------------------------
	//// Get the light properties.
	////for (i = 0; i < m_numLights; i++)
	//for (i = 3; i < 4; i++)
	//{
	//	// Create the diffuse color array from the four light colors.
	//	pointLightDiffuseColor[i] = m_PointLights[i].GetDiffuseColor();

	//	// Create the light position array from the four light positions.
	//	pointLightPosition[i] = m_PointLights[i].GetPosition();
	//}

	rotation = 0.0f;
	scaleMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);  // Build the scaling matrix.
	rotateMatrix = XMMatrixRotationY(rotation);  // Build the rotation matrix.
	translateMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);  // Build the translation matrix.

	//Multiply the scale, rotation, and translation matrices together to create the final world transformation matrix. //Трансформ матрицы
	srMatrix = XMMatrixMultiply(scaleMatrix, rotateMatrix);
	worldMatrix = XMMatrixMultiply(srMatrix, translateMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// ------------------------------------------------ 3D LIGHT RENDER ----------------------------------------------------
	//// Render the model using the light shader.
	//result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
	//	m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
	//	pointLightDiffuseColor, pointLightPosition);
	//if (!result)
	//{
	//	return false;
	//}

	result = m_AlphaMapShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(0), m_Model->GetTexture(1), m_Model->GetTexture(2));
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}