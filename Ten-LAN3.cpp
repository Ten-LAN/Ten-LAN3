#include "TenLAN.h"

class TenLANSystem *tenlan;

class TenLANSystem *GameView::system = NULL;

void SystemInit( void )
{
	tenlan = new TenLANSystem();
	// Pad2Key��L�������ăo�b�N�O���E���h�œ��͂��󂯎���悤�ɂ���B
	MikanInput->SetPad2KeyInput( 1 );
}

void UserInit( void )
{
	tenlan->Init();
	tenlan->SetBackGround( new BackGround() );
	tenlan->SetGameView( new StartUp() );
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
		_MikanWindow->SetWindow( WT_NORESIZEFULLSCREEN );
		_MikanDraw->RecoverFromDeviceLost( 1 );
	}
	return ( MikanInput->GetKeyNum( K_ESC ) > tenlan->GetExitTime() );
}

void CleanUp( void )
{
	delete( tenlan );
}

