#pragma once

#include "d3dx9.h"

class CMyAllocateHierarchy : public ID3DXAllocateHierarchy {
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
		CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials, CONST DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER* ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
};

STDMETHODIMP CMyAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame) {
	*ppNewFrame = new D3DXFRAME();
	if (*ppNewFrame == nullptr) {
		return E_OUTOFMEMORY;
	}
	ZeroMemory(*ppNewFrame, sizeof(D3DXFRAME));

	if (Name) {
		(*ppNewFrame)->Name = _strdup(Name);
		if ((*ppNewFrame)->Name == nullptr) {
			delete* ppNewFrame;
			return E_OUTOFMEMORY;
		}
	}
	else {
		(*ppNewFrame)->Name = nullptr;
	}

	(*ppNewFrame)->pFrameFirstChild = nullptr;
	(*ppNewFrame)->pFrameSibling = nullptr;
	(*ppNewFrame)->pMeshContainer = nullptr;

	return S_OK;
}

STDMETHODIMP CMyAllocateHierarchy::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER* ppNewMeshContainer) {
	if (pMeshData == nullptr || pMeshData->Type != D3DXMESHTYPE_MESH) {
		return E_FAIL;
	}

	*ppNewMeshContainer = new D3DXMESHCONTAINER();
	if (*ppNewMeshContainer == nullptr) {
		return E_OUTOFMEMORY;
	}
	ZeroMemory(*ppNewMeshContainer, sizeof(D3DXMESHCONTAINER));

	if (Name) {
		(*ppNewMeshContainer)->Name = _strdup(Name);
		if ((*ppNewMeshContainer)->Name == nullptr) {
			delete* ppNewMeshContainer;
			return E_OUTOFMEMORY;
		}
	}
	else {
		(*ppNewMeshContainer)->Name = nullptr;
	}

	(*ppNewMeshContainer)->MeshData.Type = D3DXMESHTYPE_MESH;
	(*ppNewMeshContainer)->MeshData.pMesh = pMeshData->pMesh;
	if (pMeshData->pMesh) {
		pMeshData->pMesh->AddRef();
	}

	(*ppNewMeshContainer)->NumMaterials = NumMaterials;
	(*ppNewMeshContainer)->pMaterials = new D3DXMATERIAL[NumMaterials];
	if ((*ppNewMeshContainer)->pMaterials == nullptr) {
		delete* ppNewMeshContainer;
		return E_OUTOFMEMORY;
	}
	memcpy((*ppNewMeshContainer)->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

	(*ppNewMeshContainer)->pAdjacency = new DWORD[pMeshData->pMesh->GetNumFaces() * 3];
	if ((*ppNewMeshContainer)->pAdjacency == nullptr) {
		delete[](*ppNewMeshContainer)->pMaterials;
		delete* ppNewMeshContainer;
		return E_OUTOFMEMORY;
	}
	memcpy((*ppNewMeshContainer)->pAdjacency, pAdjacency, sizeof(DWORD) * pMeshData->pMesh->GetNumFaces() * 3);

	(*ppNewMeshContainer)->pSkinInfo = pSkinInfo;
	if (pSkinInfo) {
		pSkinInfo->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CMyAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree) {
	if (pFrameToFree) {
		if (pFrameToFree->Name) {
			free(pFrameToFree->Name);
		}
		delete pFrameToFree;
	}
	return S_OK;
}

STDMETHODIMP CMyAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase) {
	if (pMeshContainerBase) {
		if (pMeshContainerBase->Name) {
			free(pMeshContainerBase->Name);
		}
		if (pMeshContainerBase->MeshData.pMesh) {
			pMeshContainerBase->MeshData.pMesh->Release();
		}
		if (pMeshContainerBase->pMaterials) {
			delete[] pMeshContainerBase->pMaterials;
		}
		if (pMeshContainerBase->pAdjacency) {
			delete[] pMeshContainerBase->pAdjacency;
		}
		if (pMeshContainerBase->pSkinInfo) {
			pMeshContainerBase->pSkinInfo->Release();
		}
		delete pMeshContainerBase;
	}
	return S_OK;
}
