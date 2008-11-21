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
//!
//! Utility general program class.
//!
//! This class provides a base implementation for a general utility which
//! helps meet behavioral criteria for command-line executables.
//!
//! Inherit batch processing ability is also provided and is used optionally
//! by the concrete implementation.
//!
//! Criteria and guidelines for utility behavior in MP4v2 are as follows:
//!     @li exit with 0 when the utility succeeds at its main task, 1 for failure.
//!     @li print brief-usage when no arguments are supplied and exit with 1.
//!     @li print brief-usage when too many arguments are supplied and exit with 1.
//!     @li issue brief-usage when invalid argument is supplied and exit with 1.
//!     @li support --help option and exit with 0.
//!     @li support --version option and exit with 0.
//!     @li do not use options with an OPTIONAL_ARG specified. if you desire
//!         convenient default behavior for an option, use a NO_ARG short-option,
//!         and a REQUIRED_ARG long-option. this avoids 'spacing' ambiguity.
//!     @li support both long and short options when possible. Sometimes it's
//!         useful to only have the long-option available especially with
//!         meta-options { --help, --version }.
//!     @li utilities which <b>create</b> new output files should never
//!         over-write an existing file unless given the project's universal
//!         '-o' or '--overwrite' option.
//!
///////////////////////////////////////////////////////////////////////////////
class Utility
{
protected:
    //! enum reprsenting compatibility behavior mode
    //!
    //! Compatibility behavior will produce warnings or errors which may
    //! be incompatible with the action being undertaken.
    enum CompatibilityMode {
        COMPAT_UNDEFINED, //!< for use when undefined
        COMPAT_NONE,      //!< disable compatibilty checks
        COMPAT_DYNAMIC,   //!< derive mode from job filename
        COMPAT_ITUNES,    //!< iTunes compatibility
    };

    //! structure passed as argument to each job during batch processing
    class JobContext
    {
    public:
        JobContext( string file_ );

        const string  file;       //!< file job is working on
        MP4FileHandle fileHandle; //!< handle of file, if applicable to job
        list<void*>   tofree;     //!< memory to free at end of job
        bool          optimize;   //!< flagged by sub-job
    };

public:
    virtual ~Utility();

    bool process();

protected:
    Utility( string, int, char** );

    void printUsage   ( bool = false );       //!< print usage
    void printHelp    ( bool, bool = false ); //!< print help
    void printVersion ( );                    //!< print utility version

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

    // delegates
    virtual void utility_printHelp( ostream& ) {  } //!< extended help
    virtual bool utility_process()             = 0; //!< parse options
    virtual bool utility_job( JobContext& )    = 0; //!< process argument

protected:
    const string        _name;    //!< executable basename
    const int           _argc;    //!< arg count
    char* const* const  _argv;    //!< arg vector
    string              _usage;   //!< usage summary
    string              _help;    //!< help summary
    bool                _helpCompatibility;
    const prog::Option* _options; //!< long options vector

    // common options
    bool     _dryrun;    //!< dry-run, no writing is actually performed
    bool     _force;     //!< force overwriting a file even if read-only
    bool     _keepgoing; //!< contine batch processing even after error
    bool     _overwrite; //!< overwrite file if already exists
    uint32_t _verbosity; //!< verbosity level, default=1
    bool     _optimize;  //!< optimize mp4 file after modification

    CompatibilityMode _compatibility;    //!< overall batch compatibility
    CompatibilityMode _compatibilityJob; //!< job compatibility
    bool              _strict;           //!< strict compatibliity

private:
    void verbose( uint32_t, const char*, va_list );

    typedef map<string,CompatibilityMode> CompatibilityMap;

    CompatibilityMap _compatibilityByExtension;

public:
    static const bool SUCCESS;
    static const bool FAILURE;

protected:
    static string  toString( CompatibilityMode );          //!< convert to string
    static string& toString( CompatibilityMode, string& ); //!< convert to string with storage
};

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util

#endif // MP4V2_UTIL_UTILITY_H
