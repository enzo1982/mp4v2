#ifndef MP4V2_PLATFORM_IO_FILE_H
#define MP4V2_PLATFORM_IO_FILE_H

namespace mp4v2 { namespace platform { namespace io {

///////////////////////////////////////////////////////////////////////////////
///
/// Base file interface.
///
/// File abstracts basic file functionality that may apply to many types of
/// files. All files are assumed to be 64-bit files unless built on a 32-bit
/// platform which does not support 64-bit file offsets. The interface uses a
/// signed 64-bit value which can help with filesize math, thus limiting
/// actual file size to 63-bits, roughly 9.22 million TB.
///
///////////////////////////////////////////////////////////////////////////////
class MP4V2_EXPORT File
{
public:
    //! type used to represent all file sizes and offsets
    typedef int64_t Size;

public:
    virtual ~File();

    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Closes file.
    //!
    //! If the file has not been opened or is not considered the
    //! owner of a filehandle, no action is taken.
    //!
    //! @return true on failure, false on success.
    //!
    ///////////////////////////////////////////////////////////////////////////
    virtual bool close() = 0;

    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Get current file position in bytes.
    //!
    //! @param pos output indicating file position.
    //!
    //! @return true on failure, false on success.
    //!
    ///////////////////////////////////////////////////////////////////////////
    virtual bool getPosition( Size& pos ) = 0;

    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Get current file size in bytes.
    //!
    //! @param size output indicating file size.
    //!
    //! @return true on failure, false on success.
    //!
    ///////////////////////////////////////////////////////////////////////////
    virtual bool getSize( Size& size ) = 0;

    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Set current file position in bytes.
    //!
    //! @param pos new position in file.
    //!
    //! @return true on failure, false on success.
    //!
    ///////////////////////////////////////////////////////////////////////////
    virtual bool setPosition( Size pos ) = 0;

    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Binary stream read.
    //!
    //! The function reads up to a maximum <b>size</b> bytes from file,
    //! storing them in <b>buffer</b>. The number of bytes actually read are
    //! returned in <b>nin</b>.
    //!
    //! @param buffer storage for data read from file.
    //! @param size maximum number of bytes to read from file.
    //! @param nin output indicating number of bytes read from file.
    //! @param maxChunkSize maximum chunk size for reads issued to operating
    //!     system or 0 for default.
    //!
    //! @return true on failure, false on success.
    //!
    ///////////////////////////////////////////////////////////////////////////
    virtual bool read( void* buffer, Size size, Size& nin, Size maxChunkSize = 0 ) = 0;

    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Binary stream write.
    //!
    //! The function writes up to a maximum <b>size</b> bytes from
    //! <b>buffer</b> to file. The number of bytes actually written are
    //! returned in <b>nout</b>.
    //!
    //! @param buffer data to be written out to file.
    //! @param size maximum number of bytes to read from file.
    //! @param nout output indicating number of bytes written to file.
    //! @param maxChunkSize maximum chunk size for writes issued to operating
    //!     system or 0 for default.
    //!
    //! @return true on failure, false on success.
    //!
    ///////////////////////////////////////////////////////////////////////////
    virtual bool write( const void* buffer, Size size, Size& nout, Size maxChunkSize = 0 ) = 0;

protected:
    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Constructor.
    //!
    //! @param name filename of file object, or empty-string.
    //! @param owner true if object is considered owner of file resources
    //!     and responsible for closing them on destruction.
    //!
    ///////////////////////////////////////////////////////////////////////////
    File( std::string name, bool owner );

protected:
    //! file pathname or empty-string if not applicable
    std::string _name;

    //! indicates object owns file and will close it upon destruction
    const bool _owner;

private:
    File(); // disabled

public:
    //! read-only: file pathname or empty-string if not applicable
    const std::string& name;
};

///////////////////////////////////////////////////////////////////////////////
//!
//! Standard file interface.
//! 
//! StdioFile implements standard buffered file functionality.
//! 
//! @ingroup platform_io
//! 
///////////////////////////////////////////////////////////////////////////////
class MP4V2_EXPORT StdioFile : public File
{
public:
    //! type used to represent underlying operating system file handle
    typedef std::FILE* Handle;

public:
    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Constructor.
    //!
    //! @param name filename of file object, or empty-string.
    //! @param owner true if object is considered owner of file resources
    //!     and responsible for closing them on destruction.
    //!
    ///////////////////////////////////////////////////////////////////////////
    explicit StdioFile ( std::string name = "", bool owner = true );

    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Constructor.
    //!
    //! @param handle operating system file-handle of file object.
    //! @param owner true if object is considered owner of file resources
    //!     and responsible for closing them on destruction.
    //!
    ///////////////////////////////////////////////////////////////////////////
    explicit StdioFile ( Handle handle, bool owner = false );

    ~StdioFile();

    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Open file.
    //!
    //! @param mode string beginning with one of the following sequences:
    //!     @arg "r"
    //!         Open file for reading.
    //!         The stream is positioned at the beginning of the file.
    //!     @arg "r+"
    //!         Open file for reading and writing.
    //!         The stream is positioned at the beginning of the file.
    //!     @arg "w"
    //!         Truncate file to zero length or create file for writing.
    //!         The stream is positioned at the beginning of the file.
    //!     @arg "w+"
    //!         Open file for reading and writing.
    //!         The file is created if it does not exist, otherwise it is truncated.
    //!         The stream is positioned at the beginning of the file.
    //!     @arg "a"
    //!         Open file for writing.
    //!         The file is created if it does not exist.
    //!         The stream is positioned at the end of the file.
    //!         Subsequent writes to the file will always end up at the then current end of file.
    //!     @arg "a+"
    //!         Open file for reading and writing.
    //!         The file is created if it does not exist.
    //!         The stream is positioned at the end of the file.
    //!         Subsequent writes to the file will always end up at the then current end of file.
    //! @param name filename to open. If empty-string filename is taken from
    //!     object state.
    //!
    //! @return true on failure, false on success.
    //!
    ///////////////////////////////////////////////////////////////////////////
    bool open( std::string mode, std::string name = "" );

public: // virtual implementation
    bool close();

    bool getPosition ( Size& );
    bool getSize     ( Size& );
    bool setPosition ( Size );

    bool read  ( void*, Size, Size&, Size = 0 );
    bool write ( const void*, Size, Size&, Size = 0 );

private:
    Handle _handle;

public:
    //! read-only: handle to underlying operating system file
    const Handle& handle;

public:
    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Get current file position in bytes.
    //!
    //! @param handle operating system file-handle.
    //! @param pos output indicating file position.
    //!
    //! @return true on failure, false on success.
    //!
    ///////////////////////////////////////////////////////////////////////////
    static bool getPosition( Handle handle, Size& pos );

    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Get current file size in bytes.
    //!
    //! @param handle operating system file-handle.
    //! @param size output indicating file size.
    //!
    //! @return true on failure, false on success.
    //!
    ///////////////////////////////////////////////////////////////////////////
    static bool getSize( Handle handle, Size& size );

    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Set current file position in bytes.
    //!
    //! @param handle operating system file-handle.
    //! @param pos new position in file.
    //!
    //! @return true on failure, false on success.
    //!
    ///////////////////////////////////////////////////////////////////////////
    static bool setPosition( Handle handle, Size pos );
};

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::io

#endif // MP4V2_PLATFORM_IO_FILE_H
