#ifndef _LOG_SYS_STORE_LOG_H_
#define _LOG_SYS_STORE_LOG_H_

#pragma warning( push, 3 )
#pragma once

#pragma warning( disable: 4786 ) // disable STL warnings

#include <string>

//////////////////////////////////////////////////////////////////////////
// Abstract class set three default function which must support any child.
// Any child must support buffered and non-buffered store 

class IStoreLog 
{
	public:
    virtual ~IStoreLog( void ){};
		virtual int FlushData() = 0;
		virtual int WriteString( const std::string &Message ) = 0;
		virtual int SetBufferLimit( long lSize ) = 0;
};

#pragma warning( pop )

#endif /* _LOG_SYS_STORE_LOG_H_ */