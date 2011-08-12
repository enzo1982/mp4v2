#ifndef MP4V2_IMPL_XTRA_H
#define MP4V2_IMPL_XTRA_H

namespace mp4v2 { namespace impl { namespace Xtra {

///////////////////////////////////////////////////////////////////////////////

class XtraTag
{
public:
    std::string name;
    MP4XtraBasicType type;
    union {
        std::wstring *pwstr;
        uint64_t	number;
        MP4GUID		guid;
        MP4FILETIME	fileTime;
    };

public:
    XtraTag(const char *pszName, const wchar_t *pwsz, const size_t len)
        : name(pszName)
    {
        type = MP4_XTRA_BT_UNICODE;
        this->pwstr = new std::wstring(pwsz, len);
    }
    XtraTag(const char *pszName, const uint64_t &number)
        : name(pszName)
    {
        type = MP4_XTRA_BT_INT64;
        this->number = number;
    }
    XtraTag(const char *pszName, const MP4FILETIME &fileTime)
        : name(pszName)
    {
        type = MP4_XTRA_BT_FILETIME;
        this->fileTime = fileTime;
    }
    XtraTag(const char *pszName, const MP4GUID &guid)
        : name(pszName)
    {
        type = MP4_XTRA_BT_GUID;
        this->guid = guid;
    }
    XtraTag(const XtraTag &other)
        : name(other.name)
        , type(other.type)
    {
        if (type == MP4_XTRA_BT_UNICODE)
            pwstr = new std::wstring(*other.pwstr);
        else if (type == MP4_XTRA_BT_INT64)
            number = other.number;
        else if (type == MP4_XTRA_BT_FILETIME)
            fileTime = other.fileTime;
        else if (type == MP4_XTRA_BT_GUID)
            guid = other.guid;
    }
    ~XtraTag()
    {
        if (type == MP4_XTRA_BT_UNICODE)
            delete pwstr;
    }

    void c_alloc ( MP4XtraTag*& );
    void c_fetch ( MP4XtraTag*& );
    void c_free  ( MP4XtraTag*& );
};

///////////////////////////////////////////////////////////////////////////////

void xtraGetItems( MP4File& file, list<MP4XtraTag> &items );

///////////////////////////////////////////////////////////////////////////////

const XtraTagProperty* xtraGetItem( MP4File& file, const char *name );

bool xtraSetItem( MP4File& file, const char* name, const XtraTagProperty& item );

bool xtraRemoveItem( MP4File& file, const char *name );

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::impl::Xtra

#endif // MP4V2_IMPL_XTRA_H
