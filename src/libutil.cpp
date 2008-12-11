///////////////////////////////////////////////////////////////////////////////
//
//  The contents of this file are subject to the Mozilla Public License
//  Version 1.1 (the "License"); you may not use this file except in
//  compliance with the License. You may obtain a copy of the License at
//  http://www.mozilla.org/MPL/
//
//  Software distributed under the License is distributed on an "AS IS"
//  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
//  License for the specific language governing rights and limitations
//  under the License.
// 
//  The Original Code is MP4v2.
// 
//  The Initial Developer of the Original Code is Kona Blend.
//  Portions created by Kona Blend are Copyright (C) 2008.
//  All Rights Reserved.
//
//  Contributors:
//      Kona Blend, kona8lend@@gmail.com
//
///////////////////////////////////////////////////////////////////////////////

#include "impl.h"
#include "libutil.h"
#include <stack>

namespace mp4v2 { namespace util {
    using namespace mp4v2::impl;

///////////////////////////////////////////////////////////////////////////////

UtilException::UtilException( const string& what_ )
    : _what ( what_ )
    , what  ( _what )
{
}

///////////////////////////////////////////////////////////////////////////////

UtilException::UtilException( const ostringstream& what_ )
    : _what ( what_.str() )
    , what  ( _what )
{
}

///////////////////////////////////////////////////////////////////////////////

UtilException::~UtilException()
{
}

///////////////////////////////////////////////////////////////////////////////

// search atom recursively for any 64-bit characteristics.
// nlargsize indicates number of atoms which use largesize extension.
// nversion1 indicates number of atoms which use version==1 extension.
// nspecial indicates number of special 64-bit atoms;
//   eg: stbl may container one of { stco || co64 } for chunkoffsets.
void searchFor64bit( MP4Atom& atom, FileSummaryInfo& info )
{
    const uint32_t max = atom.GetNumberOfChildAtoms();
    for( uint32_t i = 0; i < max; i++ ) {
        MP4Atom& child = *atom.GetChildAtom( i );

        if( child.GetLargesizeMode() )
            info.nlargesize++;

        MP4Integer8Property* version;
        if( child.FindProperty( "version", (MP4Property**)&version ) && version->GetValue() == 1 )
            info.nversion1++;

        if( !strcmp( child.GetType(), "co64" ))
            info.nspecial++;

        searchFor64bit( child, info );
    }
}

///////////////////////////////////////////////////////////////////////////////

bool
fileFetchSummaryInfo( MP4FileHandle file, FileSummaryInfo& info )
{
    if( file == MP4_INVALID_FILE_HANDLE )
        return true;
    MP4File& mp4 = *((MP4File*)file);

    MP4Atom* root = mp4.FindAtom( "" );
    if( !root )
        return true;

    MP4Atom* ftyp = root->FindAtom( "ftyp" );
    if( !ftyp )
        return true;

    info.major_brand   = ((MP4StringProperty*)ftyp->GetProperty( 0 ))->GetValue();
    info.minor_version = ((MP4Integer32Property*)ftyp->GetProperty( 1 ))->GetValue();

    uint32_t max = ((MP4Integer32Property*)ftyp->GetProperty( 2 ))->GetValue();
    MP4TableProperty* table = (MP4TableProperty*)ftyp->GetProperty( 3 );
    for( uint32_t i = 0; i < max; i++ ) {
        string s = ((MP4StringProperty*)table->GetProperty( 0 ))->GetValue( i );

        // remove spaces so brand set is presentable
        string stripped;
        const string::size_type max = s.length();
        for( string::size_type pos = 0; pos < max; pos++ ) {
            if( s[pos] != ' ' )
                stripped += s[pos];
        }

        if( stripped.empty() )
            continue;

        info.compatible_brands.insert( stripped );
    }

    info.nlargesize = 0;
    info.nversion1  = 0;
    info.nspecial   = 0;
    searchFor64bit( *root, info );

    return false;
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
