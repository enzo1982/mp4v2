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

namespace mp4v2 { namespace impl { namespace itmf {

///////////////////////////////////////////////////////////////////////////////

CoverArtBox::Item::Item()
    : type     ( BT_UNDEFINED )
    , buffer   ( NULL )
    , size     ( 0 )
    , autofree ( false )
{
}

///////////////////////////////////////////////////////////////////////////////

CoverArtBox::Item::~Item()
{
    if( autofree && buffer )
        free( buffer );
}

///////////////////////////////////////////////////////////////////////////////

void
CoverArtBox::Item::reset()
{
    if( autofree && buffer )
        free( buffer );

    type     = BT_UNDEFINED;
    buffer   = NULL;
    size     = 0;
    autofree = false;
}

///////////////////////////////////////////////////////////////////////////////

bool
CoverArtBox::add( MP4FileHandle file, const Item& item )
{
    MP4File& mp4 = *((MP4File*)file);

    const char* const covr_name = "moov.udta.meta.ilst.covr";
    MP4Atom* covr = mp4.FindAtom( covr_name );
    if( !covr ) {
        mp4.AddDescendantAtoms( "moov", "udta.meta.ilst.covr" );

        covr = mp4.FindAtom( covr_name );
        if( !covr )
            return true;
    }

    // use empty data atom if one exists
    MP4Atom* data = NULL;
    uint32_t index = 0;
    const uint32_t atomc = covr->GetNumberOfChildAtoms();
    for( uint32_t i = 0; i < atomc; i++ ) {
        MP4Atom* atom = covr->GetChildAtom( i );

        MP4BytesProperty* metadata = NULL;
        if( !atom->FindProperty( "data.metadata", (MP4Property**)&metadata ))
            continue;

        if( metadata->GetCount() )
            continue;

        data = atom;
        index = i;
        break;
    }

    // no empty atom found, create one.
    if( !data ) {
        data = MP4Atom::CreateAtom( covr, "data" );
        covr->AddChildAtom( data );
        data->Generate();
        index = covr->GetNumberOfChildAtoms() - 1;
    }

    return set( file, item, index );
}

///////////////////////////////////////////////////////////////////////////////

bool
CoverArtBox::get( MP4FileHandle file, Item& item, uint32_t index )
{
    item.reset();
    MP4File& mp4 = *((MP4File*)file);

    MP4Atom* covr = mp4.FindAtom( "moov.udta.meta.ilst.covr" );
    if( !covr )
        return true;

    if( !(index < covr->GetNumberOfChildAtoms()) )
        return true;

    MP4Atom* data = covr->GetChildAtom( index );
    if( !data )
        return true;

    MP4BytesProperty* metadata = NULL;
    if ( !data->FindProperty( "data.metadata", (MP4Property**)&metadata ))
        return true;

    metadata->GetValue( &item.buffer, &item.size );
    item.autofree = true;
    item.type = static_cast<BasicType>( data->GetFlags() );

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
CoverArtBox::list( MP4FileHandle file, ItemList& out )
{
    out.clear();
    MP4File& mp4 = *((MP4File*)file);

    const uint32_t artc = mp4.GetMetadataCoverArtCount();
    out.resize( artc );
    for( uint32_t i = 0; i < artc; i++ )
        get( file, out[i], i );

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
CoverArtBox::remove( MP4FileHandle file, uint32_t index )
{
    MP4File& mp4 = *((MP4File*)file);

    if( index == numeric_limits<uint32_t>::max() )
        return !mp4.DeleteMetadataAtom( "covr" );

    MP4Atom* covr = mp4.FindAtom( "moov.udta.meta.ilst.covr" );
    if( !covr )
        return true;

    if( !(index < covr->GetNumberOfChildAtoms()) )
        return true;

    MP4Atom* data = covr->GetChildAtom( index );
    if( !data )
        return true;

    covr->DeleteChildAtom( data );
    delete data;

    if( covr->GetNumberOfChildAtoms() == 0 )
        return !mp4.DeleteMetadataAtom( "covr" );

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
CoverArtBox::set( MP4FileHandle file, const Item& item, uint32_t index )
{
    MP4File& mp4 = *((MP4File*)file);

    MP4Atom* covr = mp4.FindAtom( "moov.udta.meta.ilst.covr" );
    if( !covr )
        return true;

    if( !(index < covr->GetNumberOfChildAtoms()) )
        return true;

    MP4Atom* data = covr->GetChildAtom( index );
    if( !data )
        return true;

    MP4BytesProperty* metadata = NULL;
    if ( !data->FindProperty( "data.metadata", (MP4Property**)&metadata ))
        return true;

    // autodetect type if BT_UNDEFINED
    const BasicType final_type = (item.type == BT_UNDEFINED)
        ? computeBasicType( item.buffer, item.size )
        : item.type;

    // set type; note: not really flags due to b0rked atom structure
    data->SetFlags( final_type );
    metadata->SetValue( item.buffer, item.size );

    return false;
}

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::impl::itmf
