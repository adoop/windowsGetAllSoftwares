#pragma once
#include <vector>
#define	IS_KEY		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall")
#define	IS_KEY_LEN	256
#define IS_DISPLAY	_T("DisplayName")
#define IS_COMNET	 L"SystemComponent"
#define IS_UNINSTALLSTRING	_T("UninstallString")
#define OS64INSTALLPATH  L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"
#define OS32INSTALLPATH   L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"
using namespace std;
struct INSTALLEDINFO
{
    CString csDisPlayName;
    CString csUnInstallString;
    BOOL operator<(const INSTALLEDINFO&oInstall) const
    {
        return this->csDisPlayName<oInstall.csDisPlayName;
    }
    BOOL operator ==(const INSTALLEDINFO&oInstall) const
    {
        return (this->csDisPlayName==oInstall.csDisPlayName)&&(this->csUnInstallString==oInstall.csUnInstallString);
    }
};
class CGetAllOSSoft
{
public:
    CGetAllOSSoft(void);
    ~CGetAllOSSoft(void);
    // check is 64 bit or not
    BOOL IsWow64OS(void);
    // Get All  Installed software displayname and unintsall string key64 and 32
    vector<INSTALLEDINFO> GetAllInstalledSoftware(CString csKeyPath );
    // get finally all list
    vector<INSTALLEDINFO> GetAllSofts(void);
	// get softs form Hey_current_user
	vector<INSTALLEDINFO> GetCurrentUserSofts(void);
};

