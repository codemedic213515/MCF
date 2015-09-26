#include <MCF/Core/String.hpp>
#include <MCF/Core/Time.hpp>
#include <MCF/Core/Random.hpp>
#include <MCFCRT/env/mcfwin.h>

using namespace MCF;

extern "C" unsigned MCFMain(){

	Utf8String str, to_find;
	str.Resize(102400);
	to_find.Resize(1024);

	unsigned seed = GetRandomUint32();
	auto rand_char = [&]{
		seed = seed * 1664525 + 1013904223;
		return static_cast<char>((seed >> 16) % 3 + '0');
	};

	double st1 = 0, st2 = 0;
	unsigned faster = 0, slower = 0, correct = 0, incorrect = 0;
	for(unsigned i = 0; i < 1000; ++i){
		for(auto ptr = str.GetBegin(); ptr != str.GetEnd(); ++ptr){
			*ptr = rand_char();
		}
		for(auto ptr = to_find.GetBegin(); ptr != to_find.GetEnd(); ++ptr){
			*ptr = rand_char();
		}
		std::memcpy(str.GetBegin() + GetRandomUint64() % (str.GetSize() - to_find.GetSize()), to_find.GetBegin(), to_find.GetSize());
//		std::printf("str = %s\nwrd = %s\n", str.GetStr(), to_find.GetStr());

		double t11, t12, t21, t22;
		char *pos1, *pos2;

		t11 = GetHiResMonoClock();
		pos1 = std::strstr(str.GetStr(), to_find.GetStr());
		t12 = GetHiResMonoClock();
//		std::printf("msvcrt strstr()  time = %f, pos = %p\n", t12 - t11, pos1);

		t21 = GetHiResMonoClock();
		auto off = str.Find(to_find);
		pos2 = (off == Utf8String::kNpos) ? nullptr : (str.GetBegin() + off);
		t22 = GetHiResMonoClock();
//		std::printf("String::Find()   time = %f, pos = %p\n", t22 - t21, pos2);

		const auto t1 = t12 - t11, t2 = t22 - t21;
		st1 += t1;
		st2 += t2;

		if(t1 > t2){
			++faster;
		} else {
			++slower;
		}

		if(pos2 == pos1){
			++correct;
		} else {
			++incorrect;
		}
	}
	std::printf("st1 = %f, st2 = %f\n", st1, st2);
	std::printf("faster = %u, slower = %u, correct = %u, incorrect = %u\n", faster, slower, correct, incorrect);

	return 0;
}
