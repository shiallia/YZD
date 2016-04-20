#include "stdafx.h"
#include "IniCfg.h"
//#include "LogMsg.h"

//���ִ�Сд���ַ����Ƚ�
int strcompi(const char* pString1, const char* pString2)
{
    int i;
    int nRet = 0;

    for(i=0;pString1[i] && pString2[i];i++)
    {
        if(toupper(pString1[i]) != toupper(pString2[i]))
        {
            if(toupper(pString1[i])>toupper(pString2[i]))
                nRet = 1;
            else
                nRet = -1;
            break;
        }
    }

    if(nRet==0)
    {
        if(pString1[i])		 nRet = 1;
        else if(pString2[i]) nRet = -1;
    }

    return nRet;
}
void LTruncate(char* szString, const char* szFill)
{
    int i;
    int iLen;

    iLen = strlen(szString);

    for(i = 0; i < iLen; i++)
    {
        if(!strchr(szFill, szString[i]))
            break;
    }

    strcpy(szString, szString+i);
}


// ȥ���ұ������ڵڶ��������е��ַ���
// ֱ��������һ�����ڵ��ַ�Ϊֹ��
void RTruncate(char* szString, const char* szFill)
{
    int i, len;
    len = strlen(szString);

    for(i = len-1; i >= 0; i--)
    {
        if(!strchr(szFill, szString[i]))
            break;
    }

    szString[i+1] = '\0';
}

// ȥ���ұ��ַ�ֵΪ�ڶ����������ַ���
// ֱ��������һ�����ڵ��ַ�Ϊֹ��

void Truncate(char* szString, const char* szFill)
{
    RTruncate(szString, szFill);
    LTruncate(szString, szFill);
}

void rtrim(char * aszData,int aiLen )
{
    int iLen,i;
    iLen = strlen(aszData);
    if (aiLen == 0)
    {	//ֱ��rtrim()
        for (i = iLen -1 ;i >=0 ;i--)
        {
            if(aszData[i] != ' ' && aszData[i] != '\t' &&
                aszData[i] != '\r' && aszData[i] != '\n' //add by lemon 20050614
                )
                return;
            if(aszData[i] == ' ' || aszData[i] == '\t' ||
                aszData[i] == '\r' || aszData[i] == '\n' //add by lemon 20050614
                )
                aszData[i] = '\0';
        }
        return;
    }
    if (iLen > aiLen)
    {
        for (i = iLen -1 ;i >=aiLen   ;i--)
        {
            //if(aszData[i] == ' ')
            aszData[i] = '\0';
        }
    }
    else
    {
        for (i = iLen;i<aiLen ;i++)
            aszData[i] = ' ';
        aszData[aiLen ] = '\0';
    }
}


std::string CIniCfg::GetValue(
                 const char * aszSession,        
                 const char * aszKey,        
                 const char * aszDefault)
{
	char szText[255] = "";
	std::string strRet = "";
#ifdef WIN32
	GetPrivateProfileString(aszSession,aszKey,aszDefault,szText,sizeof(szText),m_szFileName);
	strRet = szText;
	return strRet;
#endif
    FILE * fp;
    int  iInSession = 0; /*�Ƿ��������SESSION��*/
    int  iHaveKey = 0;
    int  i ,j;
    int  iLen = 0;
    int  iBegin = 0;
    char szSession[40] = "";
    char szKey[40] = "";
	char szLineBuf[255] = "";	/*һ�����ݵĻ���*/
    strRet = aszDefault;

    if ((fp = fopen(m_szFileName,"r")) == NULL) return strRet;
    while(fgets(szLineBuf,254,fp) != NULL)
    {
        /*����������ַ���*/
        iLen = strlen(szLineBuf);
        /*ȡ��һ���ַ�*/
        i = 0;
        for (;i < iLen && szLineBuf[i] == ' ' && szLineBuf[i] < '!' ;i ++ );
        if (i == iLen ) continue;
        if (szLineBuf[i] == '#') continue;
        if (szLineBuf[i] == ';') continue;
        if (szLineBuf[i] == '[')
        {
            if (iInSession)		/*��ǰ���������Session�У�����������һ��session�����˳�(������Key)*/
            {
                fclose(fp);
                return strRet;
            }
            /*ȡSession*/
            i++;
            for (j=i; (j < iLen && szLineBuf[j] != ']'); j ++);
            memcpy(szSession, szLineBuf+i, j-i);
            szSession[j-i] = '\0';
            if (strcmp(szSession,aszSession) == 0)
                iInSession = 1;

            continue;
        }
        else if (iInSession)	/*��Keyʱ��Ӧ����Session�У�����continue*/
        {
            /*ȡKey*/
            for (j=i; ((j < iLen) && (szLineBuf[j] != ' ') && (szLineBuf[j] != '=') && (szLineBuf[j] >= '0')); j++);
            memcpy(szKey, szLineBuf+i, j-i);
            szKey[j - i] = '\0';
            i = j;
            if (strcmp(szKey, aszKey) == 0)	/*�ҵ�Key*/
            {	/*ȡ����ֵ*/
                for (; ((i < iLen) && ((unsigned char)szLineBuf[i] < '-' || szLineBuf[i] == '=')); i++);

                for (j=i; j < iLen && (szLineBuf[j] != '#' && szLineBuf[j] != ';') ;j ++);
                if (j <=i)
                {
                    strcpy(szText,"");
                }
                else
                {
                    memcpy(szText,szLineBuf+i, j-i);
                    rtrim(szText,0);
                    szText[j-i] = '\0';
                    //strcpy(aszText, szText);
                }
                iHaveKey = 1;
                break;
            }
        }
        else
        {
            continue;
        }
    }
    fclose(fp);
    //rtrim(aszText,0);
    if (iHaveKey == 1)
        strRet = szText;
    return strRet;
}
bool CIniCfg::SetValue(
                 const char* lpAppName,		
                 const char* lpKeyName,		
                 const char* lpString)
{
#ifdef WIN32
	//ֱ����WritePrivateProfileStringʵ��
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,m_szFileName);
	return true;
#endif
    char *token, *pLineToken;
    FILE *stream;
    char szBuffer[1024];
    char szAppSeps[]="[";
    char szKeySeps[]="=";
    int  nLen;
    char *pBuf, *pFileBuf;
    char *pSectionPos, *pKeyPos;
    unsigned long ulFileLen,i,k;

    bool bSectionFound = false;
    bool bKeyFound = false;

    if((stream = fopen( m_szFileName, "r+b" )) == NULL )
    {
        return false;
    }

    pBuf = szBuffer;

    fseek(stream, 0, SEEK_END);
    ulFileLen = ftell(stream);

    pFileBuf = (char*)malloc(ulFileLen + 1);
    memset(pFileBuf,0,ulFileLen + 1);
    fseek(stream, 0, SEEK_SET);
    fread(pFileBuf, ulFileLen, 1, stream);
    fclose(stream);

    pLineToken = pFileBuf;
    while(pLineToken)
    {
        if(!bSectionFound)
        {
            pSectionPos = pLineToken;
        }
        else
        {
            pKeyPos = pLineToken;
        }

        pLineToken = strtok(pLineToken, "\n");

        if(!pLineToken) break;
        strcpy(pBuf, pLineToken);

        pLineToken+=strlen(pLineToken)+1;

        LTruncate(pBuf, " \t");		/* ȥ��ǰ���ո��TAB */

        token = strstr(szBuffer, "//");	/* ȥ��ע�� */
        if(token) token[0] = 0;

        if(pBuf = strchr(szBuffer, '\\'))
        {
            if(pBuf[1]!='x' && pBuf[1]!='X')
                continue;
        }
        pBuf = szBuffer;

        if(strlen(szBuffer)==0) continue;

        if(!bSectionFound)
        {
            token = strtok(szBuffer, szAppSeps);

            if(token == szBuffer) 
                continue; /* ����Ѱ�� */

            LTruncate(token, " \t");
            RTruncate(token, " ]\t\r\n");
            if(strcompi(token, lpAppName))
                continue;  /* ����һ��ƥ��Section */
            else
                bSectionFound = true;
        }
        else
        {
            nLen = strlen(szBuffer);
            token = strtok(szBuffer, szKeySeps);
            if(strlen(token) == nLen) 
            {   /* �ж��Ƿ��뿪ָ����Section */ 
                token = strtok(szBuffer, szAppSeps);
                if(token != szBuffer)
                    break;		/* ���뿪���˳����� */
                else
                    continue;	/* δ�뿪������Ѱ�� */
            }

            LTruncate(szBuffer, " \t");
            RTruncate(szBuffer, " \t");

            if(strcompi(szBuffer, lpKeyName))
            {
                continue;
            }
            else
            {   /* �ҵ� */
                bKeyFound = true;
                break;
            }
        }
    }

    if((stream = fopen( m_szFileName, "wb" )) == NULL )
    {
        free(pFileBuf);
        return false;
    }

    if(!bSectionFound && !bKeyFound)
    {
        for(i=0;i<ulFileLen;i++)
        {
            if(pFileBuf[i]==0) pFileBuf[i]='\n';
        }
        fwrite(pFileBuf, i, 1, stream);

        fseek(stream, 0, SEEK_END);
        sprintf(szBuffer, "[%s]\r\n", lpAppName);
        fwrite(szBuffer, strlen(szBuffer), 1, stream);
        sprintf(szBuffer, "%s=%s\r\n", lpKeyName, lpString);
        fwrite(szBuffer, strlen(szBuffer), 1, stream);
    }
    else if(bSectionFound && bKeyFound)
    {
        for(i=0;pFileBuf+i<pKeyPos;i++)
        {
            if(pFileBuf[i]==0) pFileBuf[i]='\n';
        }
        fwrite(pFileBuf, i, 1, stream);

        sprintf(szBuffer, "%s=%s\r\n", lpKeyName, lpString);
        fwrite(szBuffer, strlen(szBuffer), 1, stream);

        k=0;
        for(i+=(strlen(pKeyPos)+1);i<ulFileLen;i++)
        {
            if(pFileBuf[i]==0) pFileBuf[i]='\n';
            k++;
        }
        fwrite(pKeyPos+strlen(pKeyPos)+1, k, 1, stream);

    }
    else if(bSectionFound && !bKeyFound)
    {
        for(i=0;pFileBuf+i<pKeyPos;i++)
        {
            if(pFileBuf[i]==0) pFileBuf[i]='\n';
        }
        fwrite(pFileBuf, i, 1, stream);

        sprintf(szBuffer, "%s=%s\r\n", lpKeyName, lpString);
        fwrite(szBuffer, strlen(szBuffer), 1, stream);

        k=0;
        for(;i<ulFileLen;i++,k++)
        {
            if(pFileBuf[i]==0) pFileBuf[i]='\n';
        }
        fwrite(pKeyPos, k, 1, stream);
    }
	fflush(stream);
    fclose(stream);
    free(pFileBuf);
    return true;
}
bool CIniCfg::SetValueI(
               const char* lpAppName,		
               const char* lpKeyName,		
               int  nValue)
{
    char szTemp[20] = "";
    sprintf(szTemp,"%d",nValue);
    return SetValue(lpAppName,lpKeyName,szTemp);
}
bool CIniCfg::SetValue(
              const char* lpAppName,		
              const char* lpKeyName,		
              std::string strValue)
{
    return SetValue(lpAppName,lpKeyName,strValue.c_str());
}
bool CIniCfg::ReadFile()
{
    FILE * fp;
    if ((fp = fopen(m_szFileName,"r")) == NULL) return false;
    fclose(fp);
    return true;

}


//����ini�ļ�����ȡ�����������ļ�·��,���÷�ʽ�磺GetCfgFilePath("ma.ini");
//�����ļ�Ĭ�Ϸ��ڿ�ִ���ļ�Ŀ¼��
std::string GetCfgFilePath(const char * szConfigName)
{
    char szHome[100] = "";
    char *szTmp = NULL;
    memset(szHome,0,sizeof(szHome));
    szTmp = getenv("YST_HOME");
    if (szTmp)
        strcpy(szHome,szTmp);
    //LogMsg(1,"Home1 = %s.\n",szHome);
    //yst_getenv("YST_HOME",szHome,sizeof(szHome));
    if (strlen(szHome) == 0)
    {
        strcpy(szHome,".");
    }
    else
    {
        //��\�滻��/
        for (int i = 0 ; i < strlen(szHome) ; i ++)
            if (szHome[i] == '\\') szHome[i] = '/';
    }
    //LogMsg(1,"Home2 = %s.\n",szHome);
    char szCfgFile[100] = "";
    memset(szCfgFile,0,sizeof(szCfgFile));
    sprintf(szCfgFile,"%s/%s",szHome,szConfigName);
    //LogMsg(1,"szCfgFile = %s.\n",szCfgFile);
    //std::string tmp;
    //tmp = szCfgFile;
    //LogMsg(1,"Tmp = %s",tmp.c_str());
    //return tmp;
    return std::string(szCfgFile);
}
//��ȡ��־�ļ��ľ���·��
//��־Ŀ¼Ĭ�Ϸ��ڵ�ǰ��ִ��Ŀ¼�µ�logĿ¼
std::string GetLogPath()
{
    char szHome[100] = "";
    char *szTmp = NULL;
    memset(szHome,0,sizeof(szHome));
    szTmp = getenv("YST_HOME");
    if (szTmp)
        strcpy(szHome,szTmp);
    if (strlen(szHome) == 0)
    {
        strcpy(szHome,".");
    }
    else
    {
        //��\�滻��/
        for (int i = 0 ; i < strlen(szHome) ; i ++)
            if (szHome[i] == '\\') szHome[i] = '/';
    }
    char szLogPath[100] = "";
    sprintf(szLogPath,"%s/log",szHome);
    return std::string(szLogPath);
}
