#ifndef CLASS_TENLAN
#define CLASS_TENLAN

#include <Mikan.h>
#include "Config.h"

#define CATEGORY_MAX 11
extern unsigned long CATE_COLOR[ CATEGORY_MAX ];
extern unsigned long CATE_DCOLOR[ CATEGORY_MAX ];
extern char CATE_NAME[ CATEGORY_MAX ][ 32 ];
extern int CATE_SORT[ CATEGORY_MAX ];

enum
{
	TENLAN_PLAY,     // 通常時。登録したGameViewを実行。
	TENLAN_WAIT,     // ゲーム実行のため待機。BackGroudを登録している場合は実行。
	TENLAN_NEXTPLAY, // 復旧した時に1フレーム処理したいことがある。
};

enum
{
	THREAD_LOADING,
	THREAD_EXEC,
	THREAD_ADDLOG,
};

class TenLANSystem
{
private:

	// Ten-LAN情報。
	int mode;
	class Config * config;
	class Key * key;
	// GameView。
	class GameView *now, *next, *background;

	// ゲーム情報。
	unsigned int gamemax;
	int execgamenum;
	struct GameData *gamedata;
	char usecate[ CATEGORY_MAX ]; // TODO:
	int categorymax;
	unsigned int newline;


	// プロセス監視。
	HWND gamewindow;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	// 起動時間。
	unsigned long beginexectime;


	// ウィンドウ情報。
	int width, height;

	// 現在のディレクトリ。
	char curdir[ 512 ];

	// テクスチャ管理。
	unsigned int texmax;

	// 統計処理に使うための日付(月、日、年の順)
	int date[3];

	// デバッグモード関係
	bool debug;

	virtual int NextGameView( void );
	virtual int CleanupExecGame( void );
	virtual int TextureCheck( const char *filename );
	virtual int AddLog( unsigned int gamenum, unsigned long playtime );

	// 統計の読み込みを行う関数(ゲームデータを読み込んだ後に行うこと!)
	virtual int LoadStatistics();

	// 統計の保存を行う関数
	virtual int SaveStatistics(unsigned int gamenum, unsigned long playtime);

public:
	TenLANSystem( void );
	virtual ~TenLANSystem( void );

	// 初期化。
	virtual int Init( const char *file = NULL );

	// TenLANメイン画面制御。

	// ! 新しいGameViewを登録し、MainLoop終了後に切り替える。
	virtual int SetGameView( class GameView *newgameview = NULL );

	// ! Ten-LAN本体が呼び出すメインの制御部分。
	virtual int PlayGameView( void );

	// ! ゲーム実行中に動作するバックグラウンドのGameView。
	virtual int SetBackGround( class GameView *background = NULL );

	// ゲーム情報の管理。
	virtual int InitGamelist( int gamemax = 36, int *loadgame = NULL );
	virtual int GetGamedirDirectorys( void );

	// 補助関数。
	virtual int ExecGame( unsigned int gamenum );
	virtual int CheckEndProcess( void );
	virtual int HideTaskbar( void );
	virtual int RestoreTaskbar( void );
	virtual int ESCCannon( void );

	virtual HWND TenLANSystem::GetWindowHandle( unsigned long targetid );

	virtual unsigned int AddTexture( const char *imgfile );
	virtual unsigned int AddTexture( HMODULE module, const char *name );

	// 入力系。


	// サーバー関連。
	virtual int CountUp( unsigned int gamenum );

	// 値取得。
	// ! 現在稼働可能なゲームの数。
	virtual int GetGameMax( void );
	// ! 現在稼働可能なあるカテゴリに所属するゲームの数。
	/* !
	\param category カテゴリ番号
	*/
	virtual int GetGameMax( unsigned int category );
	// ! ゲーム番号を配列に格納する。
	/* !
	\param category カテゴリ番号。
	\param gamearray 格納する配列のポインタ。
	\param gamenum 格納するゲーム数(配列の長さ)。
	\param start 格納開始するゲームの個数(初期値0)。
	*/
	virtual int GetGameNumbers( int *gamearray, int gamenum, int start = 0 );
	// ! ゲーム番号を配列に格納する。
	/* !
	\param category カテゴリ番号。
	\param gamearray 格納する配列のポインタ。
	\param gamenum 格納するゲーム数(配列の長さ)。
	\param start 格納開始するゲームの個数(初期値0)。
	*/
	virtual int GetGameNumbers( unsigned int category, int *gamearray, int gamenum, int start = 0 );
	// ! ゲーム番号を配列に格納する(指定したカテゴリ以外)。
	/* !
	\param notcategory 含まないカテゴリ番号。
	\param gamearray 格納する配列のポインタ。
	\param gamenum 格納するゲーム数(配列の長さ)。
	\param start 格納開始するゲームの個数(初期値0)。
	*/
	virtual int GetGameNumbersNotCategory( unsigned int notcategory, int *gamearray, int gamenum, int start = 0 );

	// ! ゲームのメインカテゴリを取得。
	virtual int GetGameMainCategory( unsigned int gamenum );

	// ! ゲームの持つカテゴリ最大数を取得。
	virtual unsigned int GetGameCaetgoryMax( unsigned int gamenum );

	// ! ゲームのサブカテゴリの値を取得。
	virtual int GetGameCategory( unsigned int gamenum, unsigned int category );

	// ! ゲームのタイトルを取得。
	virtual const char * GetGameTitle( unsigned int gamenum );

	// ! ゲームの情報を取得。
	virtual const char * GetGameInfo( unsigned int gamenum );

	// ! ゲームのSSのテクスチャ番号を取得。
	virtual unsigned int GetGameSSTexture( unsigned int gamenum );

	// ! SSの持ってる枚数を取得。
	virtual int GetGameSSTMax( unsigned int gamenum );

	// ! DVD収録かどうが調べる。
	virtual int IsInDVD( unsigned int gamenum );

	// ! コンフィグ取得。
	virtual class Config * GetConfig( void );
	// ! ESCで終了する時間。
	virtual int GetExitTime( void );
	// ! 画面の横幅。
	virtual int GetWidth( void );
	// ! 画面の高さ。
	virtual int GetHeight( void );
	// ! Ten-LANに含まれるカテゴリの種類の最大値。
	virtual int GetCategoryMax( void );
	// ! そのカテゴリを持つゲームの数。
	virtual int GetCategoryGames( int catenum );
	// ! ゲームが最新年度のものかどうかの判別。
	virtual int IsNewGame( unsigned int gamenum );

	// デバッグモード関係

	// ! デバッグモードかどうかの取得(trueでデバッグモード)
	virtual bool IsDebug( void );
	// !デバッグモードの設定(trueでデバッグモード)
	virtual int SetDebug( bool debugmode );

	virtual int GetUp( void );
	virtual int GetDown( void );
	virtual int GetLeft( void );
	virtual int GetRight( void );

	virtual int GetY( void );
	virtual int GetA( void );
	virtual int GetB( void );
	virtual int GetX( void );
	virtual int GetL( void );
	virtual int GetR( void );

	virtual int GetMode( void );
};

class GameView
{
public:
	// ! Ten-LANが管理するデータにアクセスするためのオブジェクト。
	static class TenLANSystem *system;

	// ! 一度だけ実行される準備フェーズ。
	virtual void UserInit( void ){}
	// ! UserInitの後とゲームが終了して返ってきた時に実行される復旧フェーズ。
	virtual void Resume( void ){}
	// ! UIのメイン部分を実行するフェーズ。
	virtual int MainLoop( void ){
		return 0;
	}
	// ! GameViewが破棄される前に実行される片付けフェーズ。
	virtual void CleanUp(void){}
};

#define GAMEDATA_EXE_LEN   512
#define GAMEDATA_TITLE_LEN 256
#define GAMEDATA_TEXT_LEN  1024
#define GAMEDATA_TEX_LEN   512

// 統計データの構造体
struct StatisticsData
{
	unsigned int playnum;	// プレイ回数
	unsigned long playtime;	//プレイ時間
};

//ゲームデータの構造体
struct GameData
{
	unsigned int num;                 // ゲーム番号。
	char exe[ GAMEDATA_EXE_LEN ];     // 起動パス。
	char title[ GAMEDATA_TITLE_LEN ]; // ゲーム名。
	char text[ GAMEDATA_TEXT_LEN ];   // 紹介文。
	unsigned int first, nendo;        // 公開日。
	unsigned int date;                // 日付。
	int pad2key;                      // Pad2Keyが有効かどうか。
	int txnum;                        // 説明画像のテクスチャ番号。
	int imagenum;                     // 画像の枚数
	char txfile[ GAMEDATA_TEX_LEN ];  // テクスチャのファイルパス。
	unsigned int cnum;                // 持っているカテゴリ数。
	char category[ CATEGORY_MAX ];    // どのカテゴリに所属しているか。
	char dvd;                         // DVD収録済みかどうか。
	int minplnum;                     // 最小プレイ人数

	StatisticsData today, total;	  // 統計データ
};


#endif
