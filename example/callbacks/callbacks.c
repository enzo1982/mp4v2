/* This example makes use of the MP4IOCallbacks API to use custom input/output
 * routines.
 */

#include <mp4v2/mp4v2.h>
#include <stdio.h>

/*****************************************************************************/

static int my_seek( void* handle, int64_t pos )
{
    if( fseeko( (FILE*)handle, pos, SEEK_SET ) != 0)
        return -1;

    return 0;
}

static int my_read( void* handle, void* buffer, int64_t size, int64_t* nin )
{
    if( fread( buffer, size, 1, (FILE*)handle ) != 1)
        return -1;

    *nin = size;
    return 0;
}

static int64_t my_size( void* handle )
{
    int64_t pos = ftello( (FILE*)handle );

    if( fseeko( (FILE*)handle, 0, SEEK_END ) != 0 )
        return -1;

    int64_t size = ftello( (FILE*)handle );

    if( fseeko( (FILE*)handle, pos, SEEK_SET ) != 0 )
        return -1;

    return size;
}

/*****************************************************************************/

int main( int argc, char** argv )
{
    if( argc != 2 ) {
        printf( "usage: %s file.mp4\n", argv[0] );
        return 1;
    }

    /* populate data structure with custom functions.
     * safe to put on stack as it will be immediately copied internally.
     */
    MP4IOCallbacks callbacks = { 0 };

    callbacks.seek = my_seek;
    callbacks.read = my_read;
    callbacks.size = my_size;

    /* open file for read */
    FILE* handle = fopen( argv[1], "rb" );
    if( handle == NULL ) {
        printf( "fopen failed\n" );
        return 1;
    }

    MP4FileHandle file = MP4ReadCallbacks( &callbacks, handle );
    if( file == MP4_INVALID_FILE_HANDLE ) {
        printf( "MP4ReadCallbacks failed\n" );
        return 1;
    }

    /* dump file contents */
    if( !MP4Dump( file, 0 ))
        printf( "MP4Dump failed\n" );

    /* cleanup and close */
    MP4Close( file, 0 );

    fclose( handle );

    return 0;
}
