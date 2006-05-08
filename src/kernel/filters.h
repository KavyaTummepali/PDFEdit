// vim:tabstop=4:shiftwidth=4:noexpandtab:textwidth=80
/*
 * $RCSfile$
 *
 * $Log$
 * Revision 1.11  2006/05/08 10:57:47  misuj1am
 *
 * -- small changes
 * 	-- CStream::Buffer wchar_t changed to char
 * 	-- commented some debugging output out
 * 	-- buggie: changed setRawBuffer to replace characters instead of appending
 *
 * Revision 1.10  2006/05/07 10:03:12  misuj1am
 *
 * -- ADD: filter exception
 * -- filter handling improved
 *
 * Revision 1.9  2006/05/03 23:56:16  misuj1am
 *
 * -- cstream improvement
 * 	-- ADD: buffer of raw chars
 * 	-- ADD: getStringRepresentation returns the same object as in pdf file, except that it outputs only printable characters
 * 	-- and more
 *
 * Revision 1.8  2006/05/01 13:53:07  hockm0bm
 * new style printDbg
 *
 *
 */

#ifndef _FILTERS_H_
#define _FILTERS_H_

// static includes
#include "static.h"

//=======================================
namespace filters {
//=======================================

/** I/O Character type. */
typedef char StreamChar;
typedef boost::iostreams::filtering_istream InputStream;
	
//
// Forward declarations
//
struct NoFilter;
	
//=======================================
// Filter factory
//=======================================

	
/**
 * Filter creator class. Factory design pattern implemented here.
 *
 * This is an implementation of STRATEGY design pattern. We need a set of filters, which
 * behave differently but on the same data with the same information avaliable. We make
 * them interchangeable. [GoF/Strategy]
 *
 * If we would like to expose this interface outside, we would have to create a Mediator between Filter class
 * and CStream. Nothing more, nothing less.
 * 
 * REMARK: We do not use template implementation because we do not know at compile time, which implementation will be used.
 * REMARK2: Change getSupportedStreams & setStringRepresentation in order to expose newly created filters.
 */
struct CFilterFactory
{
	/**
	 * Create filter class.
	 *
	 * @param filterName Name of the filter.
	 *
	 * @return Filter, if not found, NoFilter is created.
	 */
	template<typename OUTPUT, typename FILTERS>
	static void addFilters (OUTPUT& out, const FILTERS& filterNames)
	{
		typename FILTERS::const_iterator it = filterNames.begin ();
		for (; it != filterNames.end(); ++it)
		{
			if ("" == *it)
			{
				out.push (NoFilter ());

			}else if ("" == *it)
			{
			
			}else // Not supported filter occured
				throw FilterNotSupported ();
		}
	}

	/**
	 * Get all supported filters.
	 *
	 * @param supported Supported streams.
	 */
	template<typename Container>
	static void getSupportedStreams (Container& supported) 
	{
		supported.push_back ("NoFilter");
	}

};

//=======================================
// Concrete Filters
//=======================================

/**
 * Specific filter.
 */
struct NoFilter : public boost::iostreams::input_filter
{
	typedef StreamChar	char_type;
	typedef boost::iostreams::input_filter_tag  category;
	
	/** Default constructor. */
	NoFilter () {utilsPrintDbg (debug::DBG_DBG, "NoFilter created."); };

	/** Single char output function. */
	template<typename Source>
	int get (Source& src) 
	{ 
		return boost::iostreams::get (src);
	}

	/** Destructor. */
	~NoFilter () { utilsPrintDbg (debug::DBG_DBG, "NoFilter destroyed."); };

};

/**
 *
 */
template<typename T>
class buffer_source 
{
private:
    typedef typename T::size_type   size_type;
	const T& buffer;
	size_type pos;

public:
	typedef typename T::value_type  char_type;
	typedef struct boost::iostreams::source_tag category;

	/** Constructor. */
	buffer_source (const T& _b) : buffer (_b), pos(0) {utilsPrintDbg (debug::DBG_DBG, "");};

	/** Read function.*/
    std::streamsize 
	read (char_type* s, std::streamsize n)
    {
        using namespace std;
        streamsize amt = static_cast<streamsize>(buffer.size() - pos);
        streamsize result = min (n, amt);
        if (0 != result) 
		{
			utilsPrintDbg (debug::DBG_DBG, "Position: " << pos << "Size: " << n);
            std::copy (buffer.begin() + pos, buffer.begin() + pos + result, s);
            pos += result;
            return result;
			
        } else 
            return EOF;
    }

};

/**
 * Container sink.
 */
template<typename T>
class buffer_sink 
{
	T& buffer;

public:
    typedef typename T::value_type  char_type;
    typedef boost::iostreams::sink_tag category;
    
	buffer_sink (T& container) : buffer(container) {}
	
    std::streamsize 
	write(const char_type* s, std::streamsize n)
    {
        buffer.insert (buffer.end(), s, s + n);
        return n;
    }
    
	T& getBuffer() {return buffer;}
};




//=======================================
template<typename T>
struct Printable
{
	typedef char Char;
	Char operator () (T _c) const
	{
		Char c = _c;
		if ('!' < c && c < '~')
			return c;
		else
			return '+';
	}
};


//=======================================
} // namespace filters
//=======================================


#endif // _FILTERS_H_
