#include "Config.h"

Config::Config( void )
{
	Init( new Key() );
}

Config::Config( class Key * key )
{
	Init( key );
}

int Config::Init( class Key *key )
{
	this->key = NULL;
	SetKeyClass( key );

	strcpy_s( gamedir, GAMEDIR_LEN, "Game" );

	exittime = EXIT_TIME;

	exhibition = 1;

	hidetaskbar = 1;

	viewcategory = viewdvd = viewnewgame = 1;

	logcgi = NULL;

	ui = 0;

	bgmvolume = 20;
	sevolume = 80;

	return 0;
}

Config::~Config( void )
{
	if(key)
	{
		delete( key );
	}
	if(logcgi)
	{
		free( logcgi );
	}
}

int Config::SetKeyClass( class Key *key )
{
	if(this->key)
	{
		delete( this->key );
	}
	this->key = key;
	return 0;
}

class Key * Config::GetKeyClass( void )
{
	return key;
}

const char * Config::GetGameDirectory()
{
	return gamedir;
}

int Config::GetMode( void )
{
	return exhibition;
}

int Config::GetUIType( void )
{
	return ui;
}

int Config::GetBGMVolume(void)
{
	return bgmvolume;
}

int Config::GetSEVolume(void)
{
	return sevolume;
}

int Config::IsHideTaskbar( void )
{
	return ( hidetaskbar != 0 );
}

int Config::IsViewCategory( void )
{
	return ( viewcategory != 0 );
}

int Config::IsViewNewGame( void )
{
	return ( viewnewgame != 0 );
}

int Config::IsViewDVD( void )
{
	return ( viewdvd != 0 );
}

int Config::GetExitTime( void )
{
	return exittime;
}

const char * Config::GetLogCGIURL( void )
{
	return logcgi;
}

int Config::SaveConfig( int mode, int arg )//const char *config
{
	//FILE *fp;
	//  struct stat fstat;
	//char buf[ 256 ], *tok, *str;
	//  int c, w;
	//int maxvolume = 100;
	//int pad2keysleep = 20;
	//char PAD[ 64 ] = "";


	/*if( fopen_s( &fp, "data.ini", "r" ) == 0 )
	{
	while ( fgets( buf, 256, fp ) != NULL )
	{
	str = strtok_s( buf, "=", &tok );
	if( strcmp( str, "SOUND_VOLUME" ) == 0 )
	{
	// 音量
	maxvolume = ( mode == SAVE_SOUND_VOLUME ) ? arg : atoi( strtok_s( NULL, "=", &tok ) ) ;
	}else if( strcmp( str, "PAD" ) == 0 )
	{
	// パッド
	if( mode == SAVE_PAD )
	{
	sprintf_s( PAD, 64, "%d,%d,%d,%d,%d,%d,%d", 0, keys[ 0 ], keys[ 1 ], keys[ 2 ], keys[ 3 ], keys[ 4 ], keys[ 5 ] );
	}else
	{
	strcpy_s( PAD, 64, strtok_s( NULL, "=", &tok ) );
	}
	}else if( strcmp( str, "PAD2KEY_DIS" ) == 0 )
	{
	// PAD2KEY DISTANCE
	pad2keysleep = ( mode == SAVE_PAD2KEY_DISTANCE ) ? arg : atoi( strtok_s( NULL, "=", &tok ) ) ;
	}
	}

	fclose( fp );
	}else
	{
	// 音量
	maxvolume = ( mode == SAVE_SOUND_VOLUME ) ? arg : GetMaxVolume();//sys.maxvolume ;
	// パッド
	sprintf_s( PAD, 64, "%d,%d,%d,%d,%d,%d,%d", 0, keys[ 0 ], keys[ 1 ], keys[ 2 ], keys[ 3 ], keys[ 4 ], keys[ 5 ] );
	}

	if( fopen_s( &fp, CONFIG_FILE, "w" ) == 0 )
	{
	fprintf( fp, "SOUND_VOLUME=%d\r\n", maxvolume );
	fprintf( fp, "PAD=%s\r\n", PAD );
	fprintf( fp, "PAD2KEY_DIS=%d\r\n", pad2keysleep );
	}
	fclose( fp );*/
	return 0;
}

int Config::LoadConfig( const char *config )
{
	unsigned int buttonnum;
	int button;
	char buf[ 512 ], *str, *tok;

	if(MikanFile->Open( 0, config ) >= 0)
	{
		while(MikanFile->ReadLine( 0, buf, 512 ))
		{
			str = strtok_s( buf, "=", &tok );
			// コメントは無視。
			if(str[ 0 ] == '#' || ( str[ 0 ] == '/' && str[ 1 ] == '/' ))
			{
				continue;
			}
			if(strcmp( str, "SOUND_VOLUME" ) == 0)
			{
				//sys.maxvolume = atoi( strtok_s( NULL, ",", &tok ) );
				bgmvolume = atoi(strtok_s(NULL, ",", &tok));
				if (bgmvolume < 0)
				{
					bgmvolume = 0;
				}
				else if (bgmvolume > 100)
				{
					bgmvolume = 100;
				}
				sevolume = atoi(strtok_s(NULL, ",", &tok));
				if (sevolume < 0)
				{
					sevolume = 0;
				}
				else if (sevolume > 100)
				{
					sevolume = 100;
				}
			} else if(key && strcmp( str, "PAD" ) == 0)
			{

				// パッド番号
				key->SetMainPad( atoi( strtok_s( NULL, ",", &tok ) ) );

				button = 0;
				while(( str = strtok_s( NULL, ",", &tok ) ) != NULL)
				{
					buttonnum = atoi( str );
					// パッドのキー登録。
					key->SetPadKey( button, buttonnum );
					if(++button >= 6)
					{
						break;
					}
				}

				// 複数設定が有効になれば頑張る。
				//break;
			} else if(key && strcmp( str, "ESCCANNON" ) == 0)
			{

				// ESC砲の必要フレーム数。
				key->SetEscCannonFrame( atoi( strtok_s( NULL, ",", &tok ) ) );

				while(( str = strtok_s( NULL, ",", &tok ) ) != NULL)
				{
					// ESC砲のためのパッドのキー登録。
					if(key->AddESCKey( atoi( str ) ))
					{
						break;
					}
				}

			} else if(strcmp( str, "PAD2KEY_DIS" ) == 0)
			{
				pad2keysleep = atoi( strtok_s( NULL, ",", &tok ) );
				if(pad2keysleep < 1)
				{
					pad2keysleep = 10;
				}
			} else if(strcmp( str, "EXITTIME" ) == 0)
			{
				//strcpy_s(gamedir, GAMEDIR_LEN, tok);
				exittime = atoi( tok );
			} else if(strcmp( str, "GAMEDIR" ) == 0)
			{
				strcpy_s( gamedir, GAMEDIR_LEN, tok );
				if(gamedir[ strlen( gamedir ) - 1 ] == '\n')
				{
					gamedir[ strlen( gamedir ) - 1 ] = '\0';
				}
			} else if(strcmp( str, "UI" ) == 0)
			{
				ui = atoi( tok );
			} else if(strcmp( str, "FULLSCREEN" ) == 0)
			{
				if(strncmp( tok, "true", 4 ) == 0)
				{
					// 展示モード。
					exhibition = 1;
				} else
				{
					exhibition = 0;
				}
			} else if(strcmp( str, "HIDETASKBAR" ) == 0)
			{
				if(strncmp( tok, "true", 4 ) == 0)
				{
					// タスクバーは隠す。
					hidetaskbar = 1;
				} else
				{
					hidetaskbar = 0;
				}
			} else if(strcmp( str, "VIEWCATEGORY" ) == 0)
			{
				if(strncmp( tok, "true", 4 ) == 0)
				{
					// カテゴリを表示する。
					viewcategory = 1;
				} else
				{
					// カテゴリを隠す。
					viewcategory = 0;
				}
			} else if(strcmp( str, "VIEWDVD" ) == 0)
			{
				if(strncmp( tok, "true", 4 ) == 0)
				{
					// DVD収録は表示する。
					viewdvd = 1;
				} else
				{
					// DVD収録は隠す。。
					viewdvd = 0;
				}
			} else if(strcmp( str, "VIEWNEWGAME" ) == 0)
			{
				if(strncmp( tok, "true", 4 ) == 0)
				{
					// 新作情報を表示する。
					viewnewgame = 1;
				} else
				{
					// 新作情報は表示しない。
					viewnewgame = 0;
				}
			} else if(strcmp( str, "LOGCGI" ) == 0)
			{
				if(strncmp( tok, "http://", 7 ) == 0)
				{
					logcgi = (char *)calloc( strlen( tok ) + 1, sizeof( char ) );
					strcpy_s( logcgi, strlen( tok ) + 1, tok );
				}
			}
		}
		MikanFile->Close( 0 );
	}
	return 0;
}

