#include "Alloc.h"

std::vector<std::string> GetFilesInFolder(const std::string& folder)
{
    std::vector<std::string> files;

    std::string searchPath = folder + "\\*.*";

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE)
        return files;

    do
    {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        files.push_back(folder + "\\" + findData.cFileName);

    } while (FindNextFileA(hFind, &findData));

    FindClose(hFind);

    return files;
}

int main(int argc, char* argv[])
{
    LPDIRECT3DDEVICEQ pd3dDevice = NULL;

    if (!CreateD3DDevice(pd3dDevice))
    {
        std::cout << "Failed to create D3D device." << std::endl;
        return FALSE;
    }

    std::string effectFolder = "F:\\EP6_Source\\[ Tool ]\\data\\effect";

    DxEffSinglePropGMan::GetInstance().OneTimeSceneInit(effectFolder.c_str());
    DxEffSinglePropGMan::GetInstance().InitDeviceObjects(pd3dDevice);

    std::vector<std::string> files = GetFilesInFolder(effectFolder);

    std::ofstream failedLog("failed_effects.txt");

    int okCount = 0;
    int failedCount = 0;

    for (size_t i = 0; i < files.size(); ++i)
    {
        const std::string& strFILE = files[i];

        std::cout << "Testing: " << strFILE << std::endl;

        EFF_PROPGROUP* pPropGroup =
            DxEffSinglePropGMan::GetInstance().LoadEffectGProp(strFILE.c_str());

        if (!pPropGroup)
        {
            ++failedCount;

            std::cout << "FAILED LoadEffectGProp: " << strFILE << std::endl;
            failedLog << "FAILED LoadEffectGProp: " << strFILE << std::endl;

            continue;
        }

        DxEffSingleGroup* m_pEffSingle = pPropGroup->NEWEFFGROUP();

        if (!m_pEffSingle)
        {
            ++failedCount;

            std::cout << "FAILED NEWEFFGROUP: " << strFILE << std::endl;
            failedLog << "FAILED NEWEFFGROUP: " << strFILE << std::endl;

            continue;
        }

        SAFE_DELETE(m_pEffSingle);

        ++okCount;

        std::cout << "OK: " << strFILE << std::endl;
    }

    SAFE_RELEASE(pd3dDevice);

    std::cout << "Done." << std::endl;
    std::cout << "OK: " << okCount << std::endl;
    std::cout << "Failed: " << failedCount << std::endl;

    return TRUE;
}

bool CreateD3DDevice(LPDIRECT3DDEVICEQ& pd3dDevice)
{
    HWND hWnd = GetConsoleWindow();

    LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pD3D)
        return false;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

    HRESULT hr = pD3D->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &pd3dDevice
    );

    if (FAILED(hr))
    {
        hr = pD3D->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &d3dpp,
            &pd3dDevice
        );
    }

    SAFE_RELEASE(pD3D);

    return SUCCEEDED(hr);
}