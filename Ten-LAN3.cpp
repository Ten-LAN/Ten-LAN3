#include "TenLAN.h"

class TenLANSystem *tenlan;

class TenLANSystem *GameView::system = NULL;

extern class GameView *GetTenLANBackGround( int type );
extern class GameView *GetTenLANStartUp( int type );

// GetTenLANUI.cpp �ɂ��낢�돑���Ă邩�猩�āB

void SystemInit( void )
{
	tenlan = new TenLANSystem();
	// Pad2Key��L�������ăo�b�N�O���E���h�œ��͂��󂯎���悤�ɂ���B
	MikanInput->SetPad2KeyInput( 1 );
}

void UserInit( void )
{
	tenlan->Init();
	tenlan->SetBackGround( GetTenLANBackGround( 0 ) );
	tenlan->SetGameView( GetTenLANStartUp( 0 ) );
}

int MainLoop( void )
{

	tenlan->PlayGameView();

	MikanDraw->DrawTextureScaling( 0,
		0, tenlan->GetHeight() - 4,
		1020, 1020, 4, 4,
		tenlan->GetWidth() * MikanInput->GetKeyNum( K_ESC ) / tenlan->GetExitTime(), 8, 0 );

	if(MikanInput->GetKeyNum( K_R ) == 1)
	{
		// ���������DX���C�u�������Q�[���őS��ʉ����Ȃ�
		//_MikanWindow->SetWindow( WT_NORESIZEFULLSCREEN );
		_MikanDraw->RecoverFromDeviceLost(1);
	}
	return ( MikanInput->GetKeyNum( K_ESC ) > tenlan->GetExitTime() );
}

void CleanUp( void )
{
	delete( tenlan );
}

