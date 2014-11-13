#include <MCF/StdMCF.hpp>
#include <MCF/Core/String.hpp>
#include <MCF/Core/LastError.hpp>
using namespace MCF;

extern "C" unsigned int MCFMain() noexcept {
	auto so(u8"哈11𪚥22喵33"_u8s);
	Cesu8String cu8s(so);
	for(auto ch : cu8s){
		std::printf("%02hhX ", ch);
	}
	std::putchar('\n');
	std::puts(AnsiString(cu8s).GetCStr());
	std::puts(AnsiString(GetWin32ErrorDesc(ERROR_HANDLE_EOF)).GetCStr());
	return 0;
}
