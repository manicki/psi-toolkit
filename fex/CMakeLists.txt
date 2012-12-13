add_library(fex SHARED
    #7z_C/7zAlloc.c
    #7z_C/7zBuf.c
    #7z_C/7zCrc.c
    #7z_C/7zDecode.c
    #7z_C/7zExtract.c
    #7z_C/7zHeader.c
    #7z_C/7zIn.c
    #7z_C/7zItem.c
    #7z_C/7zStream.c
    #7z_C/Bcj2.c
    #7z_C/Bra86.c
    #7z_C/LzmaDec.c
    #zlib/adler32.c
    #zlib/crc32.c
    #zlib/inffast.c
    #zlib/inflate.c
    #zlib/inftrees.c
    #zlib/zutil.c
    fex/Binary_Extractor.cpp
    fex/blargg_errors.cpp
    fex/fex.cpp
    fex/Gzip_Extractor.cpp
    fex/Rar_Extractor.cpp
    fex/Zip_Extractor.cpp
    fex/blargg_common.cpp
    fex/Data_Reader.cpp
    fex/File_Extractor.cpp
    fex/Gzip_Reader.cpp
    fex/Zip7_Extractor.cpp
    fex/Zlib_Inflater.cpp
)

include_directories("${psi-toolkit_SOURCE_DIR}/fex")
#include_directories("${psi-toolkit_SOURCE_DIR}/fex/zlib")
#include_directories("${psi-toolkit_SOURCE_DIR}/fex/7z_C")
include_directories("${psi-toolkit_SOURCE_DIR}/fex/fex")

install(TARGETS fex DESTINATION lib/)
