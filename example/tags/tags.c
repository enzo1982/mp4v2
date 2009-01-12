#include <mp4v2/mp4v2.h>

int main( int argc, char** argv )
{
    int i;

    if( argc != 2 ) {
        printf( "usage: %s file.mp4\n", argv[0] );
        return 1;
    }

    // open file for modification
    MP4FileHandle file = MP4Modify( argv[1], MP4_DETAILS_ERROR, 0 );
    if( file == MP4_INVALID_FILE_HANDLE ) {
        printf( "MP4Modify failed\n" );
        return 1;
    }

    // allocate structure
    const MP4Tags* tags = MP4TagsAlloc();

    // fetch data from MP4 file and populate structure
    MP4TagsFetch( tags, file );

    // show tag name if present
    if( tags->name )
        printf( "name: %s\n", tags->name );

    // show tag artist if present
    if( tags->artist )
        printf( "artist: %s\n", tags->artist );

    // show artwork if present
    printf( "artwork: %d items\n", tags->artworkSize );
    for( i = 0; i < tags->artworkSize; i++ ) {
        const MP4TagArtwork* art = tags->artwork + i;
        printf( "art[%d]: type=%d size=%u data=%p\n", i, art->type, art->size, art->data );
    }

    // set a new name
    MP4TagsSetName( tags, "This is our new name" );

    // remove tag artist
    MP4TagsSetArtist( tags, NULL );

    // remove first artwork
    MP4TagsRemoveArtwork( tags, 0 );

    // show tag name if present
    if( tags->name )
        printf( "name: %s\n", tags->name );

    // show tag artist if present
    if( tags->artist )
        printf( "artist: %s\n", tags->artist );

    // push data from structure to MP4 file
    MP4TagsStore( tags, file );

    // free memory associated with structure and close>
    MP4TagsFree( tags );
    MP4Close( file );

    return 0;
}
