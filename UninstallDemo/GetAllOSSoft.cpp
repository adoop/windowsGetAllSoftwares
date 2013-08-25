#include "StdAfx.h"
#include "GetAllOSSoft.h"
#include<vector>
#include<algorithm>
typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);

typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);




CGetAllOSSoft::CGetAllOSSoft(void)
{
}


CGetAllOSSoft::~CGetAllOSSoft(void)
{
}






















































































































































































































































































































































































































































































































































































































































































































































































































// check is 64 bit or not
BOOL CGetAllOSSoft::IsWow64OS(void)
{
    SYSTEM_INFO sysInfo;
    PGNSI pGNSI;
    ZeroMemory(&sysInfo, sizeof(SYSTEM_INFO));

    // Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

    pGNSI = (PGNSI) GetProcAddress(
                GetModuleHandle(TEXT("kernel32.dll")),
                "GetNativeSystemInfo");
    if(NULL != pGNSI)
        pGNSI(&sysInfo);
    else GetSystemInfo(&sysInfo);

    if ( sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 )
    {
        return TRUE;
    }
    else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL )
    {
        return  FALSE;
	}else{
		exit(-1);
	    return  FALSE;
	}




}





// Get All  Installed software displayname and unintsall string
vector<INSTALLEDINFO> CGetAllOSSoft::GetAllInstalledSoftware(CString csKeyPath)
{
    vector<INSTALLEDINFO> v_Softwares;


    CRegKey cReg;
    DWORD dwSize =MAX_PATH;
    long lret_open;
    if(IsWow64OS())
    {
        lret_open=cReg.Open(HKEY_LOCAL_MACHINE,csKeyPath,KEY_READ|KEY_WOW64_64KEY);
    }
    else
    {
        lret_open=cReg.Open(HKEY_LOCAL_MACHINE,csKeyPath,KEY_READ);
    }

    if(lret_open!=ERROR_SUCCESS)
    {
        AfxMessageBox(_T("can't open key!"));

        cReg.Close();
        exit(-1);
    }

    DWORD dwIndex = 0;
    DWORD cbName = IS_KEY_LEN;
    TCHAR szSubKeyName[IS_KEY_LEN];
    LONG lRet;

    while ((lRet = cReg.EnumKey(dwIndex, szSubKeyName, &cbName)) != ERROR_NO_MORE_ITEMS)
    {
        if (lRet == ERROR_SUCCESS)
        {
            CRegKey regItem;
            LONG lReItem;
            if(IsWow64OS())
            {
                lReItem=regItem.Open(cReg, szSubKeyName, KEY_READ|KEY_WOW64_64KEY);
            }
            else
            {
                lReItem=regItem.Open(cReg, szSubKeyName, KEY_READ);
            }


            if (lReItem == ERROR_SUCCESS)
            {

                DWORD dwBufferValue = 0;
                BOOL bShow =TRUE;
                if (regItem.QueryDWORDValue(IS_COMNET, dwBufferValue) == ERROR_SUCCESS)
                {
                    if(dwBufferValue)
                    {
                        bShow = FALSE;
                    }
                }
                // Get the "DisplayName" value
                ULONG ulChars;
                INSTALLEDINFO s_SoftInfo;
                if (regItem.QueryStringValue(IS_DISPLAY, NULL, &ulChars) == ERROR_SUCCESS)
                {
                    CString strName;
                    regItem.QueryStringValue(IS_DISPLAY, strName.GetBuffer(ulChars), &ulChars);
                    strName.ReleaseBuffer();
                    // Add to the array
                    s_SoftInfo.csDisPlayName = strName;
                }
                ULONG ulCharsUn;
                if (regItem.QueryStringValue(IS_UNINSTALLSTRING, NULL, &ulCharsUn) == ERROR_SUCCESS)
                {
                    CString csUnInstallString;
                    regItem.QueryStringValue(IS_UNINSTALLSTRING, csUnInstallString.GetBuffer(ulCharsUn), &ulCharsUn);
                    csUnInstallString.ReleaseBuffer();
                    // Add to the array
                    s_SoftInfo.csUnInstallString = csUnInstallString;
                }
                if((!bShow)||(s_SoftInfo.csUnInstallString.IsEmpty())||(s_SoftInfo.csDisPlayName.IsEmpty())||(s_SoftInfo.csDisPlayName.Find(L"OEM Application Profile") != -1))
                {
					//??? may add some funct
                }
                else
                {
                    v_Softwares.push_back(s_SoftInfo);
                }
            }
        }
        dwIndex++;
        cbName = IS_KEY_LEN;
    }



    return v_Softwares;
}



// get all
vector<INSTALLEDINFO> CGetAllOSSoft::GetAllSofts(void)
{
    vector<INSTALLEDINFO> v_Softwares;
    if(IsWow64OS())
    {
        vector<INSTALLEDINFO> v_Softwares_wow;
        v_Softwares =GetAllInstalledSoftware(OS64INSTALLPATH);
        v_Softwares_wow = GetAllInstalledSoftware(OS32INSTALLPATH);
        for(int i= 0; i<v_Softwares_wow.size(); i++)
        {
            v_Softwares.push_back(v_Softwares_wow.at(i));
        }
    }
    else
    {
        v_Softwares = GetAllInstalledSoftware(OS32INSTALLPATH);
    }
	//add user softwares
	 vector<INSTALLEDINFO> v_Softwares_current_user =GetCurrentUserSofts();

	 for(int j=0;j<v_Softwares_current_user.size();j++){
	 v_Softwares.push_back(v_Softwares_current_user.at(j));
	 }
    //sort
    sort(v_Softwares.begin(),v_Softwares.end());
    //clear repeat  item
    v_Softwares.erase(unique(v_Softwares.begin(),v_Softwares.end()),v_Softwares.end());
    return v_Softwares;
}



// get softs form Hey_current_user
vector<INSTALLEDINFO> CGetAllOSSoft::GetCurrentUserSofts(void)
{
	 vector<INSTALLEDINFO> v_Softwares;

	CString csKeyPath = OS32INSTALLPATH;
    CRegKey cReg;
    DWORD dwSize =MAX_PATH;
    long lret_open;
    if(IsWow64OS())
    {
        lret_open=cReg.Open(HKEY_CURRENT_USER,csKeyPath,KEY_READ|KEY_WOW64_64KEY);
    }
    else
    {
        lret_open=cReg.Open(HKEY_CURRENT_USER,csKeyPath,KEY_READ);
    }

    if(lret_open!=ERROR_SUCCESS)
    {
        AfxMessageBox(_T("can't open key!"));

        cReg.Close();
        exit(-1);
    }

    DWORD dwIndex = 0;
    DWORD cbName = IS_KEY_LEN;
    TCHAR szSubKeyName[IS_KEY_LEN];
    LONG lRet;

    while ((lRet = cReg.EnumKey(dwIndex, szSubKeyName, &cbName)) != ERROR_NO_MORE_ITEMS)
    {
        if (lRet == ERROR_SUCCESS)
        {
            CRegKey regItem;
            LONG lReItem;
            if(IsWow64OS())
            {
                lReItem=regItem.Open(cReg, szSubKeyName, KEY_READ|KEY_WOW64_64KEY);
            }
            else
            {
                lReItem=regItem.Open(cReg, szSubKeyName, KEY_READ);
            }


            if (lReItem == ERROR_SUCCESS)
            {

                DWORD dwBufferValue = 0;
                BOOL bShow =TRUE;
                if (regItem.QueryDWORDValue(IS_COMNET, dwBufferValue) == ERROR_SUCCESS)
                {
                    if(dwBufferValue)
                    {
                        bShow = FALSE;
                    }
                }
                // Get the "DisplayName" value
                ULONG ulChars;
                INSTALLEDINFO s_SoftInfo;
                if (regItem.QueryStringValue(IS_DISPLAY, NULL, &ulChars) == ERROR_SUCCESS)
                {
                    CString strName;
                    regItem.QueryStringValue(IS_DISPLAY, strName.GetBuffer(ulChars), &ulChars);
                    strName.ReleaseBuffer();
                    // Add to the array
                    s_SoftInfo.csDisPlayName = strName;
                }
                ULONG ulCharsUn;
                if (regItem.QueryStringValue(IS_UNINSTALLSTRING, NULL, &ulCharsUn) == ERROR_SUCCESS)
                {
                    CString csUnInstallString;
                    regItem.QueryStringValue(IS_UNINSTALLSTRING, csUnInstallString.GetBuffer(ulCharsUn), &ulCharsUn);
                    csUnInstallString.ReleaseBuffer();
                    // Add to the array
                    s_SoftInfo.csUnInstallString = csUnInstallString;
                }
                if((!bShow)||(s_SoftInfo.csUnInstallString.IsEmpty())||(s_SoftInfo.csDisPlayName.IsEmpty())||(s_SoftInfo.csDisPlayName.Find(L"OEM Application Profile") != -1))
                {

                }
                else
                {
                    v_Softwares.push_back(s_SoftInfo);
                }
            }
        }
        dwIndex++;
        cbName = IS_KEY_LEN;
    }



    return v_Softwares;
	
}
