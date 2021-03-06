#pragma once

#include "FileHandle.h"
#include "FileAccessFlags.h"
#include "FileAccessMode.h"

#if defined(PLATFORM_Linux)
#include "FileBaseLinuxReferences.h"
#elif defined(PLATFORM_Win)
#include "FileBaseWinReferences.h"
#endif


namespace et {
namespace core {


class FILE_BASE {
public:
    static FILE_HANDLE Open( const char * pathName, FILE_ACCESS_FLAGS accessFlags, FILE_ACCESS_MODE accessMode );

    static bool Close( FILE_HANDLE handle );

	static bool GetEntrySize(FILE_HANDLE handle, int64& size);

    static bool ReadFile( FILE_HANDLE handle, std::vector<uint8> & content, uint64 const numBytes, uint64 const offset = 0u );

    static bool WriteFile( FILE_HANDLE handle, const std::vector<uint8> & content );

	static bool DeleteFile( const char * pathName );

private:
#if defined(PLATFORM_Linux)
    #include "FileBaseLinuxMembers.h"
#elif defined(PLATFORM_Win)

#endif
};


} // namespace core
} // namespace et
