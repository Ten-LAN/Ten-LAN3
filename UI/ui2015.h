#include "../TenLAN.h"

// デフォルトのセレクト画面。
class UI2015 : public GameView {
private:
	enum {
		UIMODE_SELECT,
		UIMODE_SETTING,
		UIMODE_INFO,
		UIMODE_INFOBACK,
		UIMODE_EXEC,
		UIMODE_OTHER,
	};
	int mode;  // モード。
	int timer; //タイマー。
	int gamemax, minigamemax; // ゲーム数、ミニゲーム数。
	int *game, nowgames; // ゲーム番号一覧、gameに入ってるゲーム数。gameの中身を書き換えていろいろやってる。
	int category; // 現在我のカテゴリ。-2がミニゲームまとめて,-1が全部。それ以降はそのカテゴリのゲーム。
	int tex; // テクスチャ番号。
	int ranTex; // テクスチャ番号。
	int stoptime;
	double scale;
	int boxsize;
	double centerline, stopline, radius;
	double moveasp, speedline, speedline_;

	int bgmnum;        // デフォ含めた曲数
	void PlayBgm();    // ランダムでBGM流す

	int curimage;	  // 現在の背景
	int* image;		  // 背景
	int imagenum;	  // 背景の数

	unsigned int curbgm; // 現在のBGM
	void SetBGMVolume();
	void SetSEVolume();
	int selectgame;

	double infox, infoy, infospeed;
	int infonum;

	int fontsize;

	// 設定用の変数。
	int sselect[4];

	virtual int SetGames(int category);
	virtual int DrawBeginSelect(void);
	virtual int DrawSelect(int acinput = 1);
	virtual int DrawSetting(void);
	virtual int DrawInfomation(int acinput = 1);
	virtual int DrawInfomationBack(void);
	virtual int DrawBackground(void);
	virtual int DrawGameTitle(int gamenum);
	virtual int DrawGameList(int basex = 0, int basey = 0);
	virtual double DrawBox(int dx, int dy, int gamenum, double scl = 1.0, unsigned char alpha = 255);
	virtual int ExecGame(int gamenum);
	virtual int PrevSelectNumber(int select);
	virtual int NextSelectNumber(int select);
	virtual void GetSSSize(int *ssx, int *ssy, double *scale, int gamenum);
public:
	virtual void UserInit(void);
	virtual void Resume(void);
	virtual int MainLoop(void);
	virtual void CleanUp(void);
};
