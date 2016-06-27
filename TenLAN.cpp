#include "TenLAN.h"
#include <time.h>

const char LOGFILE[] = "gamelog.txt";
const char STATISTICS_H[] = "statistics";
const char STATISTICS_T[] = ".txt";

unsigned int atou( const char *str )
{
	unsigned int ret = 0;
	while(*str != '\0')
	{
		if(*str < '0' && '9' < *str)
		{
			break;
		}
		ret = ret * 10 + *str - '0';
		++str;
	}
	return ret;
}

unsigned int nendo( unsigned int date )
{
	if(date < 10000)
	{
		return 0;
	}
	return date - date % 10000 - ( date % 10000 < 400 ); // もし新しい年でも4月になってなかったら-1年する。
}

TenLANSystem::TenLANSystem( void )
{
	//ウィンドウハンドル
	HWND myhwnd = NULL;
	//RECT構造体
	RECT myrect;

	mode = TENLAN_PLAY;

	now = next = background = NULL;
	GameView::system = this;
	gamewindow = NULL;

	gamedata = NULL;

	texmax = 1;

	categorymax = 0;

	//volumemax = 100;

	config = new Config();
	key = config->GetKeyClass();

	config->LoadConfig( "config.ini" );

	SetDebug(false);

	GetCurrentDirectory( 512, curdir );

	if(config->GetMode())
	{
		//デスクトップのウィンドウハンドル取得
		myhwnd = GetDesktopWindow();

		if(myhwnd && GetClientRect( myhwnd, &myrect ))
		{
			width = myrect.right;
			height = myrect.bottom;
		}
		_MikanWindow->SetWindow( WT_NORESIZEFULLSCREEN );
	} else
	{
		width = 800;
		height = 450;
		MikanWindow->SetWindowSize( width, height );
	}

	SetEnvironmentVariable( "TENLAN", "1" );

	MikanWindow->SetWindowName( "Ten-LAN" );
	MikanWindow->SetWindowIcon( "GAME_ICON" );
	MikanSystem->SetInactiveWindow( 0 );
}

TenLANSystem::~TenLANSystem( void )
{
	// タスクバーの復旧。
	RestoreTaskbar();

	if(now)
	{
		now->CleanUp();
		delete( now );
	}
	if(next)
	{
		delete( next );
	}
	if(background)
	{
		background->CleanUp();
		delete( background );
	}

	delete( config );

	free( gamedata );
}

int TenLANSystem::Init( const char *file )
{
	// Ten-LANに対するSetPad2Key
	key->SetPad2Key( 1 );
	// Pad2Key使用時に再度デバイスを作りなおすことで、バックグラウンドでも入力を受け付けるようにする。
	//_MikanInput->RecognitionGamepad( 0 );

	// ウィンドウを最前面に持ってくる。
	SetForegroundWindow( MikanWindow->GetWindowHandle() );

	if(config->GetMode())
	{
		// マウスカーソルを隠す。
		ShowCursor( 0 );
	}
	if(config->IsHideTaskbar())
	{
		// タスクバーを隠す。
		HideTaskbar();
	}

	return 0;
}

int AddPlayGame( void *arg )
{
	char *ad = (char *)arg;
	MikanNet->HttpGet( ad, "tmp.txt" );
	free( ad );
	return 0;
}

int TenLANSystem::AddLog( unsigned int gamenum, unsigned long playtime )
{
	char *address;
	unsigned int size;
	// ゲームの起動ログやカウントアップ要請。
	time_t now = time( NULL );

	// デバッグモードなら保存をしない
	if (IsDebug())
	{
		return 0;
	}

	if(MikanFile->Open( 0, LOGFILE, "a+" ) >= 0)
	{
		MikanFile->Printf( 0, "%lld\t%d\t%s\t%lu\t\n", now, gamenum, gamedata[ gamenum ].title, playtime );
		MikanFile->Close( 0 );
	}

	if(config->GetLogCGIURL())
	{
		// URLあるらしい。

		// URLの作成。TODO:
		size = strlen( config->GetLogCGIURL() ) + 30 + 1;
		address = (char *)calloc( size, sizeof( char ) );
		sprintf_s( address, size, "%s?%d&%d", config->GetLogCGIURL(), gamenum, playtime );

		MikanSystem->RunThread( THREAD_ADDLOG, AddPlayGame, (void *)address );
	}

	SaveStatistics(gamenum, playtime);

	return 0;
}

// 統計を読み込む関数
int TenLANSystem::LoadStatistics()
{
	time_t now;
	struct tm t;
	FILE* file;
	char stfile[512];

	// 今日の日付を取得
	time(&now);
	localtime_s(&t, &now);
	date[0] = t.tm_mon + 1;
	date[1] = t.tm_mday;
	date[2] = t.tm_year + 1900;

	// 統計の合計を読み込み

	// 統計の合計のファイル名を生成
	sprintf_s(stfile, 512, "%s_total%s", STATISTICS_H, STATISTICS_T);

	if (fopen_s(&file, stfile, "r") == 0)
	{
		int check;
		while (1)
		{
			int number;// ゲーム識別番号用
			int pnum;// プレイ回数用
			int total_h = 0, total_m = 0, total_s = 0;// 総プレイ時間用
			int ave_m = 0, ave_s = 0;// 平均時間用
			int score = 0; double ave_score = 0;// 評点用

			char title[2048];// タイトル用

			// 番号 ゲーム名 プレイ回数 総プレイ時間(時間、分、秒) 平均プレイ時間(分、秒)
			// 合計評点 平均評点
			check = fscanf_s(file, "%d %s %d回 %d:%d:%d %d:%d %d %lf",
				&number, title, sizeof(title), &pnum, &total_h, &total_m, &total_s,
				&ave_m, &ave_s, &score, &ave_score);

			// 読み込みが終われば終了
			if (check == EOF)
			{
				break;
			}

			// 読み込んだゲームにその番号のものを探してデータ代入
			for (unsigned int n = 0; n < gamemax; n++)
			{
				if (gamedata[n].num == number)
				{
					gamedata[n].total.playnum = pnum;
					gamedata[n].total.playtime = total_h * 3600 + total_m * 60 + total_s;
					gamedata[n].total.score = score;

					break;
				}
			}
		}
		fclose(file);
	}

	// 今日の統計ファイル名を生成
	sprintf_s(stfile, 512, "%s%d_%02d_%04d%s",
		STATISTICS_H, date[0], date[1], date[2], STATISTICS_T);

	// 次にこの日の統計を読み込み
	// 統計データを保存したファイルを開く
	if (fopen_s(&file, stfile, "r") == 0)
	{
		int check;
		while (1)
		{
			int number;// ゲーム識別番号用
			int pnum;// プレイ回数用
			int total_h = 0, total_m = 0, total_s = 0;// 総プレイ時間用
			int ave_m = 0, ave_s = 0;// 平均時間用
			int score = 0; double ave_score = 0;// 評点

			char title[2048];// タイトル用

			// 番号 ゲーム名 プレイ回数 総プレイ時間(時間、分、秒) 平均プレイ時間(分、秒)
			// 合計評点 平均評点
			check = fscanf_s(file, "%d %s %d回 %d:%d:%d %d:%d %d %lf",
				&number, title, sizeof(title), &pnum, &total_h, &total_m, &total_s,
				&ave_m, &ave_s, &score, &ave_score);

			// 読み込みが終われば終了
			if (check == EOF)
			{
				break;
			}

			// 読み込んだゲームにその番号のものを探してデータ代入
			for (unsigned int n = 0; n < gamemax; n++)
			{
				if (gamedata[n].num == number)
				{
					gamedata[n].today.playnum = pnum;
					gamedata[n].today.playtime = total_h * 3600 + total_m * 60 + total_s;
					gamedata[n].today.score = score;

					break;
				}
			}
		}
		fclose(file);
	}

	return 1;
}

// 統計を保存する関数
int TenLANSystem::SaveStatistics(unsigned int gamenum, unsigned long playtime)
{
	// デバッグモードなら終了
	if (IsDebug())
	{
		return 0;
	}

	time_t now;
	struct tm t;
	FILE* file;
	char stfile[512];
	int curdate[3];

	// 現在の日付を取得
	time(&now);
	localtime_s(&t, &now);
	curdate[0] = t.tm_mon + 1;
	curdate[1] = t.tm_mday;
	curdate[2] = t.tm_year + 1900;

	// ゲームの統計データの更新

	// まず日付が変わっていたら今日の統計をリセット
	for (int i = 0; i < 3; i++)
	{
		if (date[i] != curdate[i])
		{
			// 日付修正
			for (int j = i; j < 3; j++)
			{
				date[j] = curdate[j];
			}
			// ゲームの今日の統計をリセット
			for (unsigned int n = 0; n < gamemax; n++)
			{
				gamedata[n].today.playnum = 0;
				gamedata[n].today.playtime = 0;
				gamedata[n].today.score = 0;
			}
		}
	}
	// 次に今プレイしたデータを加える
	gamedata[gamenum].total.playnum++;
	gamedata[gamenum].total.playtime += playtime;
	gamedata[gamenum].today.playnum++;
	gamedata[gamenum].today.playtime += playtime;

	// まずは統計の合計を保存

	// 統計の合計のファイル名を生成
	sprintf_s(stfile, 512, "%s_total%s", STATISTICS_H, STATISTICS_T);

	if (fopen_s(&file, stfile, "w") == 0)
	{
		for (unsigned int n = 0; n < gamemax; n++)
		{
			// 平均プレイ時間(秒)と平均評点
			int ave = 0; double ave_score = 0;
			if (gamedata[n].total.playnum > 0)
			{
				ave = gamedata[n].total.playtime / gamedata[n].total.playnum;
				ave_score = (double)(gamedata[n].total.score)
					/ (double)(gamedata[n].total.playnum);
			}

			// タイトル(改行コードや空白の修正)
			char title[2048];
			strcpy_s(title, sizeof(title), gamedata[n].title);
			for (int i = 0; title[i] != '\0'; i++)
			{
				if (title[i] == '\n')
				{
					title[i] = '\0';
					break;
				}

				if (title[i] == ' ')
				{
					title[i] = '_';
				}
			}

			// 番号 ゲーム名 プレイ回数 総プレイ時間(時間、分、秒) 平均プレイ時間(分、秒)
			// 合計評点 平均評点
			fprintf_s(file, "%d %s %03d回 %02d:%02d:%02d %02d:%02d %03d %1.3lf\n\0",
				gamedata[n].num, title, gamedata[n].total.playnum,
				gamedata[n].total.playtime / 3600,
				(gamedata[n].total.playtime % 3600) / 60,
				gamedata[n].total.playtime % 60,
				ave / 60, ave % 60, gamedata[n].total.score, ave_score);
		}

		fclose(file);
	}

	sprintf_s(stfile, 512, "%s%d_%02d_%04d%s", STATISTICS_H,
		date[0], date[1], date[2], STATISTICS_T);

	if (fopen_s(&file, stfile, "w") == 0)
	{
		for (unsigned int n = 0; n < gamemax; n++)
		{
			// 平均プレイ時間(秒)
			int ave = 0; double ave_score = 0;
			if (gamedata[n].today.playnum > 0)
			{
				ave = gamedata[n].today.playtime / gamedata[n].today.playnum;
				ave_score = (double)(gamedata[n].today.score)
					/ (double)(gamedata[n].today.playnum);
			}

			// タイトル(改行コードや空白の修正)
			char title[2048];
			strcpy_s(title, sizeof(title), gamedata[n].title);
			for (int i = 0; title[i] != '\0'; i++)
			{
				if (title[i] == '\n')
				{
					title[i] = '\0';
					break;
				}

				if (title[i] == ' ')
				{
					title[i] = '_';
				}
			}

			// 番号 ゲーム名 プレイ回数 総プレイ時間(時間、分、秒) 平均プレイ時間(分、秒)
			// 合計評点 平均評点
			fprintf_s(file, "%d %s %03d回 %02d:%02d:%02d %02d:%02d %03d %1.3lf\n\0",
				gamedata[n].num, title, gamedata[n].today.playnum,
				gamedata[n].today.playtime / 3600,
				(gamedata[n].today.playtime % 3600) / 60,
				gamedata[n].today.playtime % 60,
				ave / 60, ave % 60, gamedata[n].today.score, ave_score);
		}

		fclose(file);
	}

	return 1;
}

int TenLANSystem::SetGameView( class GameView *newgameview )
{
	if(now)
	{
		next = newgameview;
	} else
	{
		now = newgameview;
		now->UserInit();
	}
	return 0;
}

int TenLANSystem::PlayGameView( void )
{
	int ret = 0;
	switch(mode)
	{
	case TENLAN_PLAY: // 選択画面やOPなど。
		//デバッグモードの切り替え
		if (MikanInput->GetKeyNum(K_T) == 1 || MikanInput->GetPadNum(0,PAD_J) == 1)
		{
			SetDebug(!IsDebug());
		}
		ret = now->MainLoop();
		break;

	case TENLAN_WAIT: // ゲーム起動中。

		//if ( CheckEndProcess() )
		//{
		// ゲーム実行終了後の後片付けを行う。
		//  CleanupExecGame();

		// 復旧フェーズを走らせる。
		//  now->Resume();
		//}

		//if ( background ){ ret = background->MainLoop(); }
		
		do
		{
			//ゲームパッド入力の更新
			_MikanInput->UpdatePadInput();
			// ゲームパッドのPad2Keyの更新。
			_MikanInput->UpdatePad2KeyInput();
			//キー入力の更新
			_MikanInput->UpdateKeyInput();
			//マウス入力の更新
			_MikanInput->UpdateMouseInput();
			// 低レベル入力の更新。
			_MikanInput->UpdateRawInput();

			ESCCannon();

			SetForegroundWindow(gamewindow);
			

			MikanSystem->WaitNextFrame( 0 );
		} while(!CheckEndProcess());
		// ゲーム実行終了後の後片付けを行う。
		CleanupExecGame();

		// 復旧フェーズを走らせる。
		now->Resume();
		break;
	case TENLAN_NEXTPLAY:

		_MikanInput->ReleaseAllPad2Key();
		//_MikanInput->SendKey( K_ESC, 0 );// SendKeyは危ないので停止
		if(config->GetMode())
		{
			// これやるとDXライブラリ製の全画面にしないゲームでバグる
			// よってコメントアウトしてます
			//_MikanWindow->SetWindow( WT_NORESIZEFULLSCREEN );
		}
		_MikanDraw->RecoverFromDeviceLost( 1 );
		mode = TENLAN_PLAY;
		break;
	}

	NextGameView();
	return ret;
}

int TenLANSystem::NextGameView( void )
{
	if(next)
	{
		if(now)
		{
			now->CleanUp();
			delete( now );
		}
		now = next;
		next = NULL;
		if(now)
		{
			now->UserInit();
			now->Resume();
		}
	}
	return 0;
}

int TenLANSystem::SetBackGround( class GameView *background )
{
	if(this->background)
	{
		this->background->CleanUp();
		delete( this->background );
	}
	this->background = background;
	if(background)
	{
		background->UserInit();
		background->Resume();
	}
	return 0;
}

int TenLANSystem::InitGamelist( int gamemax, int *loadgame )
{
	HANDLE hdir;
	WIN32_FIND_DATA status;
	struct stat fstat;

	char filepath[ 1024 ] = "";
	char date[ 9 ] = "";
	char buf[ 2048 ], *tok, *str;
	int n = 0, msel = 0, r, w;
	int _loadgame = 0;

	newline = 0;

	for(r = 0; r < CATEGORY_MAX; ++r)//TODO:
	{
		usecate[ r ] = 0;
	}

	sprintf_s( filepath, 1024, "%s\\*", config->GetGameDirectory() );

	gamedata = ( struct GameData * )calloc( gamemax, sizeof( struct GameData ) );
	if(loadgame)
	{
		*loadgame = 0;
	}

	if(( hdir = FindFirstFile( filepath, &status ) ) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(( status.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ))
			{
				//ディレクトリ
				if(strcmp( status.cFileName, "." ) != 0 &&
					strcmp( status.cFileName, ".." ) != 0)
				{
					sprintf_s( filepath, 1024, "%s\\%s\\%s", config->GetGameDirectory(), status.cFileName, "data.ini" );
					stat( filepath, &fstat );
					if(MikanFile->Open( 0, filepath, "r" ) >= 0)
					{
						// 初期化
						strcpy_s( gamedata[ msel ].exe, GAMEDATA_EXE_LEN, "" );
						strcpy_s( gamedata[ msel ].title, GAMEDATA_TITLE_LEN, "NO TITLE" );
						strcpy_s( gamedata[ msel ].text, GAMEDATA_TEXT_LEN, "" );
						gamedata[ msel ].first = 0;
						gamedata[ msel ].date = 0;
						gamedata[ msel ].pad2key = 1;
						gamedata[ msel ].cnum = 0;
						gamedata[msel].category[0] = 0;
						gamedata[msel].imagenum = 1;
						gamedata[msel].minplnum = 1;

						// カテゴリー関係のデータを一時的に保存するため
						int category[CATEGORY_MAX];
						for (int i = 0; i < CATEGORY_MAX; i++)
						{
							category[i] = 0;
						}
						while(MikanFile->ReadLine( 0, buf, 512 ))
						{
							str = strtok_s( buf, "=", &tok );
							if(strcmp( str, "exe" ) == 0)
							{
								r = w = 0;
								while(tok[ r ] != '\0')
								{
									gamedata[ msel ].exe[ w++ ] = tok[ r ];
									if(tok[ r ] == '\n')
									{
										break;
									}
									++r;
								}
								gamedata[ msel ].exe[ w - 1 ] = '\0';
								++w;
							} else if(strcmp( str, "title" ) == 0)
							{
								strcpy_s( gamedata[ msel ].title, GAMEDATA_TITLE_LEN, strtok_s( NULL, "=", &tok ) );
								if(gamedata[ msel ].title[ strlen( gamedata[ msel ].title ) - 1 ] == '\n')
								{
									gamedata[ msel ].title[ strlen( gamedata[ msel ].title ) - 1 ] = '\0';
								}
							} else if(strcmp( str, "text" ) == 0)
							{
								strcpy_s( gamedata[ msel ].text, GAMEDATA_TEXT_LEN, strtok_s( NULL, "=", &tok ) );
								if(gamedata[ msel ].text[ strlen( gamedata[ msel ].text ) - 1 ] == '\n')
								{
									gamedata[ msel ].text[ strlen( gamedata[ msel ].text ) - 1 ] = '\0';
								}
							} else if(strcmp( str, "date" ) == 0)
							{
								strncpy_s( date, strtok_s( NULL, "=", &tok ), 8 );
								date[ 8 ] = '\0';
								gamedata[ msel ].date = atou( date );
							} else if(strcmp( str, "first" ) == 0)
							{
								strncpy_s( date, strtok_s( NULL, "=", &tok ), 8 );
								date[ 8 ] = '\0';
								gamedata[ msel ].first = atou( date );
								gamedata[ msel ].nendo = nendo( gamedata[ msel ].first );
								if(newline < gamedata[ msel ].first)
								{
									newline = gamedata[ msel ].first;
								}
							} else if(strcmp( str, "pad2key" ) == 0)
							{
								gamedata[ msel ].pad2key = atoi( strtok_s( NULL, "=", &tok ) );
							} else if(strcmp( str, "dvd" ) == 0)
							{
								gamedata[ msel ].dvd = atoi( strtok_s( NULL, "=", &tok ) );
							} else if(strcmp( str, "cate" ) == 0)
							{
								// カテゴリ番号を取得。
								w = 0;
								for(r = 0 ; tok[ r ] != '\0' ; ++r)
								{
									if('0' <= tok[ r ] && tok[ r ] <= '9')
									{
										gamedata[ msel ].category[ gamedata[ msel ].cnum ] = gamedata[ msel ].category[ gamedata[ msel ].cnum ] * 10 + tok[ r ] - '0';
									} else if(tok[ r ] == ',')
									{
										//++usecate[ gamedata[ msel ].category[ gamedata[ msel ].cnum ] ];
										++category[ gamedata[ msel ].category[ gamedata[ msel ].cnum ] ];
										++gamedata[ msel ].cnum;
									}
								}
								//++usecate[ gamedata[ msel ].category[ gamedata[ msel ].cnum ] ];
								++category[ gamedata[ msel ].category[ gamedata[ msel ].cnum ] ];
								++gamedata[ msel ].cnum;
							}
							else if (strcmp(str, "imagenum") == 0)
							{
								gamedata[msel].imagenum = atoi(strtok_s(NULL, "=", &tok));
							}
							else if (strcmp(str, "minplnum") == 0)
							{
								gamedata[msel].minplnum = atoi(strtok_s(NULL, "=", &tok));
							}
						}
						MikanFile->Close( 0 );

						gamedata[ msel ].num = atoi( status.cFileName );

						// 現在のパスを取得。
						GetCurrentDirectory( GAMEDATA_TEX_LEN, gamedata[ msel ].txfile );
						// テクスチャまでのパスを生成。
						sprintf_s( gamedata[ msel ].txfile, GAMEDATA_TEX_LEN, "%s\\%s\\%s\\%s", gamedata[ msel ].txfile, config->GetGameDirectory(), status.cFileName, "ss.png" );
						gamedata[ msel ].txnum = AddTexture( gamedata[ msel ].txfile );
						//MikanDraw->CreateTexture( gd[ msel ].txnum, gd[ msel ].txfile, TRC_NONE );
						// ゲームの統計データを初期化
						gamedata[ msel ].today.playnum = 0;
						gamedata[ msel ].today.playtime = 0;
						gamedata[ msel ].today.score = 0;
						gamedata[ msel ].total.playnum = 0;
						gamedata[ msel ].total.playtime = 0;
						gamedata[ msel ].total.score = 0;

						if (gamedata[msel].minplnum <= MikanInput->GetPadMount() ||
							MikanInput->GetPadMount() == 0)
						{
							++msel;

							// 最大ゲーム数。
							++_loadgame;
							if (loadgame)
							{
								*loadgame = _loadgame;
							}
							// カテゴリー
							for (int i = 0; i < CATEGORY_MAX; i++)
							{
								usecate[ i ] += category[ i ];
							}
						}
					}
				}
			}
		} while(FindNextFile( hdir, &status ));
		FindClose( hdir );

		this->gamemax = _loadgame;

		// 統計データの読み込み
		LoadStatistics();
	}

	// newlineを年度に変換したい。
	newline = nendo( newline );

	for(n = 0; n < CATEGORY_MAX; ++n)
	{
		if(usecate[ n ])
		{
			++categorymax;
		}
	}

	this->gamemax = _loadgame;
	return _loadgame;
}

int TenLANSystem::GetGamedirDirectorys( void )
{
	HANDLE hdir;
	WIN32_FIND_DATA status;
	struct stat fstat;

	char filepath[ 1024 ] = "";

	int count = 0;

	sprintf_s( filepath, 1024, "%s\\*", config->GetGameDirectory() );

	if(( hdir = FindFirstFile( filepath, &status ) ) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(( status.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ))
			{
				//ディレクトリ
				if(strcmp( status.cFileName, "." ) != 0 &&
					strcmp( status.cFileName, ".." ) != 0)
				{
					sprintf_s( filepath, 1024, "%s\\%s\\%s", config->GetGameDirectory(), status.cFileName, "data.ini" );
					stat( filepath, &fstat );
					if(fstat.st_mode & _S_IREAD)
					{
						++count;
					}
				}
			}
		} while(FindNextFile( hdir, &status ));
		FindClose( hdir );
	}

	return count;
}

int debugmode = 0;

int TenLANSystem::ExecGame( unsigned int gamenum )
{
	char *exe, dir[ 512 ];
	DWORD startpid, errcode;
	HWND hw;

	MikanSound->StopAll();

	// モード変更
	//mode = TENLAN_WAIT;

	execgamenum = gamenum;

	ZeroMemory( &( si ), sizeof( si ) );
	si.cb = sizeof( si );

	//sprintf_s( filepath, 2047, "%s\\%04d", GAMEDIR, gd[num].num );
	exe = gamedata[ gamenum ].exe + strlen( gamedata[ gamenum ].exe );
	while(*exe != '\\')
	{
		--exe;
	}

	strncpy_s( dir, 512, gamedata[ gamenum ].exe, strlen( gamedata[ gamenum ].exe ) - strlen( exe ) + 1 );

	if(SetCurrentDirectory( dir ))
	{
		// ディレクトリ移動成功。
		++exe;

		if(debugmode == 0)
		{
			// Ten-LANのPad2Keyを切る。
			key->SetPad2Key( 0 );
		}

		if(CreateProcess( NULL, exe, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &( si ), &( pi ) ))
		{
			//gamewindow = GetWindowHandle( pi.dwProcessId );
			CountUp( gamenum );

			// プロセスIDの取得。
			startpid = pi.dwProcessId;

			// タイムラグあるのでここでテクスチャ解放
			//ReleaseTexture();

			// ゲームのウィンドウハンドルが取得できるまで待機。
			do
			{
				hw = GetWindowHandle( startpid );
			} while(hw == NULL || hw == MikanWindow->GetWindowHandle());
			gamewindow = hw;

			SetWindowPos( MikanWindow->GetWindowHandle(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
			SetForegroundWindow( gamewindow );
			SetWindowPos( gamewindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

			//_MikanInput->AttachPad2Key( gamewindow, FALSE );
			_MikanInput->AttachPad2Key( gamewindow, 0 );

			if(debugmode)
			{
				_MikanInput->SetPOVMode( 0, 1 );
				// ウィンドウをアタッチする。
				//key->SetPad2Key(1);
				_MikanInput->AttachPad2Key( gamewindow, 1 );
				if(gamedata[ gamenum ].pad2key == 0)
				{
					key->InvalidKeyConfig( 0 );
				}
			} else
			{
				if(gamedata[ gamenum ].pad2key)
				{
					// ウィンドウをアタッチする。
					key->SetPad2Key( 1 );
					_MikanInput->AttachPad2Key( gamewindow, 1 );
				}
			}
			// ウィンドウを最前面に。
			//SetForegroundWindow( gamewindow );

			// モード変更
			mode = TENLAN_WAIT;

			beginexectime = timeGetTime();
		} else
		{
			// プロセス生成失敗。

			errcode = GetLastError();
			GetCurrentDirectory( 512, dir ); // TODO ????????

			//mode = TENLAN_PLAY;
			// モード変更
			mode = TENLAN_WAIT;
		}
	}

	return 0;
}

int TenLANSystem::CleanupExecGame( void )
{
	unsigned long playtime;
	// アタッチしたウィンドウのPad2Keyを解除。
	_MikanInput->SendKey( K_ESC, 0 );
	_MikanInput->ReleaseAllPad2Key();
	key->SetPad2Key( 0 );
	_MikanInput->AttachPad2Key( gamewindow, 0 );
	gamewindow = NULL;

	_MikanDraw->SetScreenSize( GetWidth(), GetHeight() );

	// TODO:Pad2Keyのバッファを開放と言うか、DownしてUpしてない事態を防ぐ。

	_MikanInput->UpdateKeyInput();
	// Ten-LANもPad2Key有効なので元に戻す。
	key->SetPad2Key( 1 );
	if(debugmode)
	{
		_MikanInput->SetPOVMode( 0, 0 );
	}

	//_MikanDraw->RecoverFromDeviceLost();

	// ログ。
	// プレイ時間を秒にする。
	playtime = ( timeGetTime() - beginexectime ) / 1000;
	if(config->GetLogCGIURL())
	{
		// 頑張ってデータ作る。
	}
	AddLog( execgamenum, playtime );

	mode = TENLAN_NEXTPLAY;

	execgamenum = -1;

	//MikanSound->Play( 0, true );
	//MikanSound->SetVolume( 0, 80 );


	return 0;
}

int active = 0;
int TenLANSystem::CheckEndProcess( void )
{
	unsigned long ExitCode;

	if(gamewindow)
	{
		GetExitCodeProcess( pi.hProcess, &ExitCode );

		if(ExitCode == STILL_ACTIVE)
		{
			if(active > 60)
			{
				//SetForegroundWindow(gamewindow);
				active = 0;
			}
			return 0;
		}
		CloseHandle( pi.hThread );
		CloseHandle( pi.hProcess );
	} else
	{
		// Ten-LANを最前面に。
		//SetForegroundWindow( MikanWindow->GetWindowHandle() );
	}

	// カレントディレクトリを戻す。
	SetCurrentDirectory( curdir );

	//SetForegroundWindow(MikanWindow->GetWindowHandle());

	// Pad2Keyを再有効化する。
	key->SetPad2Key( 1 );

	return 1;
}

// タスクバーの隠蔽
int TenLANSystem::HideTaskbar( void )
{
	OSVERSIONINFO OSver;
	HWND hWnd;

	OSver.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	GetVersionEx( &OSver );

	hWnd = FindWindow( "Shell_TrayWnd", NULL );

	if(ShowWindow( hWnd, SW_HIDE ))
	{
		if(( OSver.dwMajorVersion == 6 && OSver.dwMinorVersion >= 1 ) || OSver.dwMajorVersion > 6)
		{
			hWnd = FindWindowEx( NULL, NULL, "Button", "スタート" );
		} else
		{
			hWnd = FindWindow( "Button", "Start" );
		}
		ShowWindow( hWnd, SW_HIDE );
		return 0;
	}
	return 1;
}

// タスクバーの復活
int TenLANSystem::RestoreTaskbar( void )
{
	OSVERSIONINFO OSver;
	HWND hWnd;

	OSver.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	GetVersionEx( &OSver );

	hWnd = FindWindow( "Shell_TrayWnd", NULL );
	ShowWindow( hWnd, SW_RESTORE );

	if(( OSver.dwMajorVersion == 6 && OSver.dwMinorVersion >= 1 ) || OSver.dwMajorVersion > 6)
	{
		hWnd = FindWindowEx( NULL, NULL, "Button", "スタート" );
	} else
	{
		hWnd = FindWindow( "Button", "Start" );
	}
	ShowWindow( hWnd, SW_RESTORE );
	return 0;
}

int TenLANSystem::ESCCannon( void )
{
	// TODO:メインのパッド情報があればそれを指定するとか、すべてのパッドに対応させるとかしよう。
	return config->GetKeyClass()->ESCCannon(gamewindow);
}


HWND TenLANSystem::GetWindowHandle( unsigned long targetid )
{
	HWND hw = GetTopWindow( NULL );
	DWORD processid;

	do
	{
		if(GetWindowLong( hw, GWL_HWNDPARENT ) != 0 || !IsWindowVisible( hw ))
		{
			continue;
		}
		GetWindowThreadProcessId( hw, &processid );
		if(targetid == processid)
		{
			return hw;
		}
	} while(( hw = GetNextWindow( hw, GW_HWNDNEXT ) ) != NULL);

	return NULL;
}

unsigned int TenLANSystem::AddTexture( const char *imgfile )
{
	int tex = TextureCheck( imgfile );
	MikanDraw->CreateTexture( tex, imgfile, TRC_NONE );
	return tex;
}

unsigned int TenLANSystem::AddTexture( HMODULE module, const char *name )
{
	int tex = TextureCheck( name );
	MikanDraw->CreateTexture( tex, module, name, TRC_NONE );
	return tex;
}

int TenLANSystem::TextureCheck( const char *filename )
{
	unsigned int load;
	struct MYTEXTURE *texdata;

	for(load = 1 ; load < _MikanDraw->GetTextureMax() ; ++load)
	{
		texdata = _MikanDraw->GetTextureData( load );
		if(texdata)
		{
			if(strcmp( texdata->filename, filename ) == 0)
			{
				return load;
			}
		} else if(texmax <= load)
		{
			texmax = load + 1;
			return load;
		}
	}

	return 0;
}

int TenLANSystem::CountUp( unsigned int gamenum )
{
	return 0;
}

int TenLANSystem::GetGameMax( void ){
	return gamemax;
}

int TenLANSystem::GetGameMax( unsigned int category )
{
	int count = 0;
	unsigned int i, c;

	if(CATEGORY_MAX <= category)
	{
		return -1;
	}

	for(i = 0 ; i < gamemax ; ++i)
	{
		for(c = 0 ; c < gamedata[ i ].cnum ; ++c)
		{
			if(gamedata[ i ].category[ c ] == category)
			{
				++count;
				break;
			}
		}
	}

	return count;
}

int TenLANSystem::GetGameNumbers( int *gamearray, int gamenum, int start )
{
	int count = 0;
	unsigned int i;

	for(i = 0 ; i < gamemax ; ++i)
	{
		if(start-- <= 0)
		{
			gamearray[ count++ ] = i;
			if(gamenum <= count)
			{
				return count;
			}
			//break;
		}
	}

	return count;
}

int TenLANSystem::GetGameNumbers( unsigned int category, int *gamearray, int gamenum, int start )
{
	int count = 0;
	unsigned int i, c;

	if(CATEGORY_MAX <= category)
	{
		return -1;
	}

	for(i = 0 ; i < gamemax ; ++i)
	{
		for(c = 0 ; c < gamedata[ i ].cnum ; ++c)
		{
			if(gamedata[ i ].category[ c ] == category && start-- <= 0)
			{
				gamearray[ count++ ] = i;
				if(gamenum <= count)
				{
					return count;
				}
				break;
			}
		}
	}

	return count;
}

int TenLANSystem::GetGameNumbersNotCategory( unsigned int notcategory, int *gamearray, int gamenum, int start )
{
	int count = 0;
	unsigned int i, c;
	int ok;

	if(CATEGORY_MAX <= notcategory)
	{
		return -1;
	}

	for(i = 0 ; i < gamemax ; ++i)
	{
		for(c = 0 ; c < gamedata[ i ].cnum ; ++c)
		{
			ok = 1;
			if(gamedata[ i ].category[ c ] == notcategory)
			{
				ok = 0;
				break;
			}
		}
		if(ok && start-- <= 0)
		{
			gamearray[ count++ ] = i;
			if(gamenum <= count)
			{
				return count;
			}
		}
	}

	return count;
}

int TenLANSystem::GetGameMainCategory( unsigned int gamenum )
{
	if(gamemax <= gamenum)
	{
		return 0;
	}

	return gamedata[ gamenum ].category[ 0 ];
}

unsigned int TenLANSystem::GetGameCaetgoryMax( unsigned int gamenum )
{
	if(gamemax <= gamenum)
	{
		return 0;
	}
	return gamedata[ gamenum ].cnum;
}

int TenLANSystem::GetGameCategory( unsigned int gamenum, unsigned int category )
{
	if(gamemax <= gamenum || GetGameCaetgoryMax( gamenum ) <= category)
	{
		return 0;
	}
	return gamedata[ gamenum ].category[ category ];
}

const char * TenLANSystem::GetGameTitle( unsigned int gamenum )
{
	if(gamemax <= gamenum)
	{
		return NULL;
	}

	return gamedata[ gamenum ].title;
}

const char * TenLANSystem::GetGameInfo( unsigned int gamenum )
{
	if(gamemax <= gamenum)
	{
		return NULL;
	}

	return gamedata[ gamenum ].text;
}

unsigned int TenLANSystem::GetGameSSTexture( unsigned int gamenum )
{
	if(gamemax <= gamenum)
	{
		return 0;
	}

	return gamedata[ gamenum ].txnum;
}

int TenLANSystem::GetGameSSTMax( unsigned int gamenum )
{
	if(gamemax <= gamenum)
	{
		return 0;
	}

	return gamedata[gamenum].imagenum;
}

int TenLANSystem::IsInDVD( unsigned int gamenum )
{
	if(gamemax <= gamenum)
	{
		return 0;
	}

	return gamedata[ gamenum ].dvd;
}

class Config * TenLANSystem::GetConfig( void ) {
	return config;
}
int TenLANSystem::GetExitTime( void ){
	return config->GetExitTime();
}
int TenLANSystem::GetWidth( void ){
	return width;
}
int TenLANSystem::GetHeight( void ){
	return height;
}
int TenLANSystem::GetCategoryMax( void ){
	return categorymax;
}
int TenLANSystem::GetCategoryGames( int catenum )
{
	if(catenum < 0 || CATEGORY_MAX <= catenum)
	{
		return 0;
	}
	return usecate[ catenum ];
}
int TenLANSystem::IsNewGame( unsigned int gamenum )
{
	if(gamemax <= gamenum)
	{
		return 0;
	}
	return newline <= gamedata[ gamenum ].nendo;
}

int TenLANSystem::GetUp( void ){
	return key->GetUp();
}
int TenLANSystem::GetDown( void ){
	return key->GetDown();
}
int TenLANSystem::GetLeft( void ){
	return key->GetLeft();
}
int TenLANSystem::GetRight( void ){
	return key->GetRight();
}

bool TenLANSystem::IsDebug( void )
{
	return debug;
}
int TenLANSystem::SetDebug( bool debugmode )
{
	debug = debugmode;

	return 0;
}

int TenLANSystem::GetY( void ){
	return key->GetY();
}
int TenLANSystem::GetA( void ){
	return key->GetA();
}
int TenLANSystem::GetB( void ){
	return key->GetB();
}
int TenLANSystem::GetX( void ){
	return key->GetX();
}
int TenLANSystem::GetL( void ){
	return key->GetL();
}
int TenLANSystem::GetR( void ){
	return key->GetR();
}
int TenLANSystem::GetMode( void ){
	return config->GetMode();
}
