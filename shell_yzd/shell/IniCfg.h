//////////////////////////////////////////////////////
//
//服务器后台公用的头文件
//
//
//////////////////////////////////////////////////////
#ifndef _INI_CFG_H_ 
#define _INI_CFG_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <string>

//#include "SrvAppBase.h"

//获取系统主目录
std::string GetCfgFilePath(const char * szConfigName);
std::string GetLogPath();



 int strcompi(const char* pString1, const char* pString2);
 void LTruncate(char* szString, const char* szFill);

 void RTruncate(char* szString, const char* szFill);

 void Truncate(char* szString, const char* szFill);

 void rtrim(char * aszData,int aiLen );

class  CIniCfg
{
public:
    CIniCfg()
    {
        memset(m_szFileName,0,sizeof(m_szFileName));
    }
    void SetPath(const char * szFileName)
    {
        strcpy(m_szFileName,szFileName);
    }
    //操作ini的两个函数
    std::string GetValue(
        const char* lpAppName,   
        const char* lpKeyName,   
        const char* lpDefault);

    bool SetValue(
        const char* lpAppName,		
        const char* lpKeyName,		
        const char* lpString);
    bool SetValue(
        const char* lpAppName,		
        const char* lpKeyName,		
        std::string strValue);
    bool SetValueI(
        const char* lpAppName,		
        const char* lpKeyName,		
        int  nValue);

    bool ReadFile() ;
    bool WriteFile() {return true;};
private:
    char m_szFileName[100];
};

#endif
