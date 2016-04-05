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
	TENLAN_PLAY,     // �ʏ펞�B�o�^����GameView�����s�B
	TENLAN_WAIT,     // �Q�[�����s�̂��ߑҋ@�BBackGroud��o�^���Ă���ꍇ�͎��s�B
	TENLAN_NEXTPLAY, // ������������1�t���[���������������Ƃ�����B
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

	// Ten-LAN���B
	int mode;
	class Config * config;
	class Key * key;
	// GameView�B
	class GameView *now, *next, *background;

	// �Q�[�����B
	unsigned int gamemax;
	int execgamenum;
	struct GameData *gamedata;
	char usecate[ CATEGORY_MAX ]; // TODO:
	int categorymax;
	unsigned int newline;


	// �v���Z�X�Ď��B
	HWND gamewindow;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	// �N�����ԁB
	unsigned long beginexectime;


	// �E�B���h�E���B
	int width, height;

	// ���݂̃f�B���N�g���B
	char curdir[ 512 ];

	// �e�N�X�`���Ǘ��B
	unsigned int texmax;

	// ���v�����Ɏg�����߂̓��t(���A���A�N�̏�)
	int date[3];

	// �f�o�b�O���[�h�֌W
	bool debug;

	virtual int NextGameView( void );
	virtual int CleanupExecGame( void );
	virtual int TextureCheck( const char *filename );
	virtual int AddLog( unsigned int gamenum, unsigned long playtime );

	// ���v�̓ǂݍ��݂��s���֐�(�Q�[���f�[�^��ǂݍ��񂾌�ɍs������!)
	virtual int LoadStatistics();

	// ���v�̕ۑ����s���֐�
	virtual int SaveStatistics(unsigned int gamenum, unsigned long playtime);

public:
	TenLANSystem( void );
	virtual ~TenLANSystem( void );

	// �������B
	virtual int Init( const char *file = NULL );

	// TenLAN���C����ʐ���B

	// ! �V����GameView��o�^���AMainLoop�I����ɐ؂�ւ���B
	virtual int SetGameView( class GameView *newgameview = NULL );

	// ! Ten-LAN�{�̂��Ăяo�����C���̐��䕔���B
	virtual int PlayGameView( void );

	// ! �Q�[�����s���ɓ��삷��o�b�N�O���E���h��GameView�B
	virtual int SetBackGround( class GameView *background = NULL );

	// �Q�[�����̊Ǘ��B
	virtual int InitGamelist( int gamemax = 36, int *loadgame = NULL );
	virtual int GetGamedirDirectorys( void );

	// �⏕�֐��B
	virtual int ExecGame( unsigned int gamenum );
	virtual int CheckEndProcess( void );
	virtual int HideTaskbar( void );
	virtual int RestoreTaskbar( void );
	virtual int ESCCannon( void );

	virtual HWND TenLANSystem::GetWindowHandle( unsigned long targetid );

	virtual unsigned int AddTexture( const char *imgfile );
	virtual unsigned int AddTexture( HMODULE module, const char *name );

	// ���͌n�B


	// �T�[�o�[�֘A�B
	virtual int CountUp( unsigned int gamenum );

	// �l�擾�B
	// ! ���݉ғ��\�ȃQ�[���̐��B
	virtual int GetGameMax( void );
	// ! ���݉ғ��\�Ȃ���J�e�S���ɏ�������Q�[���̐��B
	/* !
	\param category �J�e�S���ԍ�
	*/
	virtual int GetGameMax( unsigned int category );
	// ! �Q�[���ԍ���z��Ɋi�[����B
	/* !
	\param category �J�e�S���ԍ��B
	\param gamearray �i�[����z��̃|�C���^�B
	\param gamenum �i�[����Q�[����(�z��̒���)�B
	\param start �i�[�J�n����Q�[���̌�(�����l0)�B
	*/
	virtual int GetGameNumbers( int *gamearray, int gamenum, int start = 0 );
	// ! �Q�[���ԍ���z��Ɋi�[����B
	/* !
	\param category �J�e�S���ԍ��B
	\param gamearray �i�[����z��̃|�C���^�B
	\param gamenum �i�[����Q�[����(�z��̒���)�B
	\param start �i�[�J�n����Q�[���̌�(�����l0)�B
	*/
	virtual int GetGameNumbers( unsigned int category, int *gamearray, int gamenum, int start = 0 );
	// ! �Q�[���ԍ���z��Ɋi�[����(�w�肵���J�e�S���ȊO)�B
	/* !
	\param notcategory �܂܂Ȃ��J�e�S���ԍ��B
	\param gamearray �i�[����z��̃|�C���^�B
	\param gamenum �i�[����Q�[����(�z��̒���)�B
	\param start �i�[�J�n����Q�[���̌�(�����l0)�B
	*/
	virtual int GetGameNumbersNotCategory( unsigned int notcategory, int *gamearray, int gamenum, int start = 0 );

	// ! �Q�[���̃��C���J�e�S�����擾�B
	virtual int GetGameMainCategory( unsigned int gamenum );

	// ! �Q�[���̎��J�e�S���ő吔���擾�B
	virtual unsigned int GetGameCaetgoryMax( unsigned int gamenum );

	// ! �Q�[���̃T�u�J�e�S���̒l���擾�B
	virtual int GetGameCategory( unsigned int gamenum, unsigned int category );

	// ! �Q�[���̃^�C�g�����擾�B
	virtual const char * GetGameTitle( unsigned int gamenum );

	// ! �Q�[���̏����擾�B
	virtual const char * GetGameInfo( unsigned int gamenum );

	// ! �Q�[����SS�̃e�N�X�`���ԍ����擾�B
	virtual unsigned int GetGameSSTexture( unsigned int gamenum );

	// ! SS�̎����Ă閇�����擾�B
	virtual int GetGameSSTMax( unsigned int gamenum );

	// ! DVD���^���ǂ������ׂ�B
	virtual int IsInDVD( unsigned int gamenum );

	// ! �R���t�B�O�擾�B
	virtual class Config * GetConfig( void );
	// ! ESC�ŏI�����鎞�ԁB
	virtual int GetExitTime( void );
	// ! ��ʂ̉����B
	virtual int GetWidth( void );
	// ! ��ʂ̍����B
	virtual int GetHeight( void );
	// ! Ten-LAN�Ɋ܂܂��J�e�S���̎�ނ̍ő�l�B
	virtual int GetCategoryMax( void );
	// ! ���̃J�e�S�������Q�[���̐��B
	virtual int GetCategoryGames( int catenum );
	// ! �Q�[�����ŐV�N�x�̂��̂��ǂ����̔��ʁB
	virtual int IsNewGame( unsigned int gamenum );

	// �f�o�b�O���[�h�֌W

	// ! �f�o�b�O���[�h���ǂ����̎擾(true�Ńf�o�b�O���[�h)
	virtual bool IsDebug( void );
	// !�f�o�b�O���[�h�̐ݒ�(true�Ńf�o�b�O���[�h)
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
	// ! Ten-LAN���Ǘ�����f�[�^�ɃA�N�Z�X���邽�߂̃I�u�W�F�N�g�B
	static class TenLANSystem *system;

	// ! ��x�������s����鏀���t�F�[�Y�B
	virtual void UserInit( void ){}
	// ! UserInit�̌�ƃQ�[�����I�����ĕԂ��Ă������Ɏ��s����镜���t�F�[�Y�B
	virtual void Resume( void ){}
	// ! UI�̃��C�����������s����t�F�[�Y�B
	virtual int MainLoop( void ){
		return 0;
	}
	// ! GameView���j�������O�Ɏ��s�����Еt���t�F�[�Y�B
	virtual void CleanUp(void){}
};

#define GAMEDATA_EXE_LEN   512
#define GAMEDATA_TITLE_LEN 256
#define GAMEDATA_TEXT_LEN  1024
#define GAMEDATA_TEX_LEN   512

// ���v�f�[�^�̍\����
struct StatisticsData
{
	unsigned int playnum;	// �v���C��
	unsigned long playtime;	//�v���C����
};

//�Q�[���f�[�^�̍\����
struct GameData
{
	unsigned int num;                 // �Q�[���ԍ��B
	char exe[ GAMEDATA_EXE_LEN ];     // �N���p�X�B
	char title[ GAMEDATA_TITLE_LEN ]; // �Q�[�����B
	char text[ GAMEDATA_TEXT_LEN ];   // �Љ�B
	unsigned int first, nendo;        // ���J���B
	unsigned int date;                // ���t�B
	int pad2key;                      // Pad2Key���L�����ǂ����B
	int txnum;                        // �����摜�̃e�N�X�`���ԍ��B
	int imagenum;                     // �摜�̖���
	char txfile[ GAMEDATA_TEX_LEN ];  // �e�N�X�`���̃t�@�C���p�X�B
	unsigned int cnum;                // �����Ă���J�e�S�����B
	char category[ CATEGORY_MAX ];    // �ǂ̃J�e�S���ɏ������Ă��邩�B
	char dvd;                         // DVD���^�ς݂��ǂ����B
	int minplnum;                     // �ŏ��v���C�l��

	StatisticsData today, total;	  // ���v�f�[�^
};


#endif
