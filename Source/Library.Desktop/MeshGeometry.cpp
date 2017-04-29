#include "pch.h"
#include "MeshGeometry.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include "RendererDirectX.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(MeshGeometry)

	MeshGeometry::MeshGeometry() :
		mVertexBuffer(nullptr),
		mFaces(0U)
	{
	
	}

	MeshGeometry::~MeshGeometry()
	{
		if (mVertexBuffer != nullptr)
		{
			mVertexBuffer->Release();
			mVertexBuffer = nullptr;
		}
	}

	void MeshGeometry::Load(char* data, std::uint32_t size)
	{
		size;

		if (mVertexBuffer != nullptr)
		{
			mVertexBuffer->Release();
			mVertexBuffer = nullptr;
			mFaces = 0U;
		}

		if (data != nullptr)
		{
			mVertexBuffer = LoadOBJ(data, nullptr, mFaces, RendererDirectX::Get()->Device());

			if (mVertexBuffer == nullptr)
			{
				//MessageBox(nullptr, L"Load mesh from OBJ", L"Error", MB_OK);
			}
		}
	}

	std::uint32_t MeshGeometry::GetFaces() const
	{
		return mFaces;
	}

	ID3D11Buffer* MeshGeometry::GetVertexBuffer()
	{
		return mVertexBuffer;
	}

	const ID3D11Buffer* MeshGeometry::GetVertexBuffer() const
	{
		return mVertexBuffer;
	}

	void MeshGeometry::SetRenderingState(Renderer* renderer)
	{
		static const UINT stride = sizeof(MeshVertex);
		static const UINT offset = 0U;

		RendererDirectX* directX = reinterpret_cast<RendererDirectX*>(renderer);

		directX->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		directX->Context()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	}

	void MeshGeometry::GetAttributeCounts(char* pData,
										  int& nNumVerts,
										  int& nNumUVs,
										  int& nNumNormals,
										  int& nNumFaces)
	{
		char* pNewLine = 0;
		char* pStr = 0;

		nNumVerts   = 0;
		nNumFaces   = 0;
		nNumNormals = 0;
		nNumUVs     = 0;

		pStr = pData;

		while (1)
		{
			// Find the next newline
			pNewLine  = strchr(pStr, '\n');

			// If a null pointer was returned, that means
			// a null terminator was hit.
			if (pNewLine == 0)
			{
				break;
			}

			// Parse the beginning of each line to count
			// the total number of vertices/UVs/normals/faces.
			if (pStr[0] == 'v' &&
				pStr[1] == ' ')
			{
				nNumVerts++;
			}
			else if (pStr[0] == 'v' &&
					 pStr[1] == 't')
			{
				nNumUVs++;
			}
			else if (pStr[0] == 'v' &&
					 pStr[1] == 'n')
			{
				nNumNormals++;
			}
			else if (pStr[0] == 'f')
			{
				nNumFaces++;
			}

			// Now move the pointer to the next line.
			pStr = pNewLine + 1;
		}
	}

	void MeshGeometry::GenerateVertexBuffer(int    nNumFaces,
											float* pVertices,
											float* pUVs,
											float* pNormals,
											int*   pFaces,
											float* pVB)
	{
		if (pVB != 0)
		{
			//Add vertex data for each face	
			for(int i = 0; i < nNumFaces; i++)
			{
				//Vertex 1
				pVB[i*24 + 0]  = pVertices[(pFaces[i*9 + 0]-1)*3 + 0];
				pVB[i*24 + 1]  = pVertices[(pFaces[i*9 + 0]-1)*3 + 1];
				pVB[i*24 + 2]  = pVertices[(pFaces[i*9 + 0]-1)*3 + 2];
				pVB[i*24 + 3]  = pUVs[(pFaces[i*9 + 1]-1)*2 + 0];
				pVB[i*24 + 4]  = pUVs[(pFaces[i*9 + 1]-1)*2 + 1];
				pVB[i*24 + 5]  = pNormals[(pFaces[i*9 + 2]-1)*3 + 0];
				pVB[i*24 + 6]  = pNormals[(pFaces[i*9 + 2]-1)*3 + 1];
				pVB[i*24 + 7]  = pNormals[(pFaces[i*9 + 2]-1)*3 + 2];

				//Vertex 2
				pVB[i*24 + 8]  = pVertices[(pFaces[i*9 + 3]-1)*3 + 0];
				pVB[i*24 + 9]  = pVertices[(pFaces[i*9 + 3]-1)*3 + 1];
				pVB[i*24 + 10] = pVertices[(pFaces[i*9 + 3]-1)*3 + 2];
				pVB[i*24 + 11] = pUVs[(pFaces[i*9 + 4]-1)*2 + 0];
				pVB[i*24 + 12] = pUVs[(pFaces[i*9 + 4]-1)*2 + 1];
				pVB[i*24 + 13] = pNormals[(pFaces[i*9 + 5]-1)*3 + 0];
				pVB[i*24 + 14] = pNormals[(pFaces[i*9 + 5]-1)*3 + 1];
				pVB[i*24 + 15] = pNormals[(pFaces[i*9 + 5]-1)*3 + 2];

				//Vertex 3
				pVB[i*24 + 16] = pVertices[(pFaces[i*9 + 6]-1)*3 + 0];
				pVB[i*24 + 17] = pVertices[(pFaces[i*9 + 6]-1)*3 + 1];
				pVB[i*24 + 18] = pVertices[(pFaces[i*9 + 6]-1)*3 + 2];
				pVB[i*24 + 19] = pUVs[(pFaces[i*9 + 7]-1)*2 + 0];
				pVB[i*24 + 20] = pUVs[(pFaces[i*9 + 7]-1)*2 + 1];
				pVB[i*24 + 21] = pNormals[(pFaces[i*9 + 8]-1)*3 + 0];
				pVB[i*24 + 22] = pNormals[(pFaces[i*9 + 8]-1)*3 + 1];
				pVB[i*24 + 23] = pNormals[(pFaces[i*9 + 8]-1)*3 + 2];
			}
		}
	}

	ID3D11Buffer* MeshGeometry::LoadOBJ(char* data,
		float** pVertexArray,
		unsigned int& nFaces,
		ID3D11Device* pDevice)
	{
		int i = 0;
		char* pStr = 0;
		char* pNewLine = 0;
		int v = 0;
		int n = 0;
		int t = 0;
		int f = 0;
		float* pVertexBuffer = 0;

		int nNumVerts = 0;
		int nNumFaces = 0;
		int nNumNormals = 0;
		int nNumUVs = 0;

		float* pVertices = 0;
		float* pNormals = 0;
		float* pUVs = 0;
		int*   pFaces = 0;

		// Retrieve the counts for positions/UVs/normals/faces
		GetAttributeCounts(data,
			nNumVerts,
			nNumUVs,
			nNumNormals,
			nNumFaces);

		// Set the number of faces output
		nFaces = nNumFaces;

		// Allocate arrays to store vertex data
		pVertices = new float[nNumVerts * 3];
		pNormals = new float[nNumNormals * 3];
		pUVs = new float[nNumUVs * 2];
		pFaces = new   int[nNumFaces * 9];

		// Set string pointer to beginning of file buffer
		pStr = data;

		// Loop until end of file, reading line by line.
		while (1)
		{
			pNewLine = strchr(pStr, '\n');

			if (pNewLine == 0)
			{
				break;
			}

			if (pStr[0] == 'v' &&
				pStr[1] == ' ')
			{
				// Extract X/Y/Z coordinates
				pStr = &pStr[2];
				pStr = strtok(pStr, " ");
				pVertices[v * 3] = (float)atof(pStr);
				pStr = strtok(0, " ");
				pVertices[v * 3 + 1] = (float)atof(pStr);
				pStr = strtok(0, " \n\r");
				pVertices[v * 3 + 2] = (float)atof(pStr);

				// Increase the number of vertices
				v++;
			}
			else if (pStr[0] == 'v' &&
					 pStr[1] == 't')
			{
				// Extract U/V coordinates
				pStr = &pStr[3];
				pStr = strtok(pStr, " ");
				pUVs[t * 2] = (float)atof(pStr);
				pStr = strtok(0, " \n\r");
				pUVs[t * 2 + 1] = (float)atof(pStr);

				// Increase number of texcoords
				t++;
			}
			else if (pStr[0] == 'v' &&
					 pStr[1] == 'n')
			{
				pStr = &pStr[3];
				pStr = strtok(pStr, " ");
				pNormals[n * 3] = (float)atof(pStr);
				pStr = strtok(0, " ");
				pNormals[n * 3 + 1] = (float)atof(pStr);
				pStr = strtok(0, " \n\r");
				pNormals[n * 3 + 2] = (float)atof(pStr);

				// Increase number of normals
				n++;
			}
			else if (pStr[0] == 'f')
			{
				pStr = &pStr[2];

				// Parse and convert all 9 indices.
				for (i = 0; i < 9; i++)
				{
					if (i == 0)
					{
						pStr = strtok(pStr, " /\n\r");
					}
					else
					{
						pStr = strtok(0, " /\n\r");
					}
					pFaces[f * 9 + i] = atoi(pStr);
				}

				// Increase the face count
				f++;
			}

			// Now move the pointer to the next line.
			pStr = pNewLine + 1;
		}

		// From the separate arrays, create one big buffer
		// that has the vertex data interleaved.
		pVertexBuffer = new float[nNumFaces * 24];
		GenerateVertexBuffer(nNumFaces,
			pVertices,
			pUVs,
			pNormals,
			pFaces,
			pVertexBuffer);

		// Free arrays made to hold data.
		delete[] pVertices;
		pVertices = 0;
		delete[] pNormals;
		pNormals = 0;
		delete[] pUVs;
		pUVs = 0;
		delete[] pFaces;
		pFaces = 0;

		// Create the D3D buffer object
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(MeshVertex) * nFaces * 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = pVertexBuffer;

		ID3D11Buffer* pD3DBuffer = 0;
		HRESULT hr = pDevice->CreateBuffer(&bd, &InitData, &pD3DBuffer);
		if (FAILED(hr))
		{
			return nullptr;
		}

		// Delete the client-side buffer, since it is no longer needed.
		if (pVertexArray != 0)
		{
			*pVertexArray = pVertexBuffer;
		}
		else
		{
			delete[] pVertexBuffer;
			pVertexBuffer = 0;
		}

		return pD3DBuffer;
	}
}