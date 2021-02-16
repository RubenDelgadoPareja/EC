//------------------------------------------------------------------------------
// popcount-v.cc
// http://www.dalkescientific.com/writings/diary/archive/2011/11/02/faster_popcount_update.html
//------------------------------------------------------------------------------

#include <x86intrin.h> // __rdtscp

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

unsigned popcount00(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount01(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount02(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount03(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount04(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount05(unsigned *begin, unsigned *end) // from o'hallaron
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount06(unsigned *begin, unsigned *end) // gustavo
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount07(unsigned *begin, unsigned *end) // from wikipedia
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount08(unsigned *begin, unsigned *end) // by Wegner
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned char lt8[256];

unsigned popcount09(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount10(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount11(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount12(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned char lt16[65536];

unsigned popcount13(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount14(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount15(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount16(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount17(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount18(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount19(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount20(unsigned *begin, unsigned *end)
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
}

//------------------------------------------------------------------------------

unsigned popcount21(unsigned *begin, unsigned *end) // SSSE3
{
	unsigned count = 0;
	while (begin != end)
		count += *begin++;
	return count;
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
			time_overhead = std::min(time_overhead, t2 - t1);
		}
	});
	
	unsigned long long cycles = std::numeric_limits<unsigned long long>::max();
	for (unsigned i = 0; i < REP; ++i)
	{
		unsigned long long t1 = __rdtscp(&id);
		__f(list.begin(), list.end());
		unsigned long long t2 = __rdtscp(&id);
		if (t1 < t2)
			cycles = std::min(cycles, t2 - t1);
	}
	if (cycles > cycle_overhead) cycles -= cycle_overhead;
	
	unsigned result;
	auto time = duration<high_resolution_clock::rep, std::nano>::max();
	for (unsigned i = 0; i < REP; ++i)
	{
		result = 0;
		auto t1 = high_resolution_clock::now();
		result += __f(list.begin(), list.end());
		auto t2 = high_resolution_clock::now();
		time = std::min(time, t2 - t1);
	}
	if (time > time_overhead) time -= time_overhead;
	
	std::cout << '"' << std::setw(GAP * 2 - 2) << msg << '"'
	          << std::setw(GAP) << result
	          << std::setw(GAP) << std::fixed << std::setprecision(2)
	          << cycles / static_cast<double>(list.size())
	          << std::setw(GAP) << std::fixed << std::setprecision(2)
	          << time.count() / static_cast<double>(list.size())
	          << std::setw(GAP) << time.count() / 1000.0 // ns -> us
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
	test(popcount15, "popcount15(builtinll)");
	test(popcount16, "popcount16(bitset)");
	test(popcount17, "popcount17(bitset2)");
	test(popcount18, "popcount18(inline v3)");
	test(popcount19, "popcount19(inline v4)");
	test(popcount20, "popcount20(inline v5)");
	test(popcount21, "popcount21(ssse3)");
}

//------------------------------------------------------------------------------
