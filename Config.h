#ifndef CLASS_CONFIG
#define CLASS_CONFIG
#include <Mikan.h>
#include "Key.h"

#define GAMEDIR_LEN 512

#define EXIT_TIME (60 * 1)

class Config
{
private:
	char gamedir[ GAMEDIR_LEN ];
	int bgmvolume, sevolume;
	class Key *key;
	int pad2keysleep;

	// UI�̎�ށB
	int ui;

	// �I�����ԁB
	int exittime;
	// �W�����[�h
	int exhibition;
	// �^�X�N�o�[���B���B
	int hidetaskbar;
	// �J�e�S����\������B
	int viewcategory;
	// �V������o���B
	int viewnewgame;
	// DVD���^�����o���B
	int viewdvd;

	// ���OCGI�B
	char *logcgi;

	virtual int Init( class Key *key );
public:
	Config( void );
	Config( class Key * key );
	virtual ~Config( void );

	virtual int SetKeyClass( class Key * key );
	virtual class Key * GetKeyClass( void );

	virtual const char * GetGameDirectory( void );

	virtual int GetMode( void );
	virtual int GetUIType(void);
	virtual int GetBGMVolume(void);
	virtual int GetSEVolume( void );
	virtual int IsHideTaskbar( void );
	virtual int IsViewCategory( void );
	virtual int IsViewNewGame( void );
	virtual int IsViewDVD( void );
	virtual int GetExitTime( void );
	virtual const char * GetLogCGIURL( void );

	virtual int SaveConfig( int mode, int arg );
	virtual int LoadConfig( const char *config );
};
#endif
