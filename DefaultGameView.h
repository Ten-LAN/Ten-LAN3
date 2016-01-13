#ifndef CLASS_GAMEVIEW
#define CLASS_GAMEVIEW

#include "TenLAN.h"

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

#endif