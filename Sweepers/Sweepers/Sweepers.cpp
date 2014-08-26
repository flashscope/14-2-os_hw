// Sweepers.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <Windows.h>
#include <process.h>

#define NUM_PLAYER 3

struct Player
{
	int mPlayerId;
	int mHP;
	CRITICAL_SECTION mLock;
};

Player GamerData[NUM_PLAYER];

void attack( Player* from, Player* to )
{
	_tprintf( _T( "BEGIN: from #%d to #%d \n" ), from->mPlayerId, to->mPlayerId );

	if ( from->mPlayerId > to->mPlayerId )
	{
		EnterCriticalSection( &from->mLock );
		_tprintf( _T( "--from #%d enter critical section \n" ), from->mPlayerId );

		EnterCriticalSection( &to->mLock );
		_tprintf( _T( "--to #%d enter critical section \n" ), to->mPlayerId );
	}
	else
	{
		EnterCriticalSection( &to->mLock );
		_tprintf( _T( "--to #%d enter critical section \n" ), to->mPlayerId );

		EnterCriticalSection( &from->mLock );
		_tprintf( _T( "--from #%d enter critical section \n" ), from->mPlayerId );
	}


	to->mHP -= 100;
	from->mHP += 70;

	Sleep( 100 );

	if ( from->mPlayerId > to->mPlayerId )
	{
		LeaveCriticalSection( &to->mLock );
		_tprintf( _T( "--from #%d leave critical section \n" ), from->mPlayerId );
		LeaveCriticalSection( &from->mLock );
		_tprintf( _T( "--to #%d leave critical section \n" ), from->mPlayerId );
	}
	else
	{
		LeaveCriticalSection( &from->mLock );
		_tprintf( _T( "--to #%d leave critical section \n" ), from->mPlayerId );
		LeaveCriticalSection( &to->mLock );
		_tprintf( _T( "--from #%d leave critical section \n" ), from->mPlayerId );

	}

}

unsigned int WINAPI ThreadProc( LPVOID lpParam )
{
	srand( GetCurrentThreadId() );

	int from = (int)lpParam;

	// from player�� random���� ��� to player�� ���Ͽ� ����
	for ( int i = 0; i < 10; ++i )
	{
		int to = 0;

		while ( true )
		{
			to = rand() % NUM_PLAYER;
			if ( from != to )
			{
				break;
			}
		}

		attack( &GamerData[from], &GamerData[to] );
	}

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{

	DWORD dwThreadId[NUM_PLAYER];
	HANDLE hThread[NUM_PLAYER];

	// HP�ʱ�ȭ
	for ( int i = 0; i < NUM_PLAYER; ++i )
	{
		GamerData[i].mPlayerId = i + 1001;
		GamerData[i].mHP = 2000;
		InitializeCriticalSection( &( GamerData[i].mLock ) );
	}


	for ( int i = 0; i < NUM_PLAYER; ++i )
	{
		hThread[i] = (HANDLE)_beginthreadex( NULL, 0, ThreadProc, (LPVOID)i, CREATE_SUSPENDED, (unsigned int*)&dwThreadId[i] );
	}


	_tprintf( _T("===== BEGIN ===== \n" ) );
	for ( int i = 0; i < NUM_PLAYER; ++i )
	{
		ResumeThread( hThread[i] );
	}

	WaitForMultipleObjects( NUM_PLAYER, hThread, TRUE, INFINITE );


	_tprintf( _T( "===== RESULT ===== \n" ) );
	for ( int i = 0; i < NUM_PLAYER; ++i )
	{
		_tprintf( _T( "--- Player #%d, HP: %d \n" ), GamerData[i].mPlayerId, GamerData[i].mHP );
		DeleteCriticalSection( &(GamerData[i].mLock) );
	}


	for ( int i = 0; i < NUM_PLAYER; ++i )
	{
		CloseHandle( hThread[i] );
	}



	getchar();

	return 0;
}

