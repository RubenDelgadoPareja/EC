 #include <bitset>

unsigned popcount00(unsigned elem)
{
	return elem;
}

unsigned popcount01(unsigned elem)
{
	unsigned count = 0;
	for (unsigned i = 0; i < 8 * sizeof(elem); ++i)
	{
		unsigned mask = 1 << i;
        // count += (elem & mask);
		if (elem & mask)
            ++count;
	}
	return count;
}

unsigned popcount02(unsigned elem)
{
	unsigned count = 0;
	for (unsigned i = 0; i < 8 * sizeof(elem); ++i)
	{
		unsigned mask = 1 << i;
        count += (elem & mask) != 0;
	}
	return count;
}

unsigned popcount03(unsigned elem)
{
	unsigned count = 0;
	for (unsigned i = 0; i < 8 * sizeof(elem); ++i)
	{
        count += elem & 1;
        elem >>= 1;
	}
	return count;
}

unsigned popcount04(unsigned elem)
{
	unsigned count = 0;
	while(elem)
	{
        count += elem & 1;
        elem >>= 1;
	}
	return count;
}

unsigned popcount05(unsigned elem)
{
	unsigned count = 0;
	while(elem)
	{
        asm("shr $1,%0  \n\t"
            "adc $0,%1  \n\t"
            :"+r" (elem), "+r" (count)
        );
	}
	return count;
}

unsigned popcount06(unsigned elem)
{
	unsigned count = 0;
    asm("0: shr $1,%0   \n\t"
        "   jz 1f       \n\t"
        "   adc $0,%1   \n\t"
        "   jmp 0b      \n\t"
        "1: adc $0,%1   \n\t"
        :"+r" (elem), "+r" (count)
    );
	return count;
}

unsigned popcount07(unsigned elem)
{
    unsigned count = 0;
    for (unsigned i = 0; i < 8; ++i)
    {
        count += elem & 0x01010101;
        elem >>= 1; // elem = elem >> 1
    }
    count += (count >> 16);
    count += (count >>  8);
    return count & 0xff;
}

unsigned popcount08(unsigned elem)
{
    elem = (elem & 0x55555555) + ((elem >>  1) & 0x55555555);
    elem = (elem & 0x33333333) + ((elem >>  2) & 0x33333333);
    elem = (elem & 0x0f0f0f0f) + ((elem >>  4) & 0x0f0f0f0f);
    elem = (elem & 0x00ff00ff) + ((elem >>  8) & 0x00ff00ff);
    elem = (elem & 0x0000ffff) + ((elem >> 16) & 0x0000ffff);
    return elem;
}

unsigned popcount09(unsigned elem)
{
    unsigned count;
    asm("popcnt %1,%0":"=r"(count):"r"(elem));
    return count;
}

unsigned popcount10(unsigned elem)
{
    return __builtin_popcount(elem);
}

unsigned popcount11(unsigned elem)
{
    return std::bitset<sizeof(elem)*8>(elem).count();
    //std::bitset<32> bits(elem);
    //return bits.count();
}

