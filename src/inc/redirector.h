/* FreeEMS-Loader- the open source s19 loader with special features for FreeEMS
 *
 * Copyright (C) 2008-2011 by Sean Keys <skeys@powerefi.com>
 *
 * This file is part of the FreeEMS-Loader project.
 *
 * FreeEMS-Loader software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeEMS-Loader software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with any FreeEMS-Loader software.  If not, see http://www.gnu.org/licenses/
 *
 * We ask that if you make any changes to this file you email them upstream to
 * us at info(at)powerefi(dot)com or, even better, fork the code on github.com!
 *
 * Thank you for choosing FreeEMS-Loader to load your firmware!
 *
 */

#ifndef REDIRECTOR_H
#define REDIRECTOR_H

#endif /* FREEEMS_LOADERREDIRECTOR_H_ */

template<class Elem = char, class Tr = std::char_traits<Elem> >
class StdRedirector: public std::basic_streambuf<Elem, Tr> {
	/**
	 * Callback Function.
	 */
	typedef void (*pfncb)(const Elem*, std::streamsize _Count, void* pUsrData);

public:
	/**
	 * Constructor.
	 * @param a_Stream the stream to redirect
	 * @param a_Cb the callback function
	 * @param a_pUsrData user data passed to callback
	 */
	StdRedirector(std::ostream& a_Stream, pfncb a_Cb, void* a_pUsrData) :
		m_Stream(a_Stream), m_pCbFunc(a_Cb), m_pUserData(a_pUsrData) {
		//redirect stream
		m_pBuf = m_Stream.rdbuf(this);
	}
	;

	/**
	 * Destructor.
	 * Restores the original stream.
	 */
	~StdRedirector() {
		m_Stream.rdbuf(m_pBuf);
	}

	/**
	 * Override xsputn and make it forward data to the callback function.
	 */
	std::streamsize xsputn(const Elem* _Ptr, std::streamsize _Count) {
		m_pCbFunc(_Ptr, _Count, m_pUserData);
		return _Count;
	}

	/**
	 * Override overflow and make it forward data to the callback function.
	 */
	typename Tr::int_type overflow(typename Tr::int_type v) {
		Elem ch = Tr::to_char_type(v);
		m_pCbFunc(&ch, 1, m_pUserData);
		return Tr::not_eof(v);
	}

	void compileWarning() {
		int i = 0;
		i++;
	}

protected:
	std::basic_ostream<Elem, Tr>& m_Stream;
	std::streambuf* m_pBuf;
	pfncb m_pCbFunc;
	void* m_pUserData;
};

#else
/* let us know if we are being untidy with headers */
#warning "Header file REDIRECTOR_H seen before, sort it out!"
/* end of the wrapper ifdef from the very top */
#endif
