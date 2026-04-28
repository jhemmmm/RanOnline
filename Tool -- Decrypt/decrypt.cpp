// decrypt.cpp
// Standalone batch decryption tool for EP6 encrypted binary files.
//
// Usage:
//   decrypt.exe <EMBYTECRYPT_TYPE> [/ext:<exts>] [input_dir] [output_dir]
//
// Options:
//   /ext:<exts>   Comma-separated list of extensions to process (e.g. /ext:.chf,.abf)
//                 If omitted, all files are processed.
//                 The flag may appear anywhere after the crypt type.
//
// Examples:
//   decrypt.exe EMBYTECRYPT_CONTAINER
//   decrypt.exe EMBYTECRYPT_CONTAINER /ext:.chf
//   decrypt.exe EMBYTECRYPT_CONTAINER C:\data\chars /ext:.chf,.abf
//   decrypt.exe EMBYTECRYPT_CONTAINER C:\data\chars C:\data\out /ext:.chf
//
// If output_dir is omitted, files are decrypted in-place (overwrite).
// If input_dir is omitted, the current working directory is used.
// Only files that are detected as encrypted are processed.
//
// File detection & bypass (two patterns depending on crypt type):
//
//  Pattern A — CONTAINER, PIECE, WLD, etc.  (bypass = 136)
//    [0-127]  type string | [128-131] FileID | [132-135] VERSION DWORD (plain)
//    Detected when DWORD at offset 132 >= 0x0200
//
//  Pattern B — CFG, BIN  (bypass = 132)
//    [0-127]  type string | [128-131] FileID = VERSION (plain)
//    SetEncodeType called immediately; encryption starts at byte 132
//    Detected when FileID DWORD at offset 128 >= 0x0200
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>

// -----------------------------------------------------------------------
// ByteCrypt types (mirrors EMBYTECRYPT enum from ByteCrypt.h)
// -----------------------------------------------------------------------
enum EMBYTECRYPT
{
    EMBYTECRYPT_NONE        = 0,
    EMBYTECRYPT_OLD         = 1,
    EMBYTECRYPT_RAN2        = 2,
    EMBYTECRYPT_LEVEL       = 3,
    EMBYTECRYPT_WLD         = 4,
    EMBYTECRYPT_CROWSALE    = 5,
    EMBYTECRYPT_NPCTALK     = 6,
    EMBYTECRYPT_QUEST       = 7,
    EMBYTECRYPT_ITEM        = 8,
    EMBYTECRYPT_CROW        = 9,
    EMBYTECRYPT_CLASSCONST  = 10,
    EMBYTECRYPT_PETCHARSET  = 11,
    EMBYTECRYPT_SKILL       = 12,
    EMBYTECRYPT_CHARSET     = 13,
    EMBYTECRYPT_CONTAINER   = 14,
    EMBYTECRYPT_PIECE       = 15,
    EMBYTECRYPT_ITEMIX      = 16,
    EMBYTECRYPT_CFG         = 17,
    EMBYTECRYPT_EGP         = 18,
    EMBYTECRYPT_BIN         = 19,
    EMBYTECRYPT_TAXISTATION = 20,
    EMBYTECRYPT_MAPSLIST    = 21,

    EMBYTECRYPT_LEVEL2          = 22,
    EMBYTECRYPT_WLD2            = 23,
    EMBYTECRYPT_CROWSALE2       = 24,
    EMBYTECRYPT_NPCTALK2        = 25,
    EMBYTECRYPT_QUEST2          = 26,
    EMBYTECRYPT_ITEM2           = 27,
    EMBYTECRYPT_CROW2           = 28,
    EMBYTECRYPT_CLASSCONST2     = 29,
    EMBYTECRYPT_PETCHARSET2     = 30,
    EMBYTECRYPT_SKILL2          = 31,
    EMBYTECRYPT_CHARSET2        = 32,
    EMBYTECRYPT_CONTAINER2      = 33,
    EMBYTECRYPT_PIECE2          = 34,
    EMBYTECRYPT_ITEMIX2         = 35,
    EMBYTECRYPT_CFG2            = 36,
    EMBYTECRYPT_EGP2            = 37,
    EMBYTECRYPT_BIN2            = 38,
    EMBYTECRYPT_TAXISTATION2    = 39,
    EMBYTECRYPT_MAPSLIST2       = 40,
    EMBYTECRYPT_ACTIVITY        = 41,

    EMBYTECRYPT_SIZE            = 42,
};

// -----------------------------------------------------------------------
// ByteCrypt tables (sourced from ByteCryptDef*.h)
// -----------------------------------------------------------------------
#include "../Lib -- Engine/Common/ByteCryptDef.h"
#include "../Lib -- Engine/Common/ByteCryptDefVer1.h"
#include "../Lib -- Engine/Common/ByteCryptDefVer2.h"

// -----------------------------------------------------------------------
// ARRAY_USE lookup table (built once at first use)
// -----------------------------------------------------------------------
static UCHAR ARRAY_USE[EMBYTECRYPT_SIZE][256];
static bool  s_bArrayInit = false;

static void InitArrays()
{
    if (s_bArrayInit) return;

    for (int i = 0; i < 256; ++i)
    {
        ARRAY_USE[EMBYTECRYPT_NONE][i]          = ARRAY_NONE[i];
        ARRAY_USE[EMBYTECRYPT_OLD][i]           = ARRAY_OLD[i];
        ARRAY_USE[EMBYTECRYPT_RAN2][i]          = ARRAY_RAN2[i];

        ARRAY_USE[EMBYTECRYPT_LEVEL][i]         = ARRAY_LEVEL_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_WLD][i]           = ARRAY_WLD_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_CROWSALE][i]      = ARRAY_CROWSALE_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_NPCTALK][i]       = ARRAY_NPCTALK_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_QUEST][i]         = ARRAY_QUEST_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_ITEM][i]          = ARRAY_ITEM_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_CROW][i]          = ARRAY_CROW_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_CLASSCONST][i]    = ARRAY_CLASSCONST_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_PETCHARSET][i]    = ARRAY_PETCHARSET_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_SKILL][i]         = ARRAY_SKILL_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_CHARSET][i]       = ARRAY_CHARSET_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_CONTAINER][i]     = ARRAY_CONTAINER_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_PIECE][i]         = ARRAY_PIECE_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_ITEMIX][i]        = ARRAY_ITEMIX_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_CFG][i]           = ARRAY_CFG_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_EGP][i]           = ARRAY_EGP_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_BIN][i]           = ARRAY_BIN_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_TAXISTATION][i]   = ARRAY_TAXISTATION_VAR1[i];
        ARRAY_USE[EMBYTECRYPT_MAPSLIST][i]      = ARRAY_MAPSLIST_VAR1[i];

        ARRAY_USE[EMBYTECRYPT_LEVEL2][i]        = ARRAY_LEVEL_VER2[i];
        ARRAY_USE[EMBYTECRYPT_WLD2][i]          = ARRAY_WLD_VER2[i];
        ARRAY_USE[EMBYTECRYPT_CROWSALE2][i]     = ARRAY_CROWSALE_VER2[i];
        ARRAY_USE[EMBYTECRYPT_NPCTALK2][i]      = ARRAY_NPCTALK_VER2[i];
        ARRAY_USE[EMBYTECRYPT_QUEST2][i]        = ARRAY_QUEST_VER2[i];
        ARRAY_USE[EMBYTECRYPT_ITEM2][i]         = ARRAY_ITEM_VER2[i];
        ARRAY_USE[EMBYTECRYPT_CROW2][i]         = ARRAY_CROW_VER2[i];
        ARRAY_USE[EMBYTECRYPT_CLASSCONST2][i]   = ARRAY_CLASSCONST_VER2[i];
        ARRAY_USE[EMBYTECRYPT_PETCHARSET2][i]   = ARRAY_PETCHARSET_VER2[i];
        ARRAY_USE[EMBYTECRYPT_SKILL2][i]        = ARRAY_SKILL_VER2[i];
        ARRAY_USE[EMBYTECRYPT_CHARSET2][i]      = ARRAY_CHARSET_VER2[i];
        ARRAY_USE[EMBYTECRYPT_CONTAINER2][i]    = ARRAY_CONTAINER_VER2[i];
        ARRAY_USE[EMBYTECRYPT_PIECE2][i]        = ARRAY_PIECE_VER2[i];
        ARRAY_USE[EMBYTECRYPT_ITEMIX2][i]       = ARRAY_ITEMIX_VER2[i];
        ARRAY_USE[EMBYTECRYPT_CFG2][i]          = ARRAY_CFG_VER2[i];
        ARRAY_USE[EMBYTECRYPT_EGP2][i]          = ARRAY_EGP_VER2[i];
        ARRAY_USE[EMBYTECRYPT_BIN2][i]          = ARRAY_BIN_VER2[i];
        ARRAY_USE[EMBYTECRYPT_TAXISTATION2][i]  = ARRAY_TAXISTATION_VER2[i];
        ARRAY_USE[EMBYTECRYPT_MAPSLIST2][i]     = ARRAY_MAPSLIST_VER2[i];
        ARRAY_USE[EMBYTECRYPT_ACTIVITY][i]      = ARRAY_ACTIVITY[i];
    }

    s_bArrayInit = true;
}

// Decrypts buffer in-place using a reverse-lookup of the substitution table.
static void ByteDecode(BYTE* pbuffer, DWORD dwSize, EMBYTECRYPT emCRYPT)
{
    if (!pbuffer || dwSize == 0)               return;
    if (emCRYPT == EMBYTECRYPT_NONE)           return;
    if (emCRYPT >= EMBYTECRYPT_SIZE)           return;

    InitArrays();

    UCHAR DEARRAY[256];
    for (DWORD i = 0; i < 256; ++i)
        DEARRAY[ARRAY_USE[emCRYPT][i]] = (UCHAR)i;

    for (DWORD i = 0; i < dwSize; ++i)
        pbuffer[i] = DEARRAY[pbuffer[i]];
}

// -----------------------------------------------------------------------
// EMBYTECRYPT name -> enum map
// -----------------------------------------------------------------------
static EMBYTECRYPT ParseCryptType(const char* name)
{
    struct { const char* name; EMBYTECRYPT val; } table[] = {
        { "EMBYTECRYPT_NONE",        EMBYTECRYPT_NONE        },
        { "EMBYTECRYPT_OLD",         EMBYTECRYPT_OLD         },
        { "EMBYTECRYPT_RAN2",        EMBYTECRYPT_RAN2        },
        { "EMBYTECRYPT_LEVEL",       EMBYTECRYPT_LEVEL       },
        { "EMBYTECRYPT_WLD",         EMBYTECRYPT_WLD         },
        { "EMBYTECRYPT_CROWSALE",    EMBYTECRYPT_CROWSALE    },
        { "EMBYTECRYPT_NPCTALK",     EMBYTECRYPT_NPCTALK     },
        { "EMBYTECRYPT_QUEST",       EMBYTECRYPT_QUEST       },
        { "EMBYTECRYPT_ITEM",        EMBYTECRYPT_ITEM        },
        { "EMBYTECRYPT_CROW",        EMBYTECRYPT_CROW        },
        { "EMBYTECRYPT_CLASSCONST",  EMBYTECRYPT_CLASSCONST  },
        { "EMBYTECRYPT_PETCHARSET",  EMBYTECRYPT_PETCHARSET  },
        { "EMBYTECRYPT_SKILL",       EMBYTECRYPT_SKILL       },
        { "EMBYTECRYPT_CHARSET",     EMBYTECRYPT_CHARSET     },
        { "EMBYTECRYPT_CONTAINER",   EMBYTECRYPT_CONTAINER   },
        { "EMBYTECRYPT_PIECE",       EMBYTECRYPT_PIECE       },
        { "EMBYTECRYPT_ITEMIX",      EMBYTECRYPT_ITEMIX      },
        { "EMBYTECRYPT_CFG",         EMBYTECRYPT_CFG         },
        { "EMBYTECRYPT_EGP",         EMBYTECRYPT_EGP         },
        { "EMBYTECRYPT_BIN",         EMBYTECRYPT_BIN         },
        { "EMBYTECRYPT_TAXISTATION", EMBYTECRYPT_TAXISTATION },
        { "EMBYTECRYPT_MAPSLIST",    EMBYTECRYPT_MAPSLIST    },
        { "EMBYTECRYPT_LEVEL2",      EMBYTECRYPT_LEVEL2      },
        { "EMBYTECRYPT_WLD2",        EMBYTECRYPT_WLD2        },
        { "EMBYTECRYPT_CROWSALE2",   EMBYTECRYPT_CROWSALE2   },
        { "EMBYTECRYPT_NPCTALK2",    EMBYTECRYPT_NPCTALK2    },
        { "EMBYTECRYPT_QUEST2",      EMBYTECRYPT_QUEST2      },
        { "EMBYTECRYPT_ITEM2",       EMBYTECRYPT_ITEM2       },
        { "EMBYTECRYPT_CROW2",       EMBYTECRYPT_CROW2       },
        { "EMBYTECRYPT_CLASSCONST2", EMBYTECRYPT_CLASSCONST2 },
        { "EMBYTECRYPT_PETCHARSET2", EMBYTECRYPT_PETCHARSET2 },
        { "EMBYTECRYPT_SKILL2",      EMBYTECRYPT_SKILL2      },
        { "EMBYTECRYPT_CHARSET2",    EMBYTECRYPT_CHARSET2    },
        { "EMBYTECRYPT_CONTAINER2",  EMBYTECRYPT_CONTAINER2  },
        { "EMBYTECRYPT_PIECE2",      EMBYTECRYPT_PIECE2      },
        { "EMBYTECRYPT_ITEMIX2",     EMBYTECRYPT_ITEMIX2     },
        { "EMBYTECRYPT_CFG2",        EMBYTECRYPT_CFG2        },
        { "EMBYTECRYPT_EGP2",        EMBYTECRYPT_EGP2        },
        { "EMBYTECRYPT_BIN2",        EMBYTECRYPT_BIN2        },
        { "EMBYTECRYPT_TAXISTATION2",EMBYTECRYPT_TAXISTATION2},
        { "EMBYTECRYPT_MAPSLIST2",   EMBYTECRYPT_MAPSLIST2   },
        { "EMBYTECRYPT_ACTIVITY",    EMBYTECRYPT_ACTIVITY    },
    };

    for (int i = 0; i < (int)(sizeof(table)/sizeof(table[0])); ++i)
    {
        if (_stricmp(table[i].name, name) == 0)
            return table[i].val;
    }
    return (EMBYTECRYPT)-1;
}

// -----------------------------------------------------------------------
// File header constants (matching CSerialFile::WriteFileType / ReadFileType)
//
// CSerialFile::WriteFileType always writes:
//   [0  .. 127] : file-type string  (FILETYPESIZE = 128 bytes)
//   [128.. 131] : FileID DWORD      (4 bytes)
//
// TWO different header patterns exist depending on how the file was saved:
//
//  Pattern A — CONTAINER, PIECE, WLD, etc.
//    OpenFile(FOT_WRITE)  → WriteFileType (132 bytes)
//    SFile << VERSION     → extra DWORD at [132..135]  (plain-text)
//    SetEncodeType(...)   → encryption starts at byte 136
//    Detection  : DWORD at offset 132 (the payload version) >= 0x0200
//
//  Pattern B — CFG, BIN
//    SetFileType(name, VERSION)  → stores VERSION as FileID
//    OpenFile(FOT_WRITE)         → WriteFileType (132 bytes)
//    SetEncodeType(...)          → encryption starts at byte 132 immediately
//    Detection  : FileID DWORD at offset 128 >= 0x0200
// -----------------------------------------------------------------------
static const int FILETYPESIZE    = 128; // bytes for file-type string
static const int FILEID_SIZE     = 4;   // bytes for FileID DWORD
static const int BYPASS_PATTERN_A = FILETYPESIZE + FILEID_SIZE + 4; // = 136
static const int BYPASS_PATTERN_B = FILETYPESIZE + FILEID_SIZE;     // = 132
static const DWORD VERSION_ENCRYPT = 0x0200;

// Returns the number of plain-text header bytes for the given crypt type.
static int GetBypassBytes(EMBYTECRYPT cryptType)
{
    switch (cryptType)
    {
    case EMBYTECRYPT_CFG:
    case EMBYTECRYPT_BIN:
        return BYPASS_PATTERN_B; // 132 — no extra version DWORD before payload

    default:
        return BYPASS_PATTERN_A; // 136 — has an extra version DWORD before payload
    }
}

// Returns true if the raw file data looks like an encrypted CSerialFile stream.
static bool IsEncryptedFile(const BYTE* data, size_t size, EMBYTECRYPT cryptType)
{
    const int bypass = GetBypassBytes(cryptType);
    if (size < (size_t)bypass + 1)
        return false;

    DWORD checkDWord = 0;
    if (cryptType == EMBYTECRYPT_CFG || cryptType == EMBYTECRYPT_BIN)
    {
        // FileID at offset 128 is the version for these types
        memcpy(&checkDWord, data + FILETYPESIZE, sizeof(DWORD));
    }
    else
    {
        // Extra version DWORD written at offset 132, before SetEncodeType
        memcpy(&checkDWord, data + FILETYPESIZE + FILEID_SIZE, sizeof(DWORD));
    }

    return checkDWord >= VERSION_ENCRYPT;
}

// -----------------------------------------------------------------------
// Process a single file
// Returns true if the file was decrypted and written.
// -----------------------------------------------------------------------
static bool ProcessFile(const std::string& inputPath,
                        const std::string& outputPath,
                        EMBYTECRYPT cryptType)
{
    FILE* fin = fopen(inputPath.c_str(), "rb");
    if (!fin)
    {
        printf("  [ERROR] Cannot open: %s\n", inputPath.c_str());
        return false;
    }

    fseek(fin, 0, SEEK_END);
    long fileSize = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    if (fileSize <= 0)
    {
        fclose(fin);
        return false;
    }

    BYTE* buffer = new BYTE[fileSize];
    fread(buffer, 1, fileSize, fin);
    fclose(fin);

    // Only decrypt files that are flagged as encrypted
    if (!IsEncryptedFile(buffer, (size_t)fileSize, cryptType))
    {
        delete[] buffer;
        return false; // skip - not encrypted
    }

    // Decrypt data after the plain-text header (bypass size depends on type)
    const int bypass = GetBypassBytes(cryptType);
    ByteDecode(buffer + bypass, (DWORD)(fileSize - bypass), cryptType);

    // Write output (may be same path for in-place)
    FILE* fout = fopen(outputPath.c_str(), "wb");
    if (!fout)
    {
        printf("  [ERROR] Cannot write: %s\n", outputPath.c_str());
        delete[] buffer;
        return false;
    }

    fwrite(buffer, 1, fileSize, fout);
    fclose(fout);
    delete[] buffer;
    return true;
}

// -----------------------------------------------------------------------
// Extension filter helpers
// -----------------------------------------------------------------------

// Parses "/ext:.chf,.abf,.vcf" into { ".chf", ".abf", ".vcf" } (lowercased).
static std::vector<std::string> ParseExtFilter(const char* arg)
{
    std::vector<std::string> exts;
    // arg is the full token, e.g. "/ext:.chf,.abf"
    const char* prefix = "/ext:";
    const size_t prefixLen = 5;
    if (_strnicmp(arg, prefix, prefixLen) != 0)
        return exts;

    std::string list = arg + prefixLen;
    // Split by comma
    size_t start = 0;
    while (start <= list.size())
    {
        size_t comma = list.find(',', start);
        if (comma == std::string::npos)
            comma = list.size();

        std::string ext = list.substr(start, comma - start);
        // Normalise: lowercase, ensure leading dot
        for (char& c : ext) c = (char)tolower((unsigned char)c);
        if (!ext.empty() && ext[0] != '.')
            ext = "." + ext;
        if (!ext.empty())
            exts.push_back(ext);

        start = comma + 1;
    }
    return exts;
}

// Returns true when no filter is set, or the filename's extension matches one in the list.
static bool MatchesExtFilter(const char* filename,
                             const std::vector<std::string>& exts)
{
    if (exts.empty())
        return true;

    const char* dot = strrchr(filename, '.');
    if (!dot)
        return false;

    // Lowercase copy of the file extension
    char extLow[64] = {};
    for (int i = 0; i < 63 && dot[i]; ++i)
        extLow[i] = (char)tolower((unsigned char)dot[i]);

    for (const std::string& e : exts)
    {
        if (e == extLow)
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------
// Ensure directory path exists (creates intermediate directories)
// -----------------------------------------------------------------------
static void EnsureDirectory(const std::string& path)
{
    if (path.empty()) return;
    // Use CreateDirectoryA on each component
    std::string cur;
    for (size_t i = 0; i <= path.size(); ++i)
    {
        char c = (i < path.size()) ? path[i] : '\0';
        if (c == '\\' || c == '/' || c == '\0')
        {
            if (!cur.empty())
                CreateDirectoryA(cur.c_str(), NULL);
        }
        if (c) cur += c;
    }
}

// -----------------------------------------------------------------------
// Recursive directory walk
// inputBase / outputBase are the root folders passed on the command line.
// subDir is the current subdirectory relative to those roots.
// -----------------------------------------------------------------------
static void WalkDirectory(const std::string& inputBase,
                          const std::string& outputBase,
                          const std::string& subDir,
                          EMBYTECRYPT cryptType,
                          const std::vector<std::string>& extFilter,
                          int& totalFiles,
                          int& decryptedFiles)
{
    std::string searchPath = inputBase;
    if (!subDir.empty())
        searchPath += "\\" + subDir;
    searchPath += "\\*";

    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);
    if (hFind == INVALID_HANDLE_VALUE)
        return;

    do
    {
        if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0)
            continue;

        std::string relPath = subDir.empty() ? fd.cFileName
                                             : (subDir + "\\" + fd.cFileName);

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            // Recurse into subdirectory
            WalkDirectory(inputBase, outputBase, relPath,
                          cryptType, extFilter, totalFiles, decryptedFiles);
        }
        else
        {
            // Apply extension filter before doing any file I/O
            if (!MatchesExtFilter(fd.cFileName, extFilter))
                continue;

            std::string inputPath  = inputBase  + "\\" + relPath;
            std::string outputPath = outputBase + "\\" + relPath;

            // Make sure the output subdirectory exists
            if (inputBase != outputBase && !subDir.empty())
            {
                std::string outSubDir = outputBase + "\\" + subDir;
                EnsureDirectory(outSubDir);
            }

            ++totalFiles;
            if (ProcessFile(inputPath, outputPath, cryptType))
            {
                ++decryptedFiles;
                printf("  Decrypted: %s\n", relPath.c_str());
            }
        }

    } while (FindNextFileA(hFind, &fd));

    FindClose(hFind);
}

// -----------------------------------------------------------------------
// main
// -----------------------------------------------------------------------
int main(int argc, char* argv[])
{
    printf("EP6 File Decryptor\n");
    printf("==================\n\n");

    if (argc < 2)
    {
        printf("Usage:\n");
        printf("  decrypt.exe <EMBYTECRYPT_TYPE> [/ext:<exts>] [input_dir] [output_dir]\n\n");
        printf("Options:\n");
        printf("  /ext:<exts>  Comma-separated extensions to process (e.g. /ext:.chf,.abf)\n");
        printf("               Omit to process all files.\n\n");
        printf("Examples:\n");
        printf("  decrypt.exe EMBYTECRYPT_CONTAINER\n");
        printf("  decrypt.exe EMBYTECRYPT_CONTAINER /ext:.chf\n");
        printf("  decrypt.exe EMBYTECRYPT_CONTAINER C:\\data\\chars /ext:.chf,.abf\n");
        printf("  decrypt.exe EMBYTECRYPT_CONTAINER C:\\data\\chars C:\\data\\out /ext:.chf\n\n");
        printf("Available types:\n");
        printf("  EMBYTECRYPT_OLD, EMBYTECRYPT_RAN2, EMBYTECRYPT_LEVEL,\n");
        printf("  EMBYTECRYPT_WLD, EMBYTECRYPT_CROWSALE, EMBYTECRYPT_NPCTALK,\n");
        printf("  EMBYTECRYPT_QUEST, EMBYTECRYPT_ITEM, EMBYTECRYPT_CROW,\n");
        printf("  EMBYTECRYPT_CLASSCONST, EMBYTECRYPT_PETCHARSET, EMBYTECRYPT_SKILL,\n");
        printf("  EMBYTECRYPT_CHARSET, EMBYTECRYPT_CONTAINER, EMBYTECRYPT_PIECE,\n");
        printf("  EMBYTECRYPT_ITEMIX, EMBYTECRYPT_CFG, EMBYTECRYPT_EGP,\n");
        printf("  EMBYTECRYPT_BIN, EMBYTECRYPT_TAXISTATION, EMBYTECRYPT_MAPSLIST,\n");
        printf("  (and *2 / ACTIVITY variants)\n");
        return 1;
    }

    // Parse crypt type
    EMBYTECRYPT cryptType = ParseCryptType(argv[1]);
    if (cryptType == (EMBYTECRYPT)-1)
    {
        printf("Error: Unknown crypt type '%s'\n", argv[1]);
        return 1;
    }
    if (cryptType == EMBYTECRYPT_NONE)
    {
        printf("Error: EMBYTECRYPT_NONE cannot decrypt anything.\n");
        return 1;
    }

    // Scan remaining args: collect /ext: flag and positional dir arguments
    std::vector<std::string> extFilter;
    std::vector<std::string> positional;

    for (int i = 2; i < argc; ++i)
    {
        if (_strnicmp(argv[i], "/ext:", 5) == 0)
            extFilter = ParseExtFilter(argv[i]);
        else
            positional.push_back(argv[i]);
    }

    // Resolve input / output directories
    char cwdBuf[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, cwdBuf);

    std::string inputDir  = (positional.size() >= 1) ? positional[0] : cwdBuf;
    std::string outputDir = (positional.size() >= 2) ? positional[1] : inputDir;

    // Strip trailing slashes
    while (!inputDir.empty()  && (inputDir.back()  == '\\' || inputDir.back()  == '/'))
        inputDir.pop_back();
    while (!outputDir.empty() && (outputDir.back() == '\\' || outputDir.back() == '/'))
        outputDir.pop_back();

    printf("Crypt type : %s\n", argv[1]);
    printf("Input dir  : %s\n", inputDir.c_str());
    printf("Output dir : %s\n", outputDir.c_str());
    printf("Mode       : %s\n", (inputDir == outputDir) ? "in-place" : "copy");
    if (!extFilter.empty())
    {
        printf("Ext filter : ");
        for (size_t i = 0; i < extFilter.size(); ++i)
            printf("%s%s", extFilter[i].c_str(), (i + 1 < extFilter.size()) ? ", " : "");
        printf("\n");
    }
    printf("\n");

    if (inputDir != outputDir)
        EnsureDirectory(outputDir);

    int totalFiles     = 0;
    int decryptedFiles = 0;

    WalkDirectory(inputDir, outputDir, "", cryptType, extFilter, totalFiles, decryptedFiles);

    printf("\n--- Done ---\n");
    printf("Scanned   : %d file(s)\n", totalFiles);
    printf("Decrypted : %d file(s)\n", decryptedFiles);
    printf("Skipped   : %d file(s) (not encrypted or too small)\n",
           totalFiles - decryptedFiles);

    return 0;
}
