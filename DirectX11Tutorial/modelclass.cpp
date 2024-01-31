////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"
#include <string>

ModelClass::ModelClass()
	: m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_Texture(nullptr), 
	  m_vertexCount(0), m_indexCount(0), m_model(nullptr)
{
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFilename)
{
	bool result;

	// Load in the model data.
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

//В этой функции обрабатываем создания буферов вершин и индексов. Здесь можно считать модель (3D, 2D) объекта, но для примера у нас вручную вбит треугольник
bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	//Устанавливаем для треугольника вручную (количество вершин и индексов)
	// Set the number of vertices in the vertex array.
	//m_vertexCount = 3;
	//m_vertexCount = 6;

	// Set the number of indices in the index array.
	//m_indexCount = 3;
	//m_indexCount = 6;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Загрузка 3D модели
	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	//Устанавливаем данные для треугольника вручную (координаты вершин и текстура)
	// Load the vertex array with data.
	//vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle 
	//vertices[1].texture = XMFLOAT2(0.5f, 0.0f);
	//vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//vertices[2].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//// Load the index array with data.
	//indices[0] = 0;  // Bottom left.
	//indices[1] = 1;  // Top middle.
	//indices[2] = 2;  // Bottom right.

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;


	// Create and initialize the texture object.
	m_Texture = new TextureClass;

	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

void ModelClass::LoadStandartModel(char* filename, ifstream& fin)
{
	char input;

	fin.clear();
	fin.seekg(0);

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (int i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}
}

void ModelClass::LoadtBlenderObjModel(char* filename, ifstream& fin)
{
	char input;

	fin.clear();
	fin.seekg(0);

	constexpr int ignoreBufferSize = 255;
	char ignoreBuffer[ignoreBufferSize];

	int vertexIndexCount = 0;
	int normalVectorsIndexCount = 0;
	int textureVectorsIndexCount = 0;
	int triangleCount = 0;

	while (fin.get(input))
	{
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ')
			{
				++vertexIndexCount;
			}
			else if (input == 'n')
			{
				++normalVectorsIndexCount;
			}
			else if (input == 't')
			{
				++textureVectorsIndexCount;
			}
		}
		else if (input == 'f')
		{
			++triangleCount;
		}
		else
		{
			fin.getline(ignoreBuffer, ignoreBufferSize);
		}
	}

	fin.clear();
	fin.seekg(0);

	XMFLOAT3* vertexCoord = new XMFLOAT3[vertexIndexCount + 1];
	XMFLOAT3* normalVectorsCoord = new XMFLOAT3[normalVectorsIndexCount + 1];
	XMFLOAT2* textureCoord = new XMFLOAT2[textureVectorsIndexCount + 1];

	m_vertexCount = triangleCount * 3;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];

	while (fin.get(input))
	{
		if (input == 'v')
		{
			for (int i_vertex = 1; i_vertex < vertexIndexCount + 1; ++i_vertex)
			{
				fin >> vertexCoord[i_vertex].x >> vertexCoord[i_vertex].y >> vertexCoord[i_vertex].z;
				fin.get(input);
				fin.get(input);
			}

			fin.get(input);

			for (int i_normal = 1; i_normal < normalVectorsIndexCount + 1; ++i_normal)
			{
				fin >> normalVectorsCoord[i_normal].x >> normalVectorsCoord[i_normal].y >> normalVectorsCoord[i_normal].z;
				fin.get(input);
				fin.get(input);
				fin.get(input);
			}

			fin.get(input);
			for (int i_texture = 1; i_texture < textureVectorsIndexCount + 1; ++i_texture)
			{
				fin >> textureCoord[i_texture].x >> textureCoord[i_texture].y;
				fin.get(input);
				fin.get(input);
				fin.get(input);
			}
		}
		else if (input == 'f')
		{
			int i_vertex = 0;
			int i_texture = 0;
			int i_normal = 0;
			int i = 0;
			for (int i_triangle = 1; i_triangle < triangleCount + 1; ++i_triangle)
			{
				for (int i_triangleVertex = 1; i_triangleVertex <= 3; ++i_triangleVertex)
				{
					fin >> i_vertex;
					fin.get(input);
					fin >> i_texture;
					fin.get(input);
					fin >> i_normal;
					fin.get(input);

					m_model[i].x = vertexCoord[i_vertex].x;
					m_model[i].y = vertexCoord[i_vertex].y;
					m_model[i].z = vertexCoord[i_vertex].z;
					m_model[i].tu = textureCoord[i_texture].x;
					m_model[i].tv = textureCoord[i_texture].y;
					m_model[i].nx = normalVectorsCoord[i_normal].x;
					m_model[i].ny = normalVectorsCoord[i_normal].y;
					m_model[i].nz = normalVectorsCoord[i_normal].z;

					++i;
				}
				fin.get(input);
			}
		}
		else
		{
			fin.getline(ignoreBuffer, ignoreBufferSize);
		}
	}
	delete[] vertexCoord;
	delete[] normalVectorsCoord;
	delete[] textureCoord;
}

bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	//char input;
	//int i;

	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	char firstFileSymbol;
	fin.get(firstFileSymbol);
	if (firstFileSymbol == 'V')
	{
		LoadStandartModel(filename, fin);
	}
	else if (firstFileSymbol == '#')
	{
		LoadtBlenderObjModel(filename, fin);
	}
	else
	{
		return false;
	}

	//// Read up to the value of vertex count.
	//fin.get(input);
	//while (input != ':')
	//{
	//	fin.get(input);
	//}

	//// Read in the vertex count.
	//fin >> m_vertexCount;

	//// Set the number of indices to be the same as the vertex count.
	//m_indexCount = m_vertexCount;

	//// Create the model using the vertex count that was read in.
	//m_model = new ModelType[m_vertexCount];

	//// Read up to the beginning of the data.
	//fin.get(input);
	//while (input != ':')
	//{
	//	fin.get(input);
	//}
	//fin.get(input);
	//fin.get(input);

	//// Read in the vertex data.
	//for (i = 0; i < m_vertexCount; i++)
	//{
	//	fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
	//	fin >> m_model[i].tu >> m_model[i].tv;
	//	fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	//}

	// Close the model file.
	fin.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = nullptr;
	}

	return;
}