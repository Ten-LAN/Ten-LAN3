#ifndef CLASS_GAMEVIEW
#define CLASS_GAMEVIEW

#include "TenLAN.h"

// 実行は初めの一度きり。
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

// 背景。多分ほんとどなにもしない。
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
	// ! 一度だけ実行される準備フェーズ。
	virtual void UserInit( void );
	// ! UserInitの後とゲームが終了して返ってきた時に実行される復旧フェーズ。
	virtual void Resume( void );
	// ! UIのメイン部分を実行するフェーズ。
	virtual int MainLoop( void );
	// ! GameViewが破棄される前に実行される片付けフェーズ。
	virtual void CleanUp( void );
};

#endif