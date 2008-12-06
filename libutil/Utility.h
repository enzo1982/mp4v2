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

#ifndef MP4V2_UTIL_UTILITY_H
#define MP4V2_UTIL_UTILITY_H

namespace mp4v2 { namespace util {

///////////////////////////////////////////////////////////////////////////////
///
/// @defgroup mp4v2_util libutil
/// @{
///
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///
/// Utility general program class.
///
/// This class provides a base implementation for a general utility which
/// helps meet behavioral criteria for command-line executables.
///
/// Inherit batch processing ability is also provided and is used optionally
/// by the concrete implementation.
///
/// Criteria and guidelines for utility behavior in MP4v2 are as follows:
///     @li exit with 0 when the utility succeeds at its main task, 1 for failure.
///     @li print brief-usage when no arguments are supplied and exit with 1.
///     @li print brief-usage when too many arguments are supplied and exit with 1.
///     @li issue brief-usage when invalid argument is supplied and exit with 1.
///     @li support --help option and exit with 0.
///     @li support --version option and exit with 0.
///     @li do not use options with an OPTIONAL_ARG specified. if you desire
///         convenient default behavior for an option, use a NO_ARG short-option,
///         and a REQUIRED_ARG long-option. this avoids 'spacing' ambiguity.
///     @li support both long and short options when possible. Sometimes it's
///         useful to only have the long-option available especially with
///         meta-options { --help, --version }.
///     @li utilities which <b>create</b> new output files should never
///         over-write an existing file unless given the project's universal
///         '-o' or '--overwrite' option.
///
///////////////////////////////////////////////////////////////////////////////
class Utility
{
protected:
    enum LongCode {
        LC_NONE = 0xf0000000, // safe (cannot conflict with char values)
        LC_DEBUG,
        LC_VERBOSE,
        LC_HELP,
        LC_VERSION,
        LC_VERSIONX,
        _LC_MAX // will be used to seeed derived-class long-codes enum
    };

    class Option {
    public:
        Option( char, bool, string, bool, uint32_t, string, string = "ARG", string = "", bool = false );

        const char     scode;
        const bool     shasarg;
        const string   lname;
        const bool     lhasarg;
        const uint32_t lcode;
        const string   descr;
        const string   argname;
        const string   help;
        const bool     hidden;
    };

    class Group {
    public:
        explicit Group( string );
        ~Group();

        void add( const Option& ); // options added this way will not be deleted
        void add( char, bool, string, bool, uint32_t, string, string = "ARG", string = "", bool = false );
        void add( string, bool, uint32_t, string, string = "ARG", string = "", bool = false );

        const string name;

    public:
        typedef list<const Option*> List;

    private:
        List _options;
        List _optionsDelete;

    public:
        const List& options;
    };

    //! structure passed as argument to each job during batch processing
    class JobContext
    {
    public:
        JobContext( string file_ );

        const string  file;               //!< file job is working on
        MP4FileHandle fileHandle;         //!< handle of file, if applicable to job
        bool          optimizeApplicable; //!< indicate file optimization is applicable
        list<void*>   tofree;             //!< memory to free at end of job
    };

public:
    virtual ~Utility();

    bool process();

protected:
    Utility( string, int, char** );

    void printUsage   ( bool );       //!< print usage
    void printHelp    ( bool, bool ); //!< print help
    void printVersion ( bool );       //!< print utility version

    void errf ( const char*, ... ) MP4V2_WFORMAT_PRINTF(2,3); //!< print to stderr
    void outf ( const char*, ... ) MP4V2_WFORMAT_PRINTF(2,3); //!< print to stdout

    bool herrf  ( const char*, ... ) MP4V2_WFORMAT_PRINTF(2,3); //!< print to stderr indicating error
    bool hwarnf ( const char*, ... ) MP4V2_WFORMAT_PRINTF(2,3); //!< print to stderr indicating warning

    void verbose1f ( const char*, ... ) MP4V2_WFORMAT_PRINTF(2,3);
    void verbose2f ( const char*, ... ) MP4V2_WFORMAT_PRINTF(2,3);
    void verbose3f ( const char*, ... ) MP4V2_WFORMAT_PRINTF(2,3);

    bool batch ( int );    //!< process all remaining arguments (jobs)
    bool job   ( string ); //!< process next argument

    //! open file in consideration of overwrite/force options
    bool openFileForWriting( io::StdioFile& );

    bool dryrunAbort();

    // delegates
    virtual bool utility_option( int, bool& ) = 0; //!< process command-line option
    virtual bool utility_job( JobContext& )   = 0; //!< process positional argument

private:
    void formatGroups();
    void debugUpdate( uint32_t );
    void verbose( uint32_t, const char*, va_list );

private:
    string _help;

    prog::Option* _longOptions;
    string        _shortOptions;

protected:
    const string       _name; //!< executable basename
    const int          _argc; //!< arg count
    char* const* const _argv; //!< arg vector

    // common options state
    bool     _optimize;  //!< optimize mp4 file after modification
    bool     _strict;    //!< strict compatibliity
    bool     _dryrun;    //!< dry-run, no writing is actually performed
    bool     _keepgoing; //!< contine batch processing even after error
    bool     _overwrite; //!< overwrite file if already exists
    bool     _force;     //!< force overwriting a file even if read-only
    uint32_t _debug;     //!< mp4 file I/O verbosity
    uint32_t _verbosity; //!< verbosity level, default=1

    uint32_t          _jobCount;
    uint32_t          _debugVerbosity;
    bool              _debugImplicits;

    Group         _group; // group to which standard options are added
    string        _usage;
    string        _description;
    list<Group*>  _groups;

public:
    static const bool SUCCESS;
    static const bool FAILURE;

protected:
    // standard options for concrete utilities to add to _group in constructor
    static const Option STD_OPTIMIZE;
    static const Option STD_DRYRUN;
    static const Option STD_KEEPGOING;
    static const Option STD_OVERWRITE;
    static const Option STD_FORCE;
    static const Option STD_QUIET;
    static const Option STD_DEBUG;
    static const Option STD_VERBOSE;
    static const Option STD_HELP;
    static const Option STD_VERSION;
    static const Option STD_VERSIONX;
};

/// @} ////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util

#endif // MP4V2_UTIL_UTILITY_H
