#include "ui2015.h"
#include <time.h>

void UI2015::PlayBgm() {
	curbgm = rand() % bgmnum;
	MikanSound->StopAll();
	MikanSound->Play(curbgm, true);
}

// BGM音量設定
void UI2015::SetBGMVolume()
{
	int volume = system->GetConfig()->GetBGMVolume();

	MikanSound->SetVolume(0, volume);
	MikanSound->SetVolume(1, volume / 2);
	MikanSound->SetVolume(2, volume / 2);
	MikanSound->SetVolume(3, volume / 2);
	MikanSound->SetVolume(4, volume);
}

// SE音量設定
void UI2015::SetSEVolume()
{
	int volume = system->GetConfig()->GetSEVolume();

	MikanSound->SetVolume(10, volume);
	MikanSound->SetVolume(11, volume);
}

int UI2015::SetGames(int category) {
	this->category = category;
	if (category == -1) {
		// 全て。
		nowgames = system->GetGameNumbers(game, gamemax);
	} else if (category == -2) {
		// ミニゲームは1つにまとめ、残りは普通に表示。
		if (minigamemax > 0) {
			nowgames = system->GetGameNumbersNotCategory(9, game + 1, gamemax) + 1;
			game[0] = -1; // ミニゲームのみ。
		} else {
			nowgames = system->GetGameNumbers(game, gamemax);
		}
	} else if (category == -3) {
		// ミニゲームは1つにまとめ、残りは普通に表示。
		if (minigamemax > 0) {
			nowgames = system->GetGameNumbersNotCategory(9, game + 2, gamemax) + 1;
			game[0] = -1; // ミニゲームのみ。
			// ミニゲーム以外がなければランダムは表示しない
			if (nowgames > 1)
			{
				nowgames++;
				game[1] = -3;
			}
		} else if(gamemax > 0) {
			nowgames = system->GetGameNumbers(game, gamemax) + 1;//system->GetGameNumbersがうまく機能してない
			game[0] = -3;
		} else {
			nowgames = system->GetGameNumbers(game, gamemax);
		}
	} else if (0 <= category) {
		// そのカテゴリのみ。
		nowgames = system->GetGameNumbers(category, game + 2, gamemax) + 2;
		game[0] = -2;
		game[1] = -3; // 

	}
	return 0;
}

void UI2015::UserInit(void) {
	srand((unsigned)time(0));

	tex = system->AddTexture(NULL, "DEFAULTIMG");
	ranTex = system->AddTexture(NULL, "RANDOM");
	
	// BGM
	bgmnum = 5; //デフォ含めた曲数

	MikanSound->Load(0, NULL, "MAINTHEME");
	MikanSound->Load(1, NULL, "SELECT_BGM1");
	MikanSound->Load(2, NULL, "SELECT_BGM2");
	MikanSound->Load(3, NULL, "SELECT_BGM3");
	MikanSound->Load(4, NULL, "SELECT_BGM4");

	imagenum = 3;
	image = static_cast<int*>(malloc(sizeof(int) * imagenum));

	image[0] = system->AddTexture(NULL, "BG_IMAGE1");
	image[1] = system->AddTexture(NULL, "BG_IMAGE2");
	image[2] = system->AddTexture(NULL, "BG_IMAGE3");

	// 初めの背景はなし
	curimage = -1;

	SetBGMVolume();

	// SE
	MikanSound->Load(11, NULL, "SELECT_SE");
	MikanSound->Load(10, NULL, "OK_SE");

	SetSEVolume();

	gamemax = system->GetGameMax();
	minigamemax = system->GetGameMax(9);

	if (gamemax > 0) {
		game = (int *)calloc(gamemax + 2, sizeof(int));
		for (int i = 0; i < gamemax; i++)
		{
			game[i] = 0;
		}
	} else {
		game = NULL;
	}

	scale = system->GetHeight() / 480.0;

	category = -3;
	SetGames(category);

	boxsize = (int)(system->GetHeight() / 2.0 - system->GetHeight() / 20.0);

	timer = 0;

	moveasp = MIKAN_PI;

	centerline = system->GetWidth() / 2.0;
	radius = centerline * 0.9;
	speedline = speedline_ = MIKAN_PI / 90.0;//radius /20.0;// 10.0;
	stopline = radius * abs(cos(MIKAN_PI / 3.0));

	stoptime = 0;

	selectgame = 0;

	fontsize = (int)(40 * scale);
	MikanDraw->CreateFont(10, "MigMix 1M", fontsize, 0xFF000000);
	mode = UIMODE_OTHER;
}

void UI2015::Resume(void) {
	timer = 0;
	mode = UIMODE_SELECT;
	PlayBgm();
	if (curbgm == 0)
	{
		curimage = -1;
	}
	else
	{
		curimage = curbgm - 1;
		if (curbgm == 4)
		{
			curimage = 2;
		}
	}
}

int UI2015::MainLoop(void) {
	int atimer = 30;
	int input = 0;

	MikanDraw->ClearScreen();

	if (timer < atimer * 3) {
		input = DrawBeginSelect();
	} else {
		switch (mode) {
		case UIMODE_SELECT:
			input = DrawSelect();
			break;
		case UIMODE_SETTING:
			// 入力を無効化して描画だけしてもらう。
			DrawSelect(0);
			input = DrawSetting();
			break;
		case UIMODE_INFO:
			input = DrawInfomation();
			break;
		case UIMODE_INFOBACK:
			input = DrawInfomationBack();
			break;
		case UIMODE_EXEC:
			/*      input = DrawInfomation( 0 );
			tmp = MikanSound->GetVolume( 0 );
			MikanDraw->DrawBox( 0, 0, system->GetWidth(), system->GetHeight(), MikanDraw->Color( 255 * tmp / 100, 255, 255, 255 ) );
			MikanSound->SetVolume( 0, tmp / 2 );*/
			break;
		}
	}

	// デバッグモード中の表示
	if (system->IsDebug())
	{
		MikanDraw->Printf(10, 0, 0, "デバッグモード");
	}

	if (input == 0) {
		++stoptime;
	} else {
		stoptime = 0;
	}
	++timer;

	if (system->GetMode() == 0) {
		MikanDraw->Printf(0, 30, MikanDraw->GetScreenHeight(1) - 100, 0xFF000000, "ESC長押しで終了 %d", nowgames);
	}
	
	return 0;
}

int UI2015::DrawBeginSelect(void) {
	int atimer = 30, time;

	if (timer < atimer) {
		MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), MikanDraw->Color(255 * timer / atimer, 255, 255, 255));
	} else if (timer < atimer * 2) {
		time = timer - atimer;
		DrawBackground();
		stoptime = 0;
		DrawGameList();

		// ×ボタン。
		MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
		MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

		// ○ボタン。
		MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
		MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 720, 110, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

		moveasp *= 0.9;
		if (moveasp < MIKAN_PI / 180.0) {
			moveasp = 0.0;
		}

		MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), MikanDraw->Color(255 * (atimer - time) / atimer, 255, 255, 255));

	} else if (timer < atimer * 3) {
		DrawBackground();
		stoptime = 0;
		DrawGameList();

		// ×ボタン。
		MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
		MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

		// ○ボタン。
		MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
		MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 720, 110, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

		moveasp *= 0.9;
		timer = atimer * 2;
		if (moveasp < MIKAN_PI / 180.0) {
			timer = atimer * 3;
			moveasp = 0.0;
			mode = UIMODE_SELECT;
		}
	}

	return 1;
}

int UI2015::DrawSelect(int acinput) {
	int atimer = 30;
	int input = 0;

	DrawBackground();

	DrawGameList();
	DrawGameTitle(game[selectgame]);

	if (moveasp == 0.0) {
		// 入力。
		if (acinput) {
			if (system->GetLeft() > 0 || system->GetL() > 0) {
				speedline = speedline_;
				MikanSound->Play(11, 0);
				moveasp = speedline;
				input = 1;
			} else if (system->GetRight() > 0 || system->GetR() > 0) {
				speedline = -speedline_;
				MikanSound->Play(11, 0);
				moveasp = speedline;
				input = 1;

			} else if (system->GetA() == 1) {
				MikanSound->Play(10, 0);
				if (game[selectgame] >= 0) {
					mode = UIMODE_INFO;
					infoy = 0;
				} else {
					if (game[selectgame] == -3) {
						// ランダムなら
						mode = UIMODE_INFO;
						infoy = 0;
						selectgame = 2 + rand() % (nowgames - 2);
					} else {
						// そうでない場合
						timer = 0;
						SetGames(game[selectgame] == -1 ? 9 : -3);
					}
				}
			} else if (game[0] == -2 && system->GetB() == 1) {
				timer = 0;
				MikanSound->Play(11, 0);
				selectgame = 0;
				SetGames(-2);
			} else if (system->GetY() == 1) {
				// 設定へ。
				sselect[0] = sselect[1] = sselect[2] = sselect[3] = 0;
				MikanSound->Play(11, 0);
				mode = UIMODE_SETTING;
			}
		}
	} else {
		// 入力は受け付けない。
		input = 1;
		if (abs(speedline) > 0.1) {
			speedline *= 0.9;
		}
		moveasp += speedline;
		if (MIKAN_PI / 6.0 < abs(moveasp))//stopline <= abs( moveasp ) )
		{
			moveasp = 0.0;
			// 選択ゲームの変更。
			if (speedline < 0) {
				selectgame = NextSelectNumber(selectgame);
			} else {
				selectgame = PrevSelectNumber(selectgame);
			}
		}
	}

	// メニューボタン。
	MikanDraw->DrawTextureScaling(tex, (int)(70 * scale), (int)(80 * scale), 0, 960, 160, 30, scale);

	// ×ボタン。
	MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
	MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

	// ○ボタン。
	MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
	MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 720, 110, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

	// 左右。
	MikanDraw->DrawTextureScalingC(tex, (int)(60 * scale), system->GetHeight() / 2, 820, 110, 110, 110, (int)(65 * scale), (int)(65 * scale), DRAW_LR);
	MikanDraw->DrawTextureScalingC(tex, (int)(system->GetWidth() - 60 * scale), system->GetHeight() / 2, 820, 110, 110, 110, (int)(65 * scale), (int)(65 * scale), 0);
	return input;
}

int UI2015::DrawSetting(void) {
	int i, max, cate;
	int basex, basew, basey;
	int lwide, rwide;
	int my;

	MikanDraw->DrawTextureScaling(tex, 0, 0, 820, 550, 110, 110, system->GetWidth(), system->GetHeight(), 0);

	basew = (int)(system->GetWidth() * 0.8);
	basex = (system->GetWidth() - basew) / 2;

	MikanDraw->DrawTextureScaling(tex, basex, 0, 820, 550, 110, 110, basew, system->GetHeight(), 0);

	MikanDraw->Printf(10, (int)(basex + basew / 2 - (40 * scale)), 0, 0xFFFFFFFF, "設定");

	// sselect
	// 0=今左右のどこか。
	// 1=左のどこか。
	// 2=右のどこか。
	// 3=選択肢の初めの位置。

	lwide = basew / 3;
	rwide = basew - lwide;

	// 選択済みメニュー。
	my = (int)(20 * scale + fontsize);
	MikanDraw->DrawBox(basex, my, lwide, fontsize, sselect[0] ? 0x7F7F7F00 : 0x7FFFFF00);

	// カテゴリ
	basey = (int)(20 * scale + fontsize);
	MikanDraw->Printf(10, basex, basey, 0xFFFFFFFF, "　カテゴリ");

	if (sselect[0]) {
		// 右側表示する。
		switch (sselect[1]) {
		case 0: // カテゴリ表示
			max = system->GetCategoryMax();
			for (i = sselect[3]; i < CATEGORY_MAX; ++i) {

				if (0 <= i && system->GetCategoryGames((cate = CATE_SORT[i])) == 0) {
					// そのカテゴリのゲームはない。
					continue;
				}
				if ((sselect[2] < 0 && i < 0) || i == sselect[2]) {
					// 選択中。
					MikanDraw->DrawBox(basex + lwide, basey, rwide, fontsize, 0x7FFFFF00);
				}
				if (i < 0) {
					MikanDraw->DrawTextureScaling(tex, basex + lwide, basey, 640, 0, 80, 80, fontsize, fontsize, 0);
					MikanDraw->Printf(10, basex + lwide + fontsize, basey, 0xFFFFFFFF, "すべて");
					MikanDraw->Printf(10, basex + basew - fontsize * 2, basey, 0xFFFFFFFF, "%4d", system->GetGameMax());
				} else {
					MikanDraw->Printf(10, basex + lwide + fontsize, basey, 0xFFFFFFFF, "%s", CATE_NAME[cate]);
					MikanDraw->DrawTextureScaling(tex, basex + lwide, basey, 640, 80 + cate * 80, 80, 80, fontsize, fontsize, 0);
					MikanDraw->Printf(10, basex + basew - fontsize * 2, basey, 0xFFFFFFFF, "%4d", system->GetCategoryGames(cate));
				}
				basey += fontsize;
			}
			if (-1 < sselect[2] && system->GetUp() % 30 == 1) {
				// 上行ける。
				MikanSound->Play(11, 0);
				while (0 <= --sselect[2]) {
					if (system->GetCategoryGames(CATE_SORT[sselect[2]])) {
						break;
					}
				}
				if (sselect[2] < sselect[3]) {
					sselect[3] = sselect[2];
				}
			} else if (system->GetDown() % 30 == 1) {
				MikanSound->Play(11, 0);
				i = sselect[2];
				while (++sselect[2] < CATEGORY_MAX) {
					if (system->GetCategoryGames(CATE_SORT[sselect[2]])) {
						break;
					}
				}
				if (CATEGORY_MAX <= sselect[2]) {
					sselect[2] = i;
				}
				/*if ( system->GetHeight() < basey - (11 - sselect[ 2 ] - 1) * fontsize )
				{
				++sselect[3];
				}*/
			}
			break;
		}
	} else {
		// 左側。
	}

	if (system->GetA() == 1) {
		MikanSound->Play(10, 0);
		if (sselect[0] == 0) {
			// 左側。
			sselect[0] = 1;
			switch (sselect[1]) {
			case 0:
				sselect[2] = sselect[3] = -1;
				break;
			}
		} else {
			// 右側。
			switch (sselect[1]) {
			case 0: // カテゴリ。
				mode = UIMODE_SELECT;
				SetGames(sselect[2] < 0 ? -2 : CATE_SORT[sselect[2]]);
				timer = 0;
				selectgame = 0;
				break;
			}
		}
	} else if (system->GetB() == 1) {
		MikanSound->Play(11, 0);
		if (sselect[0] == 0) {
			// 左側。
			mode = UIMODE_SELECT;
		} else {
			// 右側。
			sselect[0] = 0;
			sselect[2] = sselect[3] = 0;
		}
	} else if (system->GetY() == 1) {
		mode = UIMODE_SELECT;
		MikanSound->Play(11, 0);
	}

	if (mode == UIMODE_SELECT) {
		sselect[0] = sselect[1] = sselect[2] = sselect[3] = 0;
	}

	return 1;
}

// スクリーンショットのサイズを得る
void UI2015::GetSSSize(int *ssx, int *ssy, double *scale, int gamenum)
{
	// スクリーンショットの大きさ
	*ssx = MikanDraw->GetTextureWidth(system->GetGameSSTexture(gamenum));
	*ssy = MikanDraw->GetTextureHeight(system->GetGameSSTexture(gamenum));
	int ssnum = system->GetGameSSTMax(gamenum);
	if (ssnum <= 0)
	{
		ssnum = 1;
	}
	*ssy /= ssnum;

	// 大きさに合わせて拡大率を変更
	{
		double drawx = 640.0;
		double drawy = 480.0;
		// 横に長い場合
		if (drawy / drawx > (double)(*ssy) / (double)(*ssx))
		{
			*scale = drawx / (double)(*ssx);
		}
		// 縦に長い場合
		else
		{
			*scale = drawy / (double)(*ssy);
		}
	}
}

int UI2015::DrawInfomation(int acinput) {
	int dx, dy;
	int atimer = 10;
	unsigned long color;
	double scl, tmp;
	int i, max = 0;
	static int stTime = 0;

	// スクリーンショットの大きさ
	int ssx = 640;
	int ssy = 480;
	double size_scale = 1.0;
	GetSSSize(&ssx, &ssy, &size_scale, game[selectgame]);

	DrawBackground();

	dy = system->GetHeight() / 2;
	if (infoy < system->GetHeight()) {
		// 背景を染め上げる。
		if (category < 0) {
			color = (((unsigned char)(127 * infoy / system->GetHeight())) << 24) | CATE_COLOR[system->GetGameMainCategory(game[selectgame])];
			MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), color);
		}

		DrawGameList(0, (int)infoy);
		//if (infoy <= 0) { infoy = 1; } else { infoy *= 1.3; }
		if (infoy <= 0) {
			infoy = 1; infospeed = system->GetHeight() / 10;
		} else {
			infoy += infospeed; if (infospeed > 2) {
				infospeed *= 0.93;
			}
		}
		if (infoy > system->GetHeight()) {
			infoy = system->GetHeight();
		}

		// 中央。
		dx = (int)(radius * sin(moveasp));
		tmp = DrawBox(dx, dy, game[selectgame], 1.4, (unsigned char)(255 * (1.0 - infoy / system->GetHeight())));
		//stoptime > atimer ? 1.4 : 1.0 + ((double)stoptime / atimer) * 0.4);
		scl = (system->GetHeight() - (110 + 60)*scale) / 480.0;

		if (system->GetGameSSTMax(game[selectgame]) > 1) {
			MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
				(int)(centerline + dx + 640 * (scl * infoy / system->GetHeight())),
				(int)(dy + 25 * scale * infoy / system->GetHeight()),
				0, ssy, ssx, ssy,
				(tmp + (scl - tmp) * infoy / system->GetHeight()) * 0.9 * size_scale);
		}

		MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
			(int)(centerline + dx), (int)(dy + 25 * scale * infoy / system->GetHeight()), 0,
			0, ssx, ssy, (tmp + (scl - tmp) * infoy / system->GetHeight()) * size_scale);

		// ゲーム情報。
		dx = 0;
		dy = (int)(system->GetHeight() - scale * 50 + (system->GetHeight() - infoy) / 10);

		// ×ボタン。
		MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
		MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

		// ○ボタン。
		MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
		MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 110, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

		MikanDraw->DrawBox(dx, dy, system->GetWidth(), system->GetHeight(), 0xFFFFFFFF);
		dx += (int)(20 * scale);
		dy += (int)(5 * scale);
		
		MikanDraw->Printf(10, 0, dy, system->GetGameInfo(game[selectgame]));

		// 選択画像を0にする。
		infonum = 0;
		infox = 0;
		if (infoy >= system->GetHeight()) {
			infospeed = 0;
			stTime = 0;
		}
	} else {
		// 背景を染め上げる。
		if (category < 0) {
			color = 0x7F000000 | CATE_COLOR[system->GetGameMainCategory(game[selectgame])];
			MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), color);
		}

		dx = (int)(radius * sin(moveasp));
		scl = (system->GetHeight() - (110 + 60)*scale) / 480.0;

		max = system->GetGameSSTMax(game[selectgame]);
		//tmp = 1.0 + (scl - 1.0) * infoy / system->GetHeight();
		for (i = -2; i <= 2; ++i) {
			if (0 <= infonum + i && infonum + i < max) {
				MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
					(int)(centerline + dx + 640 * scl * i + infox), (int)(dy + 25 * scale * infoy / system->GetHeight()),
					0, ssy * (infonum + i), ssx, ssy,
					//boxsize * tmp * 1.4, boxsize * tmp * 480.0 / 640.0 * 1.4, 0
					scl * (1.0 - 0.1*fabs(640 * scl * i + infox) / (640 * scl))* size_scale
					);
				/*MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
				(int)(centerline + dx + 640 * scl * i + infox), (int)(dy + 25 * scale * infoy / system->GetHeight()),
				0, 480 * (infonum+i), 640, 480,
				scl * (1.0 - 0.1*fabs(640 * scl * i + infox) / (640 * scl))
				);*/
			}
		}

		/*tmp = 1.0 + (scl - 1.0) * infoy / system->GetHeight();
		MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
		(int)(centerline + dx), (int)(dy + 25 * scale * infoy / system->GetHeight()), 0, 0, 640, 480, boxsize * tmp * 1.4, boxsize * tmp * 480.0 / 640.0 * 1.4, 0);*/

		//MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
		//  centerline + dx, dy + 25 * scale, 0, 0, 640, 480, scl);

		// ゲーム情報。
		dx = 0;
		dy = (int)(system->GetHeight() - scale * 50);
		// ×ボタン。
		MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
		MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

		// ○ボタン。
		MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
		MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 350, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

		MikanDraw->DrawBox(dx, dy, system->GetWidth(), system->GetHeight(), 0xFFFFFFFF);
		dx += (int)(20 * scale);
		dy += (int)(5 * scale);

		// 半角換算で文字数取得
		int a = strlen(system->GetGameInfo(game[selectgame]));
		int handlepx = static_cast<int>(fontsize / 90.0) * 32;

		if (a * handlepx < system->GetWidth()) {
			MikanDraw->Printf(10, 0, dy, "%s", system->GetGameInfo(game[selectgame]));
		} else {
			MikanDraw->CreateTexture(250, a * handlepx + 400, 100);
			MikanDraw->SetRenderTarget(250);
			MikanDraw->Printf(10, 0, 0, "%s", system->GetGameInfo(game[selectgame]));
			MikanDraw->SetRenderTarget();

			int sabun = a * handlepx - system->GetWidth();
			int t = (stTime * 4);
			MikanDraw->DrawTexture(250, 0, dy, t, 0, system->GetWidth(), 100);
			++stTime;
		}

		if (infospeed == 0) {
			if (acinput) {
				if (system->GetRight() == 1 || system->GetR() == 1) {
					if (infonum + 1 < max) {
						infospeed = -640 * scl / 8;
						MikanSound->Play(11, 0);
					}
				} else if (system->GetLeft() == 1 || system->GetL() == 1) {
					if (0 < infonum) {
						infospeed = 640 * scl / 8;
						MikanSound->Play(11, 0);

					}
				}
			}
		} else {
			infox += infospeed;
			if (2 < fabs(infospeed)) {
				infospeed *= 0.9;
			}
			if (640 * scl < abs(infox)) {
				infox = 0;
				infonum += 1 - 2 * (infospeed > 0);
				infospeed = 0;
			}
		}

		// 左右。
		if (infonum > 0) {
			MikanDraw->DrawTextureScalingC(tex, (int)(60 * scale), system->GetHeight() / 2, 820, 110, 110, 110, (int)(65 * scale), (int)(65 * scale), DRAW_LR);
		}
		if (infonum + 1 < max) {
			MikanDraw->DrawTextureScalingC(tex, (int)(system->GetWidth() - 60 * scale), system->GetHeight() / 2, 820, 110, 110, 110, (int)(65 * scale), (int)(65 * scale), 0);
		}
	}

	DrawGameTitle(game[selectgame]);

	if (acinput && infoy >= system->GetHeight()) {
		if (system->GetB() == -1 ) {
			mode = UIMODE_INFOBACK;//UIMODE_SELECT;
			MikanSound->Play(11, 0);
			infox = 0;
			infospeed = 0.0;
		} else if (system->GetA() == 1) {
			mode = UIMODE_EXEC;
			MikanSound->Play(10, 0);
			ExecGame(game[selectgame]);
		}
	}

	return 1;
}

int UI2015::DrawInfomationBack(void) {
	int dx, dy;
	int atimer = 10;
	unsigned long color;
	double scl, tmp;
	int i, max;

	// スクリーンショットの大きさ
	int ssx = 640;
	int ssy = 480;
	double size_scale = 1.0;
	GetSSSize(&ssx, &ssy, &size_scale, game[selectgame]);

	DrawBackground();

	dy = system->GetHeight() / 2;

	if (infonum > 0) {
		// infonumを0に戻す。

		// 背景を染め上げる。
		if (category < 0) {
			color = 0x7F000000 | CATE_COLOR[system->GetGameMainCategory(game[selectgame])];
			MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), color);
		}
		dx = (int)(radius * sin(moveasp));
		scl = (system->GetHeight() - (110 + 60)*scale) / 480.0;

		max = system->GetGameSSTMax(game[selectgame]);
		for (i = -2; i <= 2; ++i) {
			if (0 <= infonum + i && infonum + i < max) {
				MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
					(int)(centerline + dx + 640 * scl * i + infox), (int)(dy + 25 * scale * infoy / system->GetHeight()),
					0, ssy * (infonum + i), ssx, ssy,
					scl * (1.0 - 0.1*fabs(640 * scl * i + infox) / (640 * scl)) * size_scale
					);
			}
		}

		//MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
		//  centerline + dx, dy + 25 * scale, 0, 0, 640, 480, scl);

		// ゲーム情報。
		dx = 0;
		dy = (int)(system->GetHeight() - scale * 50);
		// ×ボタン。
		MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
		MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

		// ○ボタン。
		MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
		MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 110, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

		MikanDraw->DrawBox(dx, dy, system->GetWidth(), system->GetHeight(), 0xFFFFFFFF);
		dx += (int)(20 * scale);
		dy += (int)(5 * scale);
		MikanDraw->Printf(10, dx, dy, system->GetGameInfo(game[selectgame]));

		if (infospeed == 0.0) {
			infospeed = 640 * scl / 8;
		} else {
			infox += infospeed;
			if (2 < fabs(infospeed)) {
				infospeed *= 0.9;
			}
			if (640 * scl < abs(infox)) {
				infox = 0;
				infonum += 1 - 2 * (infospeed > 0);
				infospeed = 0;
			}
		}

	} else if (0 < infoy) {
		// 背景を染め上げる。
		if (category < 0) {
			color = (((unsigned char)(127 * infoy / system->GetHeight())) << 24) | CATE_COLOR[system->GetGameMainCategory(game[selectgame])];
			MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), color);
		}

		DrawGameList(0, (int)infoy);
		if (infoy >= system->GetHeight()) {
			infoy = system->GetHeight() - 1;
			infospeed = system->GetHeight() / 10;
			//infospeed = 10;
		} else {
			infoy -= infospeed;
			if (infospeed > 2) {
				infospeed *= 0.93;
			}
			if (infoy < 0) {
				infoy = 0;
			}
		}

		// 中央。
		dx = (int)(radius * sin(moveasp));
		tmp = DrawBox(dx, dy, game[selectgame], 1.4, (unsigned char)(255 * (1.0 - infoy / system->GetHeight())));
		//stoptime > atimer ? 1.4 : 1.0 + ((double)stoptime / atimer) * 0.4);
		scl = (system->GetHeight() - (110 + 60)*scale) / 480.0;

		if (system->GetGameSSTMax(game[selectgame]) > 1) {
			MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
				(int)(centerline + dx + 640 * (scl * infoy / system->GetHeight())),
				(int)(dy + 25 * scale * infoy / system->GetHeight()),
				0, ssy, ssx, ssy, (tmp + (scl - tmp) * infoy / system->GetHeight()) * 0.9 * size_scale);
		}

		//tmp = tmp + (scl - tmp) * infoy / system->GetHeight();
		tmp = 1.0 + (scl - 1.0) * infoy / system->GetHeight();
		MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
			(int)(centerline + dx), (int)(dy + 25 * scale * infoy / system->GetHeight()),
			0, 0, ssx, ssy,
			(int)(boxsize * tmp * 1.4 * size_scale) * ssx / 640,
			(int)(boxsize * tmp * 480.0 / 640.0 * 1.4 * size_scale) * ssy / 480, 0);

		// ゲーム情報。
		dx = 0;
		dy = (int)(system->GetHeight() - scale * 50 + (system->GetHeight() - infoy) / 10);
		// ×ボタン。
		MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
		MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

		// ○ボタン。
		MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
		MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 110, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

		MikanDraw->DrawBox(dx, dy, system->GetWidth(), system->GetHeight(), 0xFFFFFFFF);
		dx += (int)(20 * scale);
		dy += (int)(5 * scale);
		MikanDraw->Printf(10, dx, dy, system->GetGameInfo(game[selectgame]));

		// 選択画像を0にする。
		infonum = 0;
		infox = 0;
		if (infoy < 0) {
			infospeed = 0;
		}
	} else {
		DrawGameList();
		// 中央。
		dx = (int)(radius * sin(moveasp));
		tmp = DrawBox(dx, dy, game[selectgame], 1.4, (unsigned char)(255 * (1.0 - infoy / system->GetHeight())));
		//stoptime > atimer ? 1.4 : 1.0 + ((double)stoptime / atimer) * 0.4);
		scl = (system->GetHeight() - (110 + 60)*scale) / 480.0;

		// 問題箇所？

		tmp = tmp + (scl - tmp) * infoy / system->GetHeight();
		//MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
		//  (int)(centerline + dx), (int)(dy + 25 * scale * infoy / system->GetHeight()), 0, 0, 640, 480, 640 * tmp, 480 * tmp,0);

		dy = system->GetHeight();
		// ×ボタン。
		MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
		MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

		// ○ボタン。
		MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
		MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 110, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

		mode = UIMODE_SELECT;
		stoptime = atimer;
	}

	DrawGameTitle(game[selectgame]);

	return 0;
}


int UI2015::PrevSelectNumber(int select) {
	return select <= 0 ? (nowgames - 1) : (select - 1);
}
int UI2015::NextSelectNumber(int select) {
	return (select + 1) % nowgames;
}

struct EXECGAME {
	class TenLANSystem *system;
	int gamenum;
};

int ExecGame(void *arg);

int UI2015::ExecGame(int gamenum) {
	struct EXECGAME *data;
	data = (struct EXECGAME *)calloc(1, sizeof(EXECGAME));
	data->system = system;
	data->gamenum = gamenum;

	MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), 0xFF000000);
	MikanSystem->WaitNextFrame(1);

	MikanSystem->RunThread(THREAD_EXEC, ::ExecGame, (void *)data);
	return 0;
}

int UI2015::DrawBackground(void) {
	int w = 0;

	// 基本の背景
	do {
		MikanDraw->DrawTextureScaling(tex, w, 0, 0, 480, 640, 480, scale);
		w += (int)(scale * 640);
	} while (w < system->GetWidth());

	// カテゴリごとに色を変える
	if (category >= 0) {
		MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), 0x7F000000 | CATE_COLOR[category]);
	}

	// 曲に合わせた背景の表示
	if (curimage >= 0)
	{
		MikanDraw->DrawTextureScaling(image[curimage],
			static_cast<int>((system->GetWidth() - (640 * scale)) / 2), 0, 0, 0, 640, 480, scale);
	}

	return 0;
}

int UI2015::DrawGameTitle(int gamenum) {
	int cate, cate_;
	double per;
	int sx = 0, sy = 0;
	// カテゴリ背景。
	if (gamenum < 0) {
		cate = CATEGORY_MAX - 2; // ミニゲーム。
	} else {
		cate = system->GetGameMainCategory(gamenum);
	}
	MikanDraw->SetColor(tex, 0xFF000000 | CATE_COLOR[cate]);
	MikanDraw->DrawTextureScaling(tex, 0, 0, 830, 0, 110, 110, scale);
	MikanDraw->SetColor(tex, 0xFFFFFFFF);

	// カテゴリアイコン。
	sy = (int)(scale * 5 * cos(abs(timer % 480 - 240) / 240.0 * MIKAN_PI * 2.0));
	MikanDraw->DrawTextureRotateScaleC(tex,
		(int)(sx + 80 * scale / 2), (int)(sy + 80 * scale / 2), 640, 80 * (cate + 1), 80, 80, scale, MIKAN_PI / 64 * sin(abs(timer % 240) / 240.0 * MIKAN_PI * 2.0));
	if (moveasp != 0.0) {
		// 移動中のため次に表示するカテゴリアイコンをうっすら表示。
		if (speedline < 0) {
			cate_ = NextSelectNumber(selectgame);
		} else {
			cate_ = PrevSelectNumber(selectgame);
		}
		if (game[cate_] < 0) {
			cate_ = CATEGORY_MAX - 2; // ミニゲーム。
		} else {
			cate_ = system->GetGameMainCategory(game[cate_]);
		}
		if (MIKAN_PI / 6.0 < abs(moveasp)) {
			per = MIKAN_PI / 6.0;
		}
		per = abs(moveasp) / (MIKAN_PI / 6.0);

		MikanDraw->SetColor(tex, MikanDraw->Color((unsigned char)(255 * per), 0, 0, 0) | CATE_COLOR[cate_]);
		MikanDraw->DrawTextureScaling(tex, 0, 0, 830, 0, 110, 110, scale);
		MikanDraw->SetColor(tex, MikanDraw->Color((unsigned char)(255 * per), 255, 255, 255));
		MikanDraw->DrawTextureRotateScaleC(tex,
			(int)(sx + 80 * scale / 2), (int)(sy + 80 * scale / 2), 640, 80 * (cate_ + 1), 80, 80, scale, MIKAN_PI / 64 * sin(abs(timer % 240) / 240.0 * MIKAN_PI * 2.0));
		MikanDraw->SetColor(tex, 0xFFFFFFFF);
	}

	// タイトル。
	MikanDraw->DrawTextureScaling(tex, (int)(110 * scale), 0, 940, 0, 40, 110, scale);
	MikanDraw->DrawTextureScaling(tex, (int)(150 * scale), 0, 980, 0, 40, 110, system->GetWidth() - 150, (int)(scale * 110), 0);
	if (gamenum < 0) {
		const char * strs[3] = { "ミニゲーム", "戻る", "ランダムセレクト" };
		int which = 0;
		switch (gamenum) {
		case -1: which = 0; break;
		case -2: which = 1; break;
		case -3: which = 2; break;
		}
		MikanDraw->Printf(10, (int)(140 * scale), (int)(20 * scale), strs[which]);
	} else {
		MikanDraw->Printf(10, (int)(140 * scale), (int)(20 * scale), system->GetGameTitle(gamenum));
	}
	// 前面。
	MikanDraw->DrawTextureScaling(tex, 0, 0, 720, 0, 110, 110, scale);

	return 0;
}

int UI2015::DrawGameList(int basex, int basey) {
	int dx, dy;
	int atimer = 10;
	dy = system->GetHeight() / 2;
	int select;

	select = PrevSelectNumber(PrevSelectNumber(PrevSelectNumber(selectgame)));
	dx = -(int)(radius * sin(MIKAN_PI / 2.0 - moveasp));
	DrawBox(basex + dx, basey + dy, game[select]);

	select = NextSelectNumber(select);
	dx = -(int)(radius * sin(MIKAN_PI / 3.0 - moveasp));
	DrawBox(basex + dx, basey + dy, game[select]);

	select = NextSelectNumber(select);
	dx = -(int)(radius * sin(MIKAN_PI / 6.0 - moveasp));
	DrawBox(basex + dx, basey + dy, game[select]);

	select = selectgame;

	select = NextSelectNumber(NextSelectNumber(NextSelectNumber(select)));
	dx = (int)(radius * sin(MIKAN_PI / 2.0 + moveasp));
	DrawBox(basex + dx, basey + dy, game[select]);

	select = PrevSelectNumber(select);
	dx = (int)(radius * sin(MIKAN_PI / 3.0 + moveasp));
	DrawBox(basex + dx, basey + dy, game[select]);

	select = PrevSelectNumber(select);
	dx = (int)(radius * sin(MIKAN_PI / 6.0 + moveasp));
	DrawBox(basex + dx, basey + dy, game[select]);

	// 中央。
	dx = (int)(radius * sin(moveasp));
	DrawBox(basex + dx, basey + dy, game[selectgame], stoptime > atimer ? 1.4 : 1.0 + ((double)stoptime / atimer) * 0.4);

	return 0;
}

double UI2015::DrawBox(int dx, int dy, int gamenum, double scl, unsigned char alpha) {
	double scale = 1.0, tmp;
	unsigned int texnum = tex;
	int dw, dh;
	unsigned int i;

	dx += (int)centerline;
	scale -= (abs(dx - centerline) / radius);
	dx -= (int)centerline;

	if (dx < -radius) {
		dx = (int)(centerline - dx - 2 * radius);
	} else if (radius < dx) {
		dx = (int)(centerline - dx + 2 * radius);
	}

	tmp = asin(dx / radius);
	scale = abs((sin(tmp + atan2(-radius, -boxsize / 2)) + sin(tmp + atan2(-radius, boxsize / 2))) / 2.0);
	if (gamenum != -3) {
		MikanDraw->DrawBoxC((int)(centerline + dx), dy, (int)(boxsize * scale * scl), (int)(boxsize * scl), (alpha << 24) | CATE_DCOLOR[gamenum < 0 ? (CATEGORY_MAX - 1) : system->GetGameMainCategory(gamenum)]);//color );
		MikanDraw->SetAlpha(tex, alpha);
	}
	// DVD収録マーク。
	tmp = (boxsize - (boxsize * 480.0 / 640)) * scl * 0.5;
	if (system->GetConfig()->IsViewDVD() && system->IsInDVD(gamenum)) {
		MikanDraw->DrawTextureScaling(tex,
			(int)(centerline + dx + (boxsize)* scl * scale * 0.5 - tmp * scale),
			(int)(dy - boxsize * scl * 0.5),
			820, 330, 110, 110, (int)(tmp * scale), (int)tmp, 0);
	}
	if (0 <= gamenum) {
		if (system->GetConfig()->IsViewNewGame() && system->IsNewGame(gamenum)) {
			// New Gameマーク。
			MikanDraw->DrawTextureScaling(tex,
				(int)(centerline + dx + (boxsize)* scl * scale * 0.5 - tmp * scale * (1 + system->GetConfig()->IsViewDVD())),
				(int)(dy - boxsize * scl * 0.5),
				820, 440, 110, 110, (int)(tmp * scale), (int)tmp, 0);
		}

		// カテゴリ一覧。
		if (system->GetConfig()->IsViewCategory()) {
			for (i = 0; i < system->GetGameCaetgoryMax(gamenum); ++i) {
				MikanDraw->DrawTextureScaling(tex,
					(int)(centerline + dx - (boxsize)* scl * scale * 0.5 + tmp * scale * i),
					(int)(dy + boxsize * scl * 0.5 - tmp),
					640, 80 + 80 * system->GetGameCategory(gamenum, i), 80, 80, (int)(tmp * scale), (int)tmp, 0);
			}
		}
	}
	MikanDraw->SetColor(tex, 0xFFFFFFFF);

	int ssx = 640;
	int ssy = 480;
	double size_scale = 1.0;
	if (gamenum >= 0) {
		texnum = system->GetGameSSTexture(gamenum);

		// スクリーンショットの大きさ
		GetSSSize(&ssx, &ssy, &size_scale, gamenum);
	}
	if (gamenum != -3) {
		MikanDraw->SetAlpha(texnum, alpha);
		MikanDraw->DrawTextureScalingC(texnum,
			(int)(centerline + dx), dy, 0, 0, ssx, ssy,
			(int)(boxsize * scale * scl * size_scale) * ssx / 640,
			(int)(boxsize * scl * 480.0 / 640.0 * size_scale) * ssy / 480, 0);
	}
	if (gamenum == -1) {
		// ミニゲーム。
		dw = (int)(boxsize * scl * 80 / 640);
		dh = (int)(boxsize * 480 / 640 * scale * scl);
		MikanDraw->DrawTextureRotateScaleC(texnum,
			(int)(centerline + dx), dy, 930, 510, 80, 400, dw + dw % 2, dh + dh % 2, MIKAN_PI / 2.0, 0);
	} else if (gamenum == -2) {
		// 戻る。
		dw = (int)(boxsize * scl * 80 / 640);
		dh = (int)(boxsize * 480 / 640 * scale * scl);
		MikanDraw->DrawTextureRotateScaleC(texnum,
			(int)(centerline + dx), dy + dy % 2, 930, 110, 80, 400, dw + dw % 2, dh + dh % 2, MIKAN_PI / 2.0, 0);
	} else if (gamenum == -3) {
		// 戻る。
		dw = (int)(boxsize * scale * scl);
		dh = (int)(boxsize * scl);
		MikanDraw->DrawTextureRotateScaleC(ranTex,
			(int)(centerline + dx), dy + dy % 2, 0, 0, 400, 400, dw + dw % 2, dh + dh % 2, 0, 0);
	}

	MikanDraw->SetColor(texnum);

	MikanDraw->DrawBoxC((int)(centerline + dx), dy, (int)(boxsize * scale * scl), (int)(boxsize * scl), MikanDraw->Color(64 * alpha / 255, (unsigned char)(255 * scale), (unsigned char)(255 * scale), (unsigned char)(255 * scale)));//color );

	//MikanDraw->DrawBoxC( centerline + dx, dy, boxsize * scale, boxsize * 0.9, MikanDraw->Color( 255, 255 * scale, 0, 0 ) );//color );
	return boxsize * scale * scl / 640.0;
}

void UI2015::CleanUp(void) {
	free(game);
}
