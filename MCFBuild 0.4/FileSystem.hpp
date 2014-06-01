// MCF Build
// Copyleft 2014, LH_Mouse. All wrongs reserved.

#ifndef MCFBUILD_FILE_SYSTEM_HPP_
#define MCFBUILD_FILE_SYSTEM_HPP_

#include "Model.hpp"
#include "../MCF/Core/VVector.hpp"
#include "../MCF/Core/String.hpp"

namespace MCFBuild {

extern bool GetFileContents(MCF::Vector<unsigned char> &vecData, const MCF::WideString &wcsPath);
// 如果不要文件内容 pvecData 可以置空。
extern bool GetFileContentsAndSha256(MCF::Vector<unsigned char> *pvecData, Sha256 &shaChecksum, const MCF::WideString &wcsPath);

extern MCF::WideString GetFullPath(const MCF::WideString &wcsSrc);

extern void CreateDirectory(const MCF::WideString &wcsPath);

}

#endif
