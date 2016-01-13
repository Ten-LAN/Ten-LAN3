#include "TenLAN.h"
#include "DefaultGameView.h"

extern class GameView *GetTenLANUI( int type );

unsigned long CATE_COLOR[ CATEGORY_MAX ] =
{
	0x00ff2828, // アクション
	0x00ff0091, // シューティング
	0x00dbae00, // スポーツ
	0x00c2cb10, // レース
	0x0016f000, // パズル
	0x0000a411, // テーブル
	0x00008f9a, // シミュレーション
	0x00004eff, // ロールプレイング
	0x00cf00ff, // 対戦
	0x00f400ff, // ミニゲーム
	0x00808080, // その他
};

unsigned long CATE_DCOLOR[ CATEGORY_MAX ] =
{
	0x00db4c4c, // アクション
	0x00d52a8a, // シューティング
	0x00b69824, // スポーツ
	0x00b7bf1c, // レース
	0x0034bb25, // パズル
	0x001b8927, // テーブル
	0x001a7980, // シミュレーション
	0x002a5ed5, // ロールプレイング
	0x00a93fc1, // 対戦
	0x00bb3ec1, // ミニゲーム
	0x00575757, // その他
};

char CATE_NAME[ CATEGORY_MAX ][ 32 ] =
{
	"アクション",
	"シューティング",
	"スポーツ",
	"レース",
	"パズル",
	"テーブル",
	"シミュレーション",
	"ロールプレイング",
	"対戦",
	"ミニゲーム",
	"その他",
};

int CATE_SORT[ CATEGORY_MAX ] =
{
	0,  // アクション
	1,  // シューティング
	4,  // パズル
	6,  // シミュレーション
	// リズム
	3,  // レース
	7,  // ロールプレイング
	5,  // テーブル
	2,  // スポーツ
	8,  // 対戦
	9,  // ミニゲーム
	10, // その他
};

int TenLAN_Loading_Thread_Funciton( void *arg )
{
	class StartUp *view = ( class StartUp * )arg;
	return view->Loading();
}

int StartUp::Loading( void )
{
	gamemax = system->InitGamelist( gamemax, &loadgame );

	return 0;
}

void StartUp::UserInit( void )
{
	MikanDraw->LoadFontFile( NULL, "SYSFONT" );
	MikanDraw->CreateFont( 0, "MigMix 1M", 40, 0xFFFFFFFF );
	MikanDraw->CreateTexture( 0, NULL, "SYSIMG", TRC_NONE );

	gamemax = system->GetGamedirDirectorys();
	loadgame = 0;
	timer = -1;

	MikanSystem->RunThread( THREAD_LOADING, TenLAN_Loading_Thread_Funciton, ( void * )this );
	rad = 0;
	int mode = 0;
}

void StartUp::Resume( void )
{
}

int StartUp::MainLoop( void )
{
	int atimer = 30, time;
	double rad_ = ( MIKAN_PI*2.0*rad / 360 );

	MikanDraw->ClearScreen();

	if(loadgame < gamemax)
	{
		//MikanDraw->DrawTextureScalingC( 0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 160, 320, 160, 2.0 );
		MikanDraw->DrawTextureRotateScaleC( 0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 390, 160, 110, 110, 2.0, rad_ ); // 歯車
		MikanDraw->DrawTextureScalingC( 0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 160, 320, 160, 2.0 );
		MikanDraw->DrawTextureRotateScaleC( 0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 320, 160, 70, 70, 2.0, rad_ ); // e

		DrawGameState();
		rad = ( rad + 6 ) % 360;
	} else if(timer < 0)
	{
		//MikanDraw->DrawTextureScalingC( 0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 0, 320, 160, 2.0 );
		rad = ( rad + 6 ) % 360;
		MikanDraw->DrawTextureRotateScaleC( 0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 390, 160, 110, 110, 2.0, rad_ ); // 歯車
		MikanDraw->DrawTextureScalingC( 0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 160, 320, 160, 2.0 );
		MikanDraw->DrawTextureRotateScaleC( 0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 320, 160, 70, 70, 2.0, rad_ ); // e
		timer = 0;
		DrawGameState();
	} else if(timer >= 0)
	{
		//MikanSystem->Join( 0 );

		if(rad != 0)
		{
			DrawGameState();
			rad = ( rad + 6 ) % 360;
			MikanDraw->DrawTextureRotateScaleC( 0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 390, 160, 110, 110, 2.0, rad_ ); // 歯車
			MikanDraw->DrawTextureScalingC( 0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 160, 320, 160, 2.0 );
			MikanDraw->DrawTextureRotateScaleC( 0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 320, 160, 70, 70, 2.0, rad_ ); // e
		} else
		{
			if(mode == 1)
			{
				if(timer < atimer)
				{
					MikanDraw->DrawTextureScalingC( 0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 0, 320, 160, 2.0 );
					MikanDraw->SetAlpha( 0, 255 * timer / atimer );
					MikanDraw->DrawTextureScalingC( 0, system->GetWidth() / 2, system->GetHeight() / 2, 320, 0, 320, 160, 2.0 );
					MikanDraw->SetColor( 0, 0xFFFFFFFF );
				} else if(timer < atimer * 2)
				{
					time = timer - atimer;
					MikanDraw->SetAlpha( 0, 255 * ( atimer - time ) / atimer );
					MikanDraw->DrawTextureScalingC( 0, system->GetWidth() / 2, system->GetHeight() / 2, 320, 0, 320, 160, 2.0 );
					MikanDraw->SetColor( 0, 0xFFFFFFFF );
				} else
				{
					system->SetGameView( new OP() );
				}
				++timer;
			} else if(MikanInput->GetKeyNum( K_Z ) == -1)
			{
				mode = 1;
			} else
			{
				DrawGameState();
				MikanDraw->DrawTextureRotateScaleC( 0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 390, 160, 110, 110, 2.0, 0 ); // 歯車
				MikanDraw->DrawTextureScalingC( 0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 160, 320, 160, 2.0 );
				MikanDraw->DrawTextureRotateScaleC( 0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 320, 160, 70, 70, 2.0, 0 ); // e
			}
		}

	} else
	{
		//MikanDraw->DrawTextureScalingC( 0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 0, 320, 160, 2.0 );
		MikanDraw->DrawTextureRotateScaleC( 0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 390, 160, 110, 110, 2.0, rad_ ); // 歯車
		MikanDraw->DrawTextureScalingC( 0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 160, 320, 160, 2.0 );
		MikanDraw->DrawTextureRotateScaleC( 0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 320, 160, 70, 70, 2.0, rad_ ); // e
		DrawGameState();
		rad = ( rad + 6 ) % 360;
	}

	return 0;
}

int StartUp::DrawGameState( void )
{
	int i, max;

	max = loadgame;

	for(i = 0 ; i < max; ++i)
	{
		MikanDraw->Printf( 0, 0, i * 80, "[%03d/%03d] - %sSS[%d]", i, gamemax, system->GetGameTitle( i ), system->GetGameSSTexture( i ) );
	}

	return 0;
}

void StartUp::CleanUp( void )
{
	//MikanDraw->ReleaseTexture( 0 );
}

// BackGround

void BackGround::UserInit( void ){}
int timer = 0;
int BackGround::MainLoop( void )
{
	MikanDraw->ClearScreen();
	//MikanDraw->Printf(0,0,0,"%d,%d,%d",++timer,MikanInput->GetKeyNum(K_X),MikanInput->GetPadNum(0,PAD_C));

	system->ESCCannon();
	/*if (MikanInput->GetPadNum(0, PAD_I) > 1 && MikanInput->GetPadNum(0, PAD_J) > 1)
	{
	MikanDraw->Printf(0,0,0,"push");
	_MikanInput->SendKey( K_ESC, 1 );
	//Sleep( 10 );
	//_MikanInput->SendKey(K_ESC, 0);
	}*/

	return 0;
}

void BackGround::CleanUp( void )
{

}

// OP

void OP::UserInit( void )
{
	timer = 0;
	//tex = system->AddTexture( "./material/op.png" );
#ifndef _DEBUG
	//if (system->GetMode())
	//{
	MikanSound->Load( 0, NULL, "MAINTHEME" );
	MikanSound->Play( 0, true );
	MikanSound->SetVolume( 0, 80 );
	//}
#endif
}

void OP::Resume( void )
{
}

int OP::MainLoop( void )
{
	MikanDraw->ClearScreen();

	//MikanDraw->DrawTextureScalingC( tex, system->GetWidth() / 2, system->GetHeight() / 2, 0, 0, 512, 512, 1.0 );
	MikanDraw->Printf( 0, 0, 0, "キーテスト\n↑(UP)%-4d ↓(DOWN)%-4d ←(LEFT)%-4d →(RIGHT)%-4d\n○(Z)%-4d Ｘ(X)%-4d △(A)%-4d □(S)%-4d\nL1(D)%-4d R1(C)%-4d",
		MikanInput->GetKeyNum( K_UP ), MikanInput->GetKeyNum( K_DOWN ), MikanInput->GetKeyNum( K_LEFT ), MikanInput->GetKeyNum( K_RIGHT ),
		MikanInput->GetKeyNum( K_Z ), MikanInput->GetKeyNum( K_X ), MikanInput->GetKeyNum( K_A ), MikanInput->GetKeyNum( K_S ),
		MikanInput->GetKeyNum( K_D ), MikanInput->GetKeyNum( K_C )
		);
	if(system->GetGameMax() > 0)
	{
		if(MikanInput->GetKeyNum( K_Z ) == -1)
		{
			system->SetGameView( GetTenLANUI( system->GetConfig()->GetUIType() ) );
		}
	} else
	{
		// 起動できるゲームがない。
		MikanDraw->Printf( 0, system->GetWidth() / 2 - 40 * 17 / 2, system->GetHeight() / 2, "起動できるゲームがありません。\nESC長押しで終了。" );
	}
	return 0;
}

void OP::CleanUp( void )
{
}
