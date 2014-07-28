// MyShellNormal.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#define STR_LEN 256
#define CMD_TOKEN_NUM 10
#define DIR_LEN MAX_PATH+1


int CmdProcessing( void );
TCHAR* StrLower( TCHAR* pStr );


TCHAR ERROR_CMD[] = _T( "'%s'��(��) ������ �� �ִ� ���α׷��� �ƴմϴ�. \n " );
TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T( " ,\t\n" );

int _tmain( int argc, _TCHAR* argv[] )
{

	_tsetlocale( LC_ALL, _T( "Korean" ) );

	DWORD isExit;
	while ( true )
	{
		isExit = CmdProcessing();
		if ( TRUE == isExit )
		{
			_fputts( _T( "��ɾ� ó���� �����մϴ�. \n " ), stdout );
			break;
		}
	}

	return 0;
}


int CmdProcessing( void )
{
	_fputts( _T( "Best command prompt>> " ), stdout );
	_getts( cmdString );

	TCHAR* token = _tcstok( cmdString, seps );

	int tokenNum = 0;
	while ( token != NULL )
	{
		_tcscpy( cmdTokenList[tokenNum++], StrLower( token ) );
		token = _tcstok( NULL, seps );
	}

	if ( !_tcscmp( cmdTokenList[0], _T( "exit" ) ) )
	{
		return TRUE;
	}
	else if ( !_tcscmp( cmdTokenList[0], _T( "pwd" ) ) )
	{
		TCHAR cDir[DIR_LEN];
		GetCurrentDirectory( DIR_LEN, cDir );
		_fputts( cDir, stdout );
		_fputts( _T( "\n" ), stdout );
	}
	else if ( !_tcscmp( cmdTokenList[0], _T( "�߰��Ǵ� ��ɾ� 2" ) ) )
	{

	}
	else
	{
		STARTUPINFO si = { 0, };
		PROCESS_INFORMATION pi;
		si.cb = sizeof( si );
		BOOL isRun = CreateProcess( NULL, cmdTokenList[0], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi );
		if ( FALSE == isRun )
		{
			_tprintf( ERROR_CMD, cmdTokenList[0] );
		}

	}

	return 0;

}


TCHAR* StrLower( TCHAR* pStr )
{
	TCHAR* ret = pStr;

	while ( *pStr )
	{
		if ( _istupper( *pStr ) )
		{
			*pStr = _totlower( *pStr );
		}
		pStr++;
	}

	return ret;

}

