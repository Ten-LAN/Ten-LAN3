#ifndef CLASS_TENLAN
#define CLASS_TENLAN

#include <Mikan.h>
#include "Config.h"

#define CATEGORY_MAX 11

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
  int gamemax;
  int execgamenum;
  struct GameData *gamedata;
  char usecate[CATEGORY_MAX]; // TODO:
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

  virtual int NextGameView( void );
  virtual int CleanupExecGame( void );
  virtual int TextureCheck( const char *filename );
  virtual int AddLog( unsigned int gamenum, unsigned long playtime );
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
  virtual int GetGameCaetgoryMax(unsigned int gamenum);

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
  virtual int GetCategoryMax(void);
  // ! ���̃J�e�S�������Q�[���̐��B
  virtual int GetCategoryGames( int catenum );
  // ! �Q�[�����ŐV�N�x�̂��̂��ǂ����̔��ʁB
  virtual int IsNewGame( unsigned int gamenum );

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
  virtual int MainLoop( void ){ return 0; }
  // ! GameView���j�������O�Ɏ��s�����Еt���t�F�[�Y�B
  virtual void CleanUp( void ){}
};

// ���s�͏��߂̈�x����B
class StartUp : public GameView
{
private:
  int timer;
  int gamemax, loadgame;
  int rad;
  int mode;
  virtual int DrawGameState( void );
public:
  virtual int Loading( void );
  virtual void UserInit( void );
  virtual void Resume( void );
  virtual int MainLoop( void );
  virtual void CleanUp( void );
};

// �w�i�B�����ق�ƂǂȂɂ����Ȃ��B
class BackGround : public GameView
{
public:
  virtual void UserInit( void );
  virtual int MainLoop( void );
  virtual void CleanUp( void );
};

class OP : public GameView
{
private:
  int timer;
  int tex;
public:
  // ! ��x�������s����鏀���t�F�[�Y�B
  virtual void UserInit( void );
  // ! UserInit�̌�ƃQ�[�����I�����ĕԂ��Ă������Ɏ��s����镜���t�F�[�Y�B
  virtual void Resume( void );
  // ! UI�̃��C�����������s����t�F�[�Y�B
  virtual int MainLoop( void );
  // ! GameView���j�������O�Ɏ��s�����Еt���t�F�[�Y�B
  virtual void CleanUp( void );
};

// �f�t�H���g�̃Z���N�g��ʁB
class UIDefault : public GameView
{
private:
  enum
  {
    UIMODE_SELECT,
    UIMODE_SETTING,
    UIMODE_INFO,
    UIMODE_INFOBACK,
    UIMODE_EXEC,
    UIMODE_OTHER,
  };
  int mode;  // ���[�h�B
  int timer; //�^�C�}�[�B
  int gamemax, minigamemax; // �Q�[�����A�~�j�Q�[�����B
  int *game, nowgames; // �Q�[���ԍ��ꗗ�Agame�ɓ����Ă�Q�[�����Bgame�̒��g�����������Ă��낢�����Ă�B
  int category; // ���݉�̃J�e�S���B-2���~�j�Q�[���܂Ƃ߂�,-1���S���B����ȍ~�͂��̃J�e�S���̃Q�[���B
  int tex; // �e�N�X�`���ԍ��B
  int stoptime;
  double scale;
  int boxsize;
  double centerline, stopline, radius;
  double moveasp, speedline, speedline_;

  int selectgame;

  double infox, infoy, infospeed;
  int infonum;

  int fontsize;

  // �ݒ�p�̕ϐ��B
  int sselect[ 4 ];

  virtual int SetGames( int category );
  virtual int DrawBeginSelect( void );
  virtual int DrawSelect( int acinput = 1 );
  virtual int DrawSetting(void);
  virtual int DrawInfomation( int acinput = 1 );
  virtual int DrawInfomationBack(void);
  virtual int DrawBackground(void);
  virtual int DrawGameTitle( int gamenum );
  virtual int DrawGameList( int basex = 0, int basey = 0 );
  virtual double DrawBox( int dx, int dy, int gamenum, double scl = 1.0, unsigned char alpha = 255 );
  virtual int ExecGame(int gamenum);
  virtual int PrevSelectNumber( int select );
  virtual int NextSelectNumber( int select );
public:
  virtual void UserInit( void );
  virtual void Resume( void );
  virtual int MainLoop( void );
  virtual void CleanUp( void );
};

#define GAMEDATA_EXE_LEN   512
#define GAMEDATA_TITLE_LEN 256
#define GAMEDATA_TEXT_LEN  1024
#define GAMEDATA_TEX_LEN   512

struct GameData
{
  int num;                          // �Q�[���ԍ��B
  char exe[ GAMEDATA_EXE_LEN ];     // �N���p�X�B
  char title[ GAMEDATA_TITLE_LEN ]; // �Q�[�����B
  char text[ GAMEDATA_TEXT_LEN ];   // �Љ�B
  unsigned int first, nendo;        // ���J���B
  unsigned int date;                // ���t�B
  int pad2key;                      // Pad2Key���L�����ǂ����B
  int txnum;                        // �����摜�̃e�N�X�`���ԍ��B
  char txfile[ GAMEDATA_TEX_LEN ];  // �e�N�X�`���̃t�@�C���p�X�B
  int cnum;                         // �����Ă���J�e�S�����B
  char category[ CATEGORY_MAX ];    // �ǂ̃J�e�S���ɏ������Ă��邩�B
  char dvd;                         // DVD���^�ς݂��ǂ����B
};

#endif
