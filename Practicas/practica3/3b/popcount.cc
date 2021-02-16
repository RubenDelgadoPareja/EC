//------------------------------------------------------------------------------
// popcount.cc
// http://www.dalkescientific.com/writings/diary/archive/2011/11/02/faster_popcount_update.html
//------------------------------------------------------------------------------

#include <x86intrin.h> // __rdtscp

#include <algorithm>   // for_each
#include <array>       // array
#include <bitset>      // bitset
#include <chrono>      // now
#include <iomanip>     // setw
#include <iostream>    // cout endl
#include <mutex>       // once_flag
#include <numeric>     // iota

//------------------------------------------------------------------------------

const unsigned GAP = 12;            // gap between columns
const unsigned REP = 1000;          // how many times we repeat the experiment

//------------------------------------------------------------------------------

std::array<unsigned, 1 << 16> list; // 64K * 4B = 256KB

//------------------------------------------------------------------------------

unsigned popcount00(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount01(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount02(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount03(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount04(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount05(unsigned elemento) // from o'hallaron
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount06(unsigned elemento) // gustavo
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount07(unsigned elemento) // from wikipedia
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount08(unsigned elemento) // by Wegner
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned char lt8[256];

unsigned popcount09(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount10(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount11(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount12(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned char lt16[65536];

unsigned popcount13(unsigned elemento)
{
	return elemento;

}

//------------------------------------------------------------------------------

unsigned popcount14(unsigned elemento)
{
	return elemento;

}

//------------------------------------------------------------------------------

unsigned popcount15(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount16(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount17(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount18(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount19(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

unsigned popcount20(unsigned elemento)
{
	return elemento;
}

//------------------------------------------------------------------------------

template <class _F> void test (_F& __f, const char* msg)
{
	using namespace std::chrono;
	
	static std::once_flag flag;
	static unsigned long long cycle_overhead;
	static duration<high_resolution_clock::rep, std::nano> time_overhead;
	unsigned id; // needed by __rdtscp()
	
	std::call_once(flag, [&]
	{
		cycle_overhead = std::numeric_limits<unsigned long long>::max();
		for (unsigned i = 0; i < REP * REP; ++i)
		{
			unsigned long long t1 = __rdtscp(&id);
			unsigned long long t2 = __rdtscp(&id);
			if (t1 < t2)
				cycle_overhead = std::min(cycle_overhead, t2 - t1);
		}
		
		time_overhead = duration<high_resolution_clock::rep, std::nano>::max();
		for (unsigned i = 0; i < REP * REP; ++i)
		{
			auto t1 = high_resolution_clock::now();
			auto t2 = high_resolution_clock::now();
			if (t1 < t2)
				time_overhead = std::min(time_overhead, t2 - t1);
		}
	});
	
	unsigned long long t1 = __rdtscp(&id);
	for (unsigned i = 0; i < REP; ++i)
		__f(list[0]);
	unsigned long long t2 = __rdtscp(&id);
	unsigned long long cycles = (t2 - t1) / REP;
	if (cycles > cycle_overhead) cycles -= cycle_overhead;
	
	unsigned result;
	auto time = duration<high_resolution_clock::rep, std::nano>::max();
	for (unsigned i = 0; i < REP; ++i)
	{
		result = 0;
		auto t1 = high_resolution_clock::now();
		std::for_each(list.begin(), list.end(), [&](unsigned x){ result += __f(x); });
		auto t2 = high_resolution_clock::now();
		time = std::min(time, t2 - t1);
	}
	if (time > time_overhead) time -= time_overhead;
	
	std::cout << '"' << std::setw(GAP * 2 - 2) << msg << '"' 
	          << std::setw(GAP) << result
	          << std::setw(GAP) << cycles
	          << std::setw(GAP) << std::fixed << std::setprecision(2)
	          << duration_cast<nanoseconds>(time).count() / static_cast<double>(list.size())
	          << std::setw(GAP) << duration_cast<nanoseconds>(time).count() / 1000.0
	          << std::endl;
}

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	std::iota(list.begin(), list.end(), 0);

	for (unsigned i = 0; i < 256; ++i)
		lt8[i] = __builtin_popcount(i);

	for (unsigned i = 0; i < 65536; ++i)
		lt16[i] = __builtin_popcount(i);

	std::cout << "#" << std::setw(GAP * 2 - 1) << argv[0]
	          << std::setw(GAP) << "value"
	          << std::setw(GAP) << "cycles"
	          << std::setw(GAP) << "timex1(ns)"
	          << std::setw(GAP) << "timexn(us)"
	          << std::endl;

	test(popcount00, "popcount00(empty)");
	test(popcount01, "popcount01(for)");
	test(popcount02, "popcount02(while)");
	test(popcount03, "popcount03(inline)");
	test(popcount04, "popcount04(inline v2)");
	test(popcount05, "popcount05(0'hallaron)");
	test(popcount06, "popcount06(gus)");
	test(popcount07, "popcount07(wikipedia)");
	test(popcount08, "popcount08(Wegner)");
	test(popcount09, "popcount09(lt8)");
	test(popcount10, "popcount10(lt8 v2)");
	test(popcount11, "popcount11(lt8 v3)");
	test(popcount12, "popcount12(lt8 v4)");
	test(popcount13, "popcount13(lt16)");
	test(popcount14, "popcount14(builtin)");
	test(popcount15, "popcount15(bitset)");
	test(popcount16, "popcount16(recursive)");
	test(popcount17, "popcount17(lt8+ptr)");
	test(popcount18, "popcount18(popcnt)");
	test(popcount19, "popcount19(do while)");
	test(popcount20, "popcount20(O(1))");
}

//------------------------------------------------------------------------------
