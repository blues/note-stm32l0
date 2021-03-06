/*!
 * @file n_ftoa.c
 *
 *
 *	stm32tpl --	 STM32 C++ Template Peripheral Library
 *
 *	Copyright (c) 2009-2014 Anton B. Gusev aka AHTOXA
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in
 *	all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
 *
 * 	description	 : convert double to string
 *
 */

#include "n_lib.h"

char * JNtoA(JNUMBER f, char * buf, int original_precision)
{
	char * ptr = buf;
	char * p = ptr;
	char * p1;
	char c;
	long intPart;

	// For our low-SRAM devices we'd rather have this on the stack
	const JNUMBER rounders[JNTOA_PRECISION + 1] =
		{
			0.5,				// 0
			0.05,				// 1
			0.005,				// 2
			0.0005,				// 3
			0.00005,			// 4
			0.000005,			// 5
			0.0000005,			// 6
			0.00000005,			// 7
			0.000000005,		// 8
			0.0000000005,		// 9
			0.00000000005		// 10
		};

	// Check specifically for uncommon but bad floating point numbers that can't be converted
	uint8_t fbytes[8];
	memcpy(&fbytes, &f, sizeof(fbytes));
	bool wasFF = true;
	int i;
	for (i=0; i<(int)sizeof(fbytes); i++)
		if (fbytes[i] != 0xff) wasFF = false;
	if (wasFF)
		f = 0.0;

	// check precision bounds
	int precision = original_precision;
	if (precision < 0 || precision > JNTOA_PRECISION)
		precision = JNTOA_PRECISION;

	// sign stuff
	if (f < 0)
	{
		f = -f;
		*ptr++ = '-';
	}

	if (original_precision < 0)	 // negative precision == automatic precision guess
	{
		if (f < 1.0) precision = 6;
		else if (f < 10.0) precision = 5;
		else if (f < 100.0) precision = 4;
		else if (f < 1000.0) precision = 3;
		else if (f < 10000.0) precision = 2;
		else if (f < 100000.0) precision = 1;
		else precision = 0;
	}

	// round value according the precision
	if (precision)
		f += rounders[precision];

	// integer part...
	intPart = (int) f;
	f -= intPart;

	if (!intPart)
		*ptr++ = '0';
	else
	{
		// save start pointer
		p = ptr;

		// convert (reverse order)
		while (intPart)
		{
			*p++ = '0' + intPart % 10;
			intPart /= 10;
		}

		// save end pos
		p1 = p;

		// reverse result
		while (p > ptr)
		{
			c = *--p;
			*p = *ptr;
			*ptr++ = c;
		}

		// restore end pos
		ptr = p1;
	}

	// decimal part
	if (precision)
	{

		// place decimal point
		*ptr++ = '.';

		// convert
		while (precision--)
		{
			f *= 10.0;
			c = (int) f;

			// Invalid floating point numbers (specifically 0xffffff) end up at this point
			// with a c == 255 after the coercion
			if (c > 9) c = 0;

			*ptr++ = '0' + c;
			f -= c;
		}
	}

	// terminating zero
	*ptr = 0;

	// Remove trailing zero's if automatic precision
	if (NULL != strchr(buf, '.')) {
		if (original_precision < 0) {
			--ptr;
			while (ptr > (buf+1) && *ptr == '0')
				*ptr-- = 0;
			if (*ptr == '.')
				*ptr = 0;
		}
	}

	return buf;
}
