// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>

#ifdef PLUGIN_EXPORTS
#define PLUGIN_API extern "C" __declspec(dllexport)
#else
#define PLUGIN_API extern "C" __declspec(dllimport)
#endif

// TODO: may need add your path of LoadRunner 
// to the project configuration, if you want to use
// the LoadRunner C APIs
// #include <lrun.h>
// #pragma comment(lib, "lrun50.lib")