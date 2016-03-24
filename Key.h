#ifndef CLASS_KEY
#define CLASS_KEY
#include <Mikan.h>

class Key
{
private:
	// キー管理。
	int keys[ 6 ];
	int mainpad;
	// ESC判定用のキー。
	int ekey[ 6 ];
	// ESC砲の発射のフレーム数。
	int esccannonframe;
public:
	Key( void );
	virtual ~Key( void );

	// ! メインで使用するゲームパッドの設定。
	virtual int SetMainPad( int padnum = 0 );
	// ! Pad2Keyのキー設定。
	/* !
	\param buttomnum パッドのボタン番号
	\param keycode キーボードのキー番号
	*/
	virtual int SetPadKey( int buttonnum, int keycode );

	// ! ESC砲のキー設定。
	virtual int AddESCKey( int button );
	// ! ESC砲のフレーム設定。
	virtual int SetEscCannonFrame( int frame );

	// ! Pad2Keyの有効化/無効化
	/* !
	\param padnum パッド番号
	\param flag 0=無効化,1=有効化
	*/
	virtual int SetPad2Key( unsigned int padnum, int flag );
	// ! メインで使用するゲームパッドに対するPad2Keyの有効化/無効化
	/* !
	\param flag 0=無効化,1=有効化
	*/
	virtual int SetPad2Key( int flag );

	// ! Pad2Keyを有効化した時の設定を行う
	/* !
	\param padnum パッド番号。
	*/
	virtual int SetKeyConfig( unsigned int padnum );
	virtual int InvalidKeyConfig( unsigned int padnum );

	virtual int ESCCannon(HWND gamewindow, unsigned int padnum = 0);

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
};

#endif