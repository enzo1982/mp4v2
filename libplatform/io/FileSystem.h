#ifndef MP4V2_PLATFORM_IO_FILESYSTEM_H
#define MP4V2_PLATFORM_IO_FILESYSTEM_H

namespace mp4v2 { namespace platform { namespace io {

///////////////////////////////////////////////////////////////////////////////
//!
//! General file-system abstraction.
//!
//! FileSystem abstracts operations on files and directories.
//!
//! @ingroup platform_io
//!
///////////////////////////////////////////////////////////////////////////////
class FileSystem
{
public:
    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Query file presence.
    //! Check if <b>name</b> exists.
    //! @param name filename to query.
    //! @return true if present, false otherwise.
    //!
    ///////////////////////////////////////////////////////////////////////////
    static bool exists( std::string name );

    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Query directory type.
    //! Check if <b>name</b> exists and is a directory.
    //! @param name pathname to query.
    //! @return true if directory, false otherwise.
    //!
    ///////////////////////////////////////////////////////////////////////////
    static bool isDirectory( std::string name );

    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Query file type.
    //! Check if <b>name</b> exists and is a file.
    //! @param name filename to query.
    //! @return true if file, false otherwise.
    //!
    ///////////////////////////////////////////////////////////////////////////
    static bool isFile( std::string name );

    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Rename file or directory.
    //!
    //! Rename <b>oldname</b> to <b>newname</b>.
    //! If <b>newname</b> exists, it is first removed.
    //! Both <b>oldname</b> and <b>newname</b> must be of the same type;
    //! that is, both must be either files or directories and must reside on
    //! the same filesystem.
    //!
    //! @param oldname existing pathname to rename.
    //! @param newname new pathname.
    //!
    //! @return true on failure, false on success.
    //!
    ///////////////////////////////////////////////////////////////////////////
    static bool rename( std::string oldname, std::string newname );

    ///////////////////////////////////////////////////////////////////////////
    //!
    //! Generate temporary filename.
    //!
    //! @param name output containing generated filename.
    //! @param dir relative or absolute directory of filename.
    //! @param prefix text prepended to base filename.
    //! @param suffix text appended to base filename.
    //!
    //! @return true if directory, false otherwise.
    //!
    ///////////////////////////////////////////////////////////////////////////
    static bool tempFilename( std::string& name, string dir = ".", string prefix = "tmp", string suffix = "" );
};

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::io

#endif // MP4V2_PLATFORM_IO_FILESYSTEM_H
