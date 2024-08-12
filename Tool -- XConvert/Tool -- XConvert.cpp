// Tool -- XConvert.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Alloc.h"
#include <string>
#include <iostream>


// console.exe /x/xt <input> <output>
// <Options>
// /x = Binary
// /xt = Text
// <input> = input file
// <output> = output file
int main(int argc, char* argv[])
{
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <option:/xt /x> <input> <output>" << std::endl;
        return -1;
    }

    std::string option = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    if (option != "/xt" && option != "/x") {
        std::cerr << "Invalid option. Use /xt for text format or /x for binary format." << std::endl;
        return -1;
    }

    LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pD3D) {
        std::cerr << "Failed to create Direct3D object." << std::endl;
        return -1;
    }

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = GetConsoleWindow();

    LPDIRECT3DDEVICE9 pd3dDevice;
    HRESULT hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetConsoleWindow(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice);
    if (FAILED(hr)) {
        std::cerr << "Failed to create Direct3D device: " << hr << std::endl;
        pD3D->Release();
        return -1;
    }

	LPD3DXFRAME m_pFrameRoot = NULL;
	LPD3DXANIMATIONCONTROLLER m_pAnimController = NULL;
	CMyAllocateHierarchy Alloc;
    DWORD dwFormat = (option == "/xt") ? D3DXF_FILEFORMAT_TEXT : D3DXF_FILEFORMAT_BINARY;

    hr = D3DXLoadMeshHierarchyFromXA(inputFile.c_str(), D3DXMESH_MANAGED, pd3dDevice,
        &Alloc, NULL, &m_pFrameRoot, &m_pAnimController);
    if (FAILED(hr)) {
        std::cerr << "Failed to load mesh hierarchy: " << hr << std::endl;
        pd3dDevice->Release();
        pD3D->Release();
        return -1;
    }

    hr = D3DXSaveMeshHierarchyToFileA(outputFile.c_str(), dwFormat, m_pFrameRoot, m_pAnimController, NULL);
    if (FAILED(hr)) {
        std::cerr << "Failed to save mesh hierarchy: " << hr << std::endl;
        if (m_pFrameRoot) {
            D3DXFrameDestroy(m_pFrameRoot, &Alloc);
            m_pFrameRoot = NULL;
        }
        if (m_pAnimController) {
            m_pAnimController->Release();
            m_pAnimController = NULL;
        }
        pd3dDevice->Release();
        pD3D->Release();
        return -1;
    }

    // Cleanup
    if (m_pFrameRoot) {
        D3DXFrameDestroy(m_pFrameRoot, &Alloc);
        m_pFrameRoot = NULL;
    }

    if (m_pAnimController) {
        m_pAnimController->Release();
        m_pAnimController = NULL;
    }

    pd3dDevice->Release();
    pD3D->Release();

    std::cout << "Mesh hierarchy saved successfully." << std::endl;
}