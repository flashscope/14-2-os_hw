// MyShellNormal.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#define STR_LEN 256
#define CMD_TOKEN_NUM 10
#define DIR_LEN MAX_PATH+1





TCHAR ERROR_CMD[] = _T( "'%s'은(는) 실행할 수 있는 프로그램이 아닙니다. \n " );
TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T( " ,\t\n" );


int CmdReadTokenize( void );
int CmdProcessing( int tokenNum );
TCHAR* StrLower( TCHAR* pStr );



int _tmain( int argc, _TCHAR* argv[] )
{

	_tsetlocale( LC_ALL, _T( "Korean" ) );

	if ( argc > 2 )
	{
		for ( int i = 1; i < argc; ++i )
		{
			_tcscpy( cmdTokenList[i - 1], argv[i] );
		}
		CmdProcessing( argc - 1 );
	}


	DWORD isExit = NULL;
	while ( true )
	{
		int tokenNum = CmdReadTokenize();

		if ( tokenNum == 0 ) // for enter input
		{
			continue;
		}

		isExit = CmdProcessing(tokenNum);

		if ( TRUE == isExit )
		{
			_fputts( _T( "명령어 처리를 종료합니다. \n " ), stdout );
			break;
		}
	}

	return 0;
}

int CmdReadTokenize( void )
{

	TCHAR* token;

	_fputts( _T( "Best command prompt >> " ), stdout );
	_getts( cmdString );

	token = _tcstok( cmdString, seps );

	int tokenNum = 0;

	while ( token != NULL )
	{
		_tcscpy( cmdTokenList[tokenNum++], StrLower( token ) );
		token = _tcstok( NULL, seps );
	}

	return tokenNum;

}


int CmdProcessing( int tokenNum )
{
	BOOL isRun;
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;

	TCHAR cmdStringWithOptions[STR_LEN] = { 0, };
	TCHAR optString[STR_LEN] = { 0, };


	si.cb = sizeof( si );


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
	else if ( !_tcscmp( cmdTokenList[0], _T( "start" ) ) )
	{
		if ( tokenNum > 1 )
		{
			// combine all tokens... without start
			for ( int i = 1; i < tokenNum; ++i )
			{
				_stprintf( optString, _T( "%s %s" ), optString, cmdTokenList[i] );
			}
			_stprintf( cmdStringWithOptions, _T( "%s %s" ), _T( "MyShellNormal.exe" ), optString );
		}
		else // only write start
		{
			_stprintf( cmdStringWithOptions, _T( "%s" ), _T( "MyShellNormal.exe" ) );
		}

		isRun = CreateProcess( NULL, cmdStringWithOptions, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi );

		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
		
	}
	else if ( !_tcscmp( cmdTokenList[0], _T( "echo" ) ) )
	{
		for ( int i = 1; i < tokenNum; ++i )
		{
			_stprintf( optString, _T( "%s %s" ), optString, cmdTokenList[i] );
		}

		_tprintf( _T( "echo message:%s \n" ), optString );
	}
	else
	{
		_tcscpy( cmdStringWithOptions, cmdTokenList[0] );

		for ( int i = 1; i < tokenNum; ++i )
		{
			_stprintf( cmdStringWithOptions, _T( "%s %s" ), cmdStringWithOptions, cmdTokenList[i] );
		}

		isRun = CreateProcess( NULL, cmdStringWithOptions, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi );

		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );

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

