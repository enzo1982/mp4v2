#include "impl.h"

namespace mp4v2 { namespace impl {

///////////////////////////////////////////////////////////////////////////////

bool
LessIgnoreCase::operator()( const string& x, const string& y ) const
{
    const string::size_type xlen = x.length();
    const string::size_type ylen = y.length();

    if( xlen < ylen ) {
        for( string::size_type i = 0; i < xlen; i++ ) {
            if( std::toupper(x[i]) < std::toupper(y[i]) )
                return true;
        }
        return true;
    }
    else {
        for( string::size_type i = 0; i < ylen; i++ ) {
            if( std::toupper(x[i]) < std::toupper(y[i]) )
                return true;
        }
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::impl
