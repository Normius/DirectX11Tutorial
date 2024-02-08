////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textureclass.h"

TextureClass::TextureClass()
	:m_targaData(nullptr), m_texture(nullptr), m_textureView(nullptr), m_width(0), m_height(0)
{
}

TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;
	//int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	// Load the targa image data into memory.
	result = LoadTarga24or32Bit(filename);
	if (!result)
	{
		return false;
	}

	// Setup the description of the texture.
	textureDesc.Height = m_height;
	textureDesc.Width = m_width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hResult))
	{
		return false;
	}

	// Set the row pitch of the targa image data.
	rowPitch = (m_width * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture.
	deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

	// Setup the shader resource view description.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if (FAILED(hResult))
	{
		return false;
	}

	// Generate mipmaps for this texture.
	deviceContext->GenerateMips(m_textureView);

	// Release the targa image data now that the image data has been loaded into the texture.
	delete[] m_targaData;
	m_targaData = nullptr;

	return true;
}

void TextureClass::Shutdown()
{
	// Release the texture view resource.
	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = nullptr;
	}

	// Release the texture.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}

	// Release the targa data.
	if (m_targaData)
	{
		delete[] m_targaData;
		m_targaData = nullptr;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView;
}

bool TextureClass::LoadTarga24or32Bit(char* filename)
{
	int error, bpp, targaSize, textureSize, targaBitsPerPixel, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;


	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Get the important information from the header.
	m_height = (int)targaFileHeader.height;
	m_width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if (bpp == 32)
	{
		//32-bit targa image (4 bit per pixel) with aplha-channel
		targaBitsPerPixel = 4;
	}
	else if (bpp == 24)
	{
		//24-bit targa image (3 bit per pixel) without aplha-channel
		targaBitsPerPixel = 3;
	}
	else
	{
		return false;
	}

	// Calculate the size of the targa image (24-bit or 32-bit targa)
	targaSize = m_width * m_height * targaBitsPerPixel;

	// Allocate memory for the targa image data.
	targaImage = new unsigned char[targaSize];

	// Read in the targa image data.
	count = (unsigned int)fread(targaImage, 1, targaSize, filePtr);
	if (count != targaSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Calculate the size of the targa destination data (4 bit per pixel for both 24-bit and 32-bit targa)
	textureSize = targaSize = m_width * m_height * 4;

	// Allocate memory for the targa destination data.
	m_targaData = new unsigned char[textureSize];

	// Initialize the index into the targa destination data array.
	index = 0;

	// Initialize the index into the targa image data (start from last row because targa is upside down)
	k = (m_width * m_height * targaBitsPerPixel) - (m_width * targaBitsPerPixel);

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down and also is not in RGBA order.
	for (j = 0; j < m_height; ++j)
	{
		for (i = 0; i < m_width; ++i)
		{
			m_targaData[index + 0] = targaImage[k + 2];  // Red.
			m_targaData[index + 1] = targaImage[k + 1];  // Green.
			m_targaData[index + 2] = targaImage[k + 0];  // Blue
			if (bpp == 32)
			{
				m_targaData[index + 3] = targaImage[k + 3]; // Alpha
				
			}
			else
			{
				m_targaData[index + 3] = 255;  // Alpha
			}

			// Increment the indexes into the targa data.
			k += targaBitsPerPixel;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (m_width * targaBitsPerPixel * 2);
	}
	
	// Release the targa image data now that it was copied into the destination array.
	delete[] targaImage;
	targaImage = nullptr;

	return true;
}

int TextureClass::GetWidth()
{
	return m_width;
}

int TextureClass::GetHeight()
{
	return m_height;
}
