// copydir.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <direct.h>
#include <string.h>
#include <io.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <process.h>
#include <windows.h>
using namespace std;
#define BUF_SIZE 256
int copyFile(const char * pSrc, const char *pDes)
{
	FILE *in_file, *out_file;
	char data[BUF_SIZE];
	size_t bytes_in, bytes_out;
	long len = 0;
	if ((in_file = fopen(pSrc, "rb")) == NULL)
	{
		perror(pSrc);
		return -2;
	}
	if ((out_file = fopen(pDes, "wb")) == NULL)
	{
		perror(pDes);
		return -3;
	}
	while ((bytes_in = fread(data, 1, BUF_SIZE, in_file)) > 0)
	{
		bytes_out = fwrite(data, 1, bytes_in, out_file);
		if (bytes_in != bytes_out)
		{
			perror("Fatal write error.\n");
			return -4;
		}
		len += bytes_out;
		//printf("copying file .... %d bytes copy\n", len);
	}
	fclose(in_file);
	fclose(out_file);
	printf("修复文件：%s 完成\n", pDes);
	return 1;
}


int copyDir(const char * pSrc, const char *pDes)
{
	if (NULL == pSrc || NULL == pDes)	return -1;
	_mkdir(pDes);
	char dir[MAX_PATH] = { 0 };
	char srcFileName[MAX_PATH] = { 0 };
	char desFileName[MAX_PATH] = { 0 };
	char *str = "\\*.*";
	int filecount = 0;                  //复制的文件数量
	strcpy(dir, pSrc);
	strcat(dir, str);
	//首先查找dir中符合要求的文件
	long hFile;
	_finddata_t fileinfo;
	if ((hFile = _findfirst(dir, &fileinfo)) != -1)
	{
		do
		{
			strcpy(srcFileName, pSrc);
			strcat(srcFileName, "\\");
			strcat(srcFileName, fileinfo.name);
			strcpy(desFileName, pDes);
			strcat(desFileName, "\\");
			strcat(desFileName, fileinfo.name);
			//检查是不是目录
			//如果不是目录,则进行处理文件夹下面的文件
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				copyFile(srcFileName, desFileName);
				filecount++;
			}
			else//处理目录，递归调用
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					copyDir(srcFileName, desFileName);
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
		return filecount;
	}
	return -3;
}
int main(int argc, char* argv[])
{
	//std::cout $amp;
	printf("**********************开始自动修复*************************\n");
	int count = copyDir(argv[1], argv[2]);
	if (count > 0)
	{
		printf("**************自动修复完成,复制文件总数：%d****************\n", count);
		printf("*********************请重新启动设备************************\n");
		printf("****************如仍存问题，请使用还原u盘******************\n");

	}
	else if (count == -3)
	{
		printf("**************自动修复失败,还原文件损坏****************\n");
	}
	else
	{
		printf("***********************自动修复失败**********************\n");
	}
	


	system("pause");
	//std::cout $amp;
	return 0;
}



