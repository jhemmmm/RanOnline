/*!
 * \file XMLEncrypt.h
 *
 * \author Juver
 * \date May 2017
 *
 * 
 */

#ifndef XMLENCRYPT_
#define XMLENCRYPT_

#pragma once

namespace NS_XMLCRYPT
{
	enum
	{
		XMLCRYPT_KEY_1 = 0x10,
		XMLCRYPT_KEY_2 = 0x21,
	};

	void Encode(BYTE* buff, int nSize);
	void Decode(BYTE* buff, int nSize);
};

#endif
