#ifndef MP4V2_UTIL_UTILEXCEPTION_H
#define MP4V2_UTIL_UTILEXCEPTION_H

namespace mp4v2 { namespace util {

///////////////////////////////////////////////////////////////////////////////

class MP4V2_EXPORT UtilException
{
protected:
    string _what;

public:
    const string& what;

public:
    explicit UtilException( const string& what );
    explicit UtilException( const ostringstream& what );
    ~UtilException();
};

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util

#endif // MP4V2_UTIL_UTILEXCEPTION_H
