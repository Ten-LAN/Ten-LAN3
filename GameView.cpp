#include "TenLAN.h"

unsigned long CATE_COLOR[ CATEGORY_MAX ] =
{
  0x00ff2828, // �A�N�V����
  0x00ff0091, // �V���[�e�B���O
  0x00dbae00, // �X�|�[�c
  0x00c2cb10, // ���[�X
  0x0016f000, // �p�Y��
  0x0000a411, // �e�[�u��
  0x00008f9a, // �V�~�����[�V����
  0x00004eff, // ���[���v���C���O
  0x00cf00ff, // �ΐ�
  0x00f400ff, // �~�j�Q�[��
  0x00808080, // ���̑�
};

unsigned long CATE_DCOLOR[CATEGORY_MAX] =
{
  0x00db4c4c, // �A�N�V����
  0x00d52a8a, // �V���[�e�B���O
  0x00b69824, // �X�|�[�c
  0x00b7bf1c, // ���[�X
  0x0034bb25, // �p�Y��
  0x001b8927, // �e�[�u��
  0x001a7980, // �V�~�����[�V����
  0x002a5ed5, // ���[���v���C���O
  0x00a93fc1, // �ΐ�
  0x00bb3ec1, // �~�j�Q�[��
  0x00575757, // ���̑�
};

char CATE_NAME[CATEGORY_MAX][32] =
{
  "�A�N�V����",
  "�V���[�e�B���O",
  "�X�|�[�c",
  "���[�X",
  "�p�Y��",
  "�e�[�u��",
  "�V�~�����[�V����",
  "���[���v���C���O",
  "�ΐ�",
  "�~�j�Q�[��",
  "���̑�",
};

int CATE_SORT[CATEGORY_MAX] =
{
  0,  // �A�N�V����
  1,  // �V���[�e�B���O
  4,  // �p�Y��
  6,  // �V�~�����[�V����
  // ���Y��
  3,  // ���[�X
  7,  // ���[���v���C���O
  5,  // �e�[�u��
  2,  // �X�|�[�c
  8,  // �ΐ�
  9,  // �~�j�Q�[��
  10, // ���̑�
};

int TenLAN_Loading_Thread_Funciton( void *arg )
{
  class StartUp *view = (class StartUp *)arg;
  return view->Loading();
}

int StartUp::Loading( void )
{
  gamemax = system->InitGamelist( gamemax, &loadgame );

  return 0;
}

void StartUp::UserInit( void )
{
  MikanDraw->LoadFontFile( NULL, "SYSFONT" );
  MikanDraw->CreateFont( 0, "MigMix 1M", 40, 0xFFFFFFFF );
  MikanDraw->CreateTexture( 0, NULL, "SYSIMG", TRC_NONE );

  gamemax = system->GetGamedirDirectorys();
  loadgame = 0;
  timer = -1;

  MikanSystem->RunThread(THREAD_LOADING, TenLAN_Loading_Thread_Funciton, (void *)this);
  rad = 0;
  int mode = 0;
}

void StartUp::Resume( void )
{
}

int StartUp::MainLoop( void )
{
  int atimer = 30, time;
  double rad_ = (MIKAN_PI*2.0*rad / 360);

  MikanDraw->ClearScreen();

  if ( loadgame < gamemax )
  {
    //MikanDraw->DrawTextureScalingC( 0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 160, 320, 160, 2.0 );
    MikanDraw->DrawTextureRotateScaleC(0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 390, 160, 110, 110, 2.0, rad_); // ����
    MikanDraw->DrawTextureScalingC(0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 160, 320, 160, 2.0);
    MikanDraw->DrawTextureRotateScaleC(0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 320, 160, 70, 70, 2.0, rad_); // e

    DrawGameState();
    rad = ( rad + 6 ) % 360;
  } else if ( timer < 0  )
  {
    //MikanDraw->DrawTextureScalingC( 0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 0, 320, 160, 2.0 );
    rad = (rad + 6) % 360;
    MikanDraw->DrawTextureRotateScaleC(0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 390, 160, 110, 110, 2.0, rad_); // ����
    MikanDraw->DrawTextureScalingC(0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 160, 320, 160, 2.0);
    MikanDraw->DrawTextureRotateScaleC(0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 320, 160, 70, 70, 2.0, rad_); // e
    timer = 0;
    DrawGameState();
  } else if ( timer >= 0 )
  {
    //MikanSystem->Join( 0 );

    if ( rad != 0 )
    {
      DrawGameState();
      rad = (rad + 6) % 360;
      MikanDraw->DrawTextureRotateScaleC(0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 390, 160, 110, 110, 2.0, rad_); // ����
      MikanDraw->DrawTextureScalingC(0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 160, 320, 160, 2.0);
      MikanDraw->DrawTextureRotateScaleC(0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 320, 160, 70, 70, 2.0, rad_); // e
    } else
    {
      if ( mode == 1 )
      {
        if (timer < atimer)
        {
          MikanDraw->DrawTextureScalingC(0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 0, 320, 160, 2.0);
          MikanDraw->SetAlpha(0, 255 * timer / atimer);
          MikanDraw->DrawTextureScalingC(0, system->GetWidth() / 2, system->GetHeight() / 2, 320, 0, 320, 160, 2.0);
          MikanDraw->SetColor(0, 0xFFFFFFFF);
        } else if (timer < atimer * 2)
        {
          time = timer - atimer;
          MikanDraw->SetAlpha(0, 255 * (atimer - time) / atimer);
          MikanDraw->DrawTextureScalingC(0, system->GetWidth() / 2, system->GetHeight() / 2, 320, 0, 320, 160, 2.0);
          MikanDraw->SetColor(0, 0xFFFFFFFF);
        } else
        {
          system->SetGameView(new OP());
        }
        ++timer;
      } else if (MikanInput->GetKeyNum(K_Z) == -1)
      {
        mode = 1;
      } else
      {
        DrawGameState();
        MikanDraw->DrawTextureRotateScaleC(0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 390, 160, 110, 110, 2.0, 0); // ����
        MikanDraw->DrawTextureScalingC(0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 160, 320, 160, 2.0);
        MikanDraw->DrawTextureRotateScaleC(0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 320, 160, 70, 70, 2.0, 0); // e
      }
    }

  } else
  {
    //MikanDraw->DrawTextureScalingC( 0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 0, 320, 160, 2.0 );
    MikanDraw->DrawTextureRotateScaleC(0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 390, 160, 110, 110, 2.0, rad_); // ����
    MikanDraw->DrawTextureScalingC(0, system->GetWidth() / 2, system->GetHeight() / 2, 0, 160, 320, 160, 2.0 );
    MikanDraw->DrawTextureRotateScaleC(0, system->GetWidth() / 2 - 170, system->GetHeight() / 2 + 24, 320, 160, 70, 70, 2.0, rad_ ); // e
    DrawGameState();
    rad = (rad + 6) % 360;
  }

  return 0;
}

int StartUp::DrawGameState(void)
{
  int i, max;

  max = loadgame;

  for (i = 0 ; i < max; ++i )
  {
    MikanDraw->Printf( 0, 0, i * 80, "[%03d/%03d] - %sSS[%d]", i, gamemax, system->GetGameTitle( i ), system->GetGameSSTexture( i ) );
  }

  return 0;
}

void StartUp::CleanUp( void )
{
  //MikanDraw->ReleaseTexture( 0 );
}

// BackGround

void BackGround::UserInit( void ){}
int timer = 0;
int BackGround::MainLoop( void )
{
  MikanDraw->ClearScreen();
  //MikanDraw->Printf(0,0,0,"%d,%d,%d",++timer,MikanInput->GetKeyNum(K_X),MikanInput->GetPadNum(0,PAD_C));

  system->ESCCannon();
  /*if (MikanInput->GetPadNum(0, PAD_I) > 1 && MikanInput->GetPadNum(0, PAD_J) > 1)
  {
    MikanDraw->Printf(0,0,0,"push");
    _MikanInput->SendKey( K_ESC, 1 );
    //Sleep( 10 );
    //_MikanInput->SendKey(K_ESC, 0);
  }*/

  return 0;
}

void BackGround::CleanUp( void )
{
  
}

// OP

void OP::UserInit( void )
{
  timer = 0;
  //tex = system->AddTexture( "./material/op.png" );
#ifndef _DEBUG
  //if (system->GetMode())
  //{
    MikanSound->Load(0, NULL, "MAINTHEME");
    MikanSound->Play(0, true);
    MikanSound->SetVolume( 0, 80 );
  //}
#endif
}

void OP::Resume( void )
{
}

int OP::MainLoop( void )
{
  MikanDraw->ClearScreen();

  //MikanDraw->DrawTextureScalingC( tex, system->GetWidth() / 2, system->GetHeight() / 2, 0, 0, 512, 512, 1.0 );
  MikanDraw->Printf( 0, 0, 0, "�L�[�e�X�g\n��(UP)%-4d ��(DOWN)%-4d ��(LEFT)%-4d ��(RIGHT)%-4d\n��(Z)%-4d �w(X)%-4d ��(A)%-4d ��(S)%-4d\nL1(D)%-4d R1(C)%-4d",
    MikanInput->GetKeyNum( K_UP ), MikanInput->GetKeyNum( K_DOWN ), MikanInput->GetKeyNum( K_LEFT ), MikanInput->GetKeyNum( K_RIGHT ),
    MikanInput->GetKeyNum( K_Z ), MikanInput->GetKeyNum( K_X ), MikanInput->GetKeyNum( K_A ), MikanInput->GetKeyNum( K_S ),
    MikanInput->GetKeyNum( K_D ), MikanInput->GetKeyNum( K_C )
    );
  if (system->GetGameMax() > 0)
  {
    if (MikanInput->GetKeyNum(K_Z) == -1)
    {
      system->SetGameView(new UIDefault());
    }
  } else
  {
    // �N���ł���Q�[�����Ȃ��B
    MikanDraw->Printf( 0, system->GetWidth() / 2 - 40 * 17 / 2, system->GetHeight() / 2, "�N���ł���Q�[��������܂���B\nESC�������ŏI���B" );
  }
  return 0;
}

void OP::CleanUp( void )
{
}

// DefaultSelect

int UIDefault::SetGames( int category )
{
  this->category = category;
  if ( category == -1 )
  {
    // �S�āB
    nowgames = system->GetGameNumbers( game, gamemax );
  } else if( category == -2 )
  {
    // �~�j�Q�[����1�ɂ܂Ƃ߁A�c��͕��ʂɕ\���B
    if ( minigamemax > 0 )
    {
      nowgames = system->GetGameNumbersNotCategory( 9, game + 1, gamemax ) + 1;
      game[ 0 ] = -1; // �~�j�Q�[���̂݁B
    } else
    {
      nowgames = system->GetGameNumbers( game, gamemax );
    }
  } else if( 0 <= category )
  {
    // ���̃J�e�S���̂݁B
    nowgames = system->GetGameNumbers( category, game + 1, gamemax ) + 1;
    game[0] = -2;
  }
  return 0;
}

void UIDefault::UserInit( void )
{
  tex = system->AddTexture( NULL, "DEFAULTIMG" );

  gamemax = system->GetGameMax();
  minigamemax = system->GetGameMax( 9 );

  if ( gamemax > 0 )
  {
    game = (int *)calloc( gamemax + 1, sizeof( int ) );
  } else
  {
    game = NULL;
  }

  scale = system->GetHeight() / 480.0;

  category = -2;
  SetGames( category );

  boxsize = (int)(system->GetHeight() / 2.0 - system->GetHeight() / 20.0);

  timer = 0;

  moveasp = MIKAN_PI;

  centerline = system->GetWidth() / 2.0;
  radius = centerline * 0.9;
  speedline = speedline_ = MIKAN_PI / 90.0;//radius /20.0;// 10.0;
  stopline = radius * abs( cos( MIKAN_PI / 3.0 ) );

  stoptime = 0;

  selectgame = 0;

  fontsize = (int)(40 * scale);
  MikanDraw->CreateFont( 10, "MigMix 1M", fontsize, 0xFF000000 );

  mode = UIMODE_OTHER;
}

void UIDefault::Resume( void )
{
  timer = 0;
  mode = UIMODE_SELECT;
  if (!MikanSound->IsPlaying(0))
  {
    MikanSound->Play(0, 1);
  }
}

int UIDefault::MainLoop( void )
{
  int atimer = 30;
  int input = 0;
  int tmp;

  MikanDraw->ClearScreen();

  if( timer < atimer * 3 )
  {
    input = DrawBeginSelect();
  } else
  {
    switch( mode )
    {
    case UIMODE_SELECT:
      input = DrawSelect();
      break;
    case UIMODE_SETTING:
      // ���͂𖳌������ĕ`�悾�����Ă��炤�B
      DrawSelect( 0 );
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

  if ( input == 0 ){ ++stoptime; } else { stoptime = 0; }
  ++timer;

  if ( system->GetMode() == 0 )
  {
    MikanDraw->Printf( 0, 30, MikanDraw->GetScreenHeight( 1 ) - 100, 0xFF000000, "ESC�������ŏI��" );
  }

  return 0;
}

int UIDefault::DrawBeginSelect( void )
{
  int atimer = 30, time, tmp;

  tmp = MikanSound->GetVolume( 0 );
  if ( tmp < 80 )
  {
    if (tmp == 0) { tmp = 1; }
    tmp *= 2;
    if (100 < tmp) { tmp = 80; }
    MikanSound->SetVolume( 0, tmp );
  }

  if (timer < atimer)
  {
    MikanDraw->DrawBox( 0, 0, system->GetWidth(), system->GetHeight(), MikanDraw->Color( 255 * timer / atimer, 255, 255, 255 ) );
  } else if( timer < atimer * 2 )
  {
    time = timer - atimer;
    DrawBackground();
    stoptime = 0;
    DrawGameList();

    // �~�{�^���B
    MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
    MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

    // ���{�^���B
    MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
    MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 720, 110, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

    moveasp *= 0.9;
    if (moveasp < MIKAN_PI / 180.0){ moveasp = 0.0; }

    MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), MikanDraw->Color(255 * (atimer - time) / atimer, 255, 255, 255));

  } else if( timer < atimer * 3 )
  {
    DrawBackground();
    stoptime = 0;
    DrawGameList();

    // �~�{�^���B
    MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
    MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

    // ���{�^���B
    MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
    MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 720, 110, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

    moveasp *= 0.9;
    timer = atimer * 2;
    if( moveasp < MIKAN_PI / 180.0 )
    {
      timer = atimer * 3;
      moveasp = 0.0;
      mode = UIMODE_SELECT;
    }
  }

  return 1;
}

int UIDefault::DrawSelect(int acinput)
{
  int atimer = 30;
  int input = 0;

  DrawBackground();

  DrawGameList();
  DrawGameTitle( game[ selectgame ] );

  if ( moveasp == 0.0 )
  {
    // ���́B
    if (acinput)
    {
      if (system->GetLeft() > 0 || system->GetL() > 0)
      {
        speedline = speedline_;
        moveasp = speedline;
        input = 1;
      }
      else if (system->GetRight() > 0 || system->GetR() > 0)
      {
        speedline = -speedline_;
        moveasp = speedline;
        input = 1;

      } else if (system->GetA() == 1)
      {
        if (game[selectgame] >= 0)
        {
          mode = UIMODE_INFO;
          infoy = 0;
        } else
        {
          timer = 0;
          SetGames(game[selectgame] == -1 ? 9 : -2);
        }
      } else if (game[0] == -2 && system->GetB() == 1)
      {
        timer = 0;
        selectgame = 0;
        SetGames(-2);
      } else if ( system->GetY() == 1 )
      {
        // �ݒ�ցB
        sselect[0] = sselect[1] = sselect[2] = sselect[3] = 0;
        mode = UIMODE_SETTING;
      }
    }
  } else
  {
    // ���͎͂󂯕t���Ȃ��B
    input = 1;
    if ( abs( speedline ) > 0.1 ){ speedline *= 0.9; }
    moveasp += speedline;
    if ( MIKAN_PI / 6.0 < abs( moveasp ) )//stopline <= abs( moveasp ) )
    {
      moveasp = 0.0;
      // �I���Q�[���̕ύX�B
      if ( speedline < 0 )
      {
        selectgame = NextSelectNumber( selectgame );
      } else
      {
        selectgame = PrevSelectNumber( selectgame );
      }
    }
  }

  // ���j���[�{�^���B
  MikanDraw->DrawTextureScaling( tex, (int)(70 * scale), (int)(80 * scale), 0, 960, 160, 30, scale );

  // �~�{�^���B
  MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
  MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

  // ���{�^���B
  MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
  MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(system->GetHeight() - 60 * scale * 0.5), 720, 110, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

  // ���E�B
  MikanDraw->DrawTextureScalingC( tex, (int)(60 * scale), system->GetHeight() / 2, 820, 110, 110, 110, (int)(65 * scale), (int)(65 * scale), DRAW_LR );
  MikanDraw->DrawTextureScalingC( tex, (int)(system->GetWidth() - 60 * scale), system->GetHeight() / 2, 820, 110, 110, 110, (int)(65 * scale), (int)(65 * scale), 0 );
  return input;
}

int UIDefault::DrawSetting( void )
{
  int i, max, cate;
  int basex, basew, basey;
  int lwide, rwide;
  int my;

  MikanDraw->DrawTextureScaling( tex, 0, 0, 820, 550, 110, 110, system->GetWidth(), system->GetHeight(), 0 );

  basew = (int)(system->GetWidth() * 0.8);
  basex = (system->GetWidth() - basew) / 2;

  MikanDraw->DrawTextureScaling(tex, basex, 0, 820, 550, 110, 110, basew, system->GetHeight(), 0);

  MikanDraw->Printf( 10, (int)(basex + basew / 2 - (40 * scale) ), 0, 0xFFFFFFFF, "�ݒ�" );

  // sselect
  // 0=�����E�̂ǂ����B
  // 1=���̂ǂ����B
  // 2=�E�̂ǂ����B
  // 3=�I�����̏��߂̈ʒu�B

  lwide = basew / 3;
  rwide = basew - lwide;

  // �I���ς݃��j���[�B
  my = (int)(20 * scale + fontsize);
  MikanDraw->DrawBox(basex, my, lwide, fontsize, sselect[0] ? 0x7F7F7F00 : 0x7FFFFF00);

  // �J�e�S��
  basey = (int)(20 * scale + fontsize);
  MikanDraw->Printf(10, basex, basey, 0xFFFFFFFF, "�@�J�e�S��");

  if (sselect[0])
  {
    // �E���\������B
    switch (sselect[1])
    {
    case 0: // �J�e�S���\��
      max = system->GetCategoryMax();
      for (i = sselect[3]; i < CATEGORY_MAX; ++i)
      {
        
        if (0 <= i && system->GetCategoryGames((cate = CATE_SORT[i])) == 0)
        {
          // ���̃J�e�S���̃Q�[���͂Ȃ��B
          continue;
        }
        if ((sselect[2] < 0 && i < 0) || i == sselect[ 2 ] )
        {
          // �I�𒆁B
          MikanDraw->DrawBox(basex + lwide, basey, rwide, fontsize, 0x7FFFFF00);
        }
        if (i < 0)
        {
          MikanDraw->DrawTextureScaling(tex, basex + lwide, basey, 640, 0, 80, 80, fontsize, fontsize, 0);
          MikanDraw->Printf(10, basex + lwide + fontsize, basey, 0xFFFFFFFF, "���ׂ�");
          MikanDraw->Printf(10, basex + basew - fontsize * 2, basey, 0xFFFFFFFF, "%4d", system->GetGameMax());
        } else
        {
          MikanDraw->Printf(10, basex + lwide + fontsize, basey, 0xFFFFFFFF, "%s", CATE_NAME[cate]);
          MikanDraw->DrawTextureScaling(tex, basex + lwide, basey, 640, 80 + cate * 80, 80, 80, fontsize, fontsize, 0);
          MikanDraw->Printf(10, basex + basew - fontsize * 2, basey, 0xFFFFFFFF, "%4d", system->GetCategoryGames( cate ));
        }
        basey += fontsize;
      }
      if ( -1 < sselect[ 2 ] && system->GetUp() % 30 == 1 )
      {
        // ��s����B
        while ( 0 <= --sselect[2])
        {
          if (system->GetCategoryGames(CATE_SORT[sselect[2]])){ break; }
        }
        if ( sselect[ 2 ] < sselect[ 3 ] )
        {
          sselect[3] = sselect[2];
        }
      } else if ( system->GetDown() % 30 == 1  )
      {
        i = sselect[2];
        while (++sselect[2] < CATEGORY_MAX)
        {
          if (system->GetCategoryGames(CATE_SORT[sselect[2]])){ break; }
        }
        if (CATEGORY_MAX <= sselect[2])
        {
          sselect[2] = i;
        }
        /*if ( system->GetHeight() < basey - (11 - sselect[ 2 ] - 1) * fontsize )
        {
          ++sselect[3];
        }*/
      }
      break;
    }
  } else
  {
    // �����B
  }

  if ( system->GetA() == 1 )
  {
    if ( sselect[ 0 ] == 0 )
    {
      // �����B
      sselect[0] = 1;
      switch (sselect[1])
      {
      case 0:
        sselect[2] = sselect[3] = -1;
        break;
      }
    } else
    {
      // �E���B
      switch ( sselect[ 1 ] )
      {
      case 0: // �J�e�S���B
        mode = UIMODE_SELECT;
        SetGames(sselect[2] < 0 ? -2 : CATE_SORT[sselect[2]]);
        timer = 0;
        selectgame = 0;
        break;
      }
    }
  } else if ( system->GetB() == 1 )
  {
    if (sselect[0] == 0)
    {
      // �����B
      mode = UIMODE_SELECT;
    } else
    {
      // �E���B
      sselect[0] = 0;
      sselect[2] = sselect[3]=0;
    }
  } else if (system->GetY() == 1)
  {
    mode = UIMODE_SELECT;
  }

  if (mode == UIMODE_SELECT)
  {
    sselect[0] = sselect[1] = sselect[2] = sselect[3] = 0;
  }

  return 1;
}

int UIDefault::DrawInfomation(int acinput)
{
  int dx, dy;
  int atimer = 10;
  unsigned long color;
  double scl, tmp;
  int i, max = 0;

  DrawBackground();

  dy = system->GetHeight() / 2;
  if (infoy < system->GetHeight() )
  {
    // �w�i����ߏグ��B
    if (category < 0)
    {
      color = (((unsigned char)(127 * infoy / system->GetHeight())) << 24) | CATE_COLOR[system->GetGameMainCategory(game[selectgame])];
      MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), color);
    }

    DrawGameList(0, (int)infoy);
    //if (infoy <= 0) { infoy = 1; } else { infoy *= 1.3; }
    if (infoy <= 0) { infoy = 1; infospeed = system->GetHeight() / 10; } else { infoy += infospeed; if ( infospeed > 2 ) { infospeed *= 0.93; } }
    if (infoy > system->GetHeight()) { infoy = system->GetHeight(); }

    // �����B
    dx = (int)(radius * sin(moveasp));
    tmp = DrawBox(dx, dy, game[selectgame], 1.4, (unsigned char)(255 * (1.0-infoy / system->GetHeight()) ) );
    //stoptime > atimer ? 1.4 : 1.0 + ((double)stoptime / atimer) * 0.4);
    scl = (system->GetHeight() - (110+60)*scale) / 480.0;

    if (system->GetGameSSTMax(game[selectgame]) > 1)
    {
      MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
        (int)(centerline + dx + 640 * (scl * infoy / system->GetHeight()) ),
        (int)(dy + 25 * scale * infoy / system->GetHeight()),
        0, 480, 640, 480, (tmp + (scl - tmp) * infoy / system->GetHeight()) * 0.9);
    }

    MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
      (int)(centerline + dx), (int)(dy + 25 * scale * infoy / system->GetHeight()), 0,
      0, 640, 480, tmp +(scl - tmp) * infoy / system->GetHeight() );

    // �Q�[�����B
    dx = 0;
    dy = (int)( system->GetHeight() - scale * 50 + (system->GetHeight()-infoy)/10 );

    // �~�{�^���B
    MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
    MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

    // ���{�^���B
    MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
    MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 110, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

    MikanDraw->DrawBox(dx, dy, system->GetWidth(), system->GetHeight(), 0xFFFFFFFF);
    dx += (int)(20 * scale);
    dy += (int)(5 * scale);
    MikanDraw->Printf(10, dx, dy, system->GetGameInfo(game[selectgame]));

    // �I���摜��0�ɂ���B
    infonum = 0;
    infox = 0;
    if (infoy >= system->GetHeight()) { infospeed = 0; }
  } else
  {
    // �w�i����ߏグ��B
    if (category < 0)
    {
      color = 0x7F000000 | CATE_COLOR[system->GetGameMainCategory(game[selectgame])];
      MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), color);
    }

    dx = (int)(radius * sin(moveasp));
    scl = (system->GetHeight() - (110 + 60)*scale) / 480.0;

    max = system->GetGameSSTMax(game[selectgame]);
    //tmp = 1.0 + (scl - 1.0) * infoy / system->GetHeight();
    for ( i = -2 ; i <= 2 ; ++i )
    {
      if (0 <= infonum + i && infonum + i < max)
      {
        MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
          (int)(centerline + dx + 640 * scl * i + infox), (int)(dy + 25 * scale * infoy / system->GetHeight()),
          0, 480 * (infonum + i), 640, 480,
          //boxsize * tmp * 1.4, boxsize * tmp * 480.0 / 640.0 * 1.4, 0
          scl * (1.0 - 0.1*fabs(640 * scl * i + infox) / (640 * scl))
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

    // �Q�[�����B
    dx = 0;
    dy = (int)(system->GetHeight() - scale * 50);
    // �~�{�^���B
    MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
    MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

    // ���{�^���B
    MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
    MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 350, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

    MikanDraw->DrawBox(dx, dy, system->GetWidth(), system->GetHeight(), 0xFFFFFFFF);
    dx += (int)(20 * scale);
    dy += (int)(5 * scale);
    MikanDraw->Printf(10, dx, dy, "%s", system->GetGameInfo( game[ selectgame ] ) );

    if (infospeed == 0)
    {
      if (acinput)
      {
        if (system->GetRight() == 1 || system->GetR() == 1)
        {
          if (infonum + 1 < max) { infospeed = -640 * scl / 8; }
        } else if (system->GetLeft() == 1 || system->GetL() == 1)
        {
          if (0 < infonum) { infospeed = 640 * scl / 8; }
        }
      }
    } else
    {
      infox += infospeed;
      if (2 < fabs(infospeed)) { infospeed *= 0.9; }
      if (640 * scl < abs(infox))
      {
        infox = 0;
        infonum += 1 - 2 * ( infospeed > 0 );
        infospeed = 0;
      }
    }

    // ���E�B
    if (infonum > 0)
    {
      MikanDraw->DrawTextureScalingC(tex, (int)(60 * scale), system->GetHeight() / 2, 820, 110, 110, 110, (int)(65 * scale), (int)(65 * scale), DRAW_LR);
    }
    if (infonum + 1 < max)
    {
      MikanDraw->DrawTextureScalingC(tex, (int)(system->GetWidth() - 60 * scale), system->GetHeight() / 2, 820, 110, 110, 110, (int)(65 * scale), (int)(65 * scale), 0);
    }
  }

  DrawGameTitle(game[selectgame]);
  
  if (acinput)
  {
    if (system->GetB() == -1)
    {
      mode = UIMODE_INFOBACK;//UIMODE_SELECT;
      infox = 0;
      infospeed = 0.0;
    } else if (system->GetA() == 1)
    {
      mode = UIMODE_EXEC;
      ExecGame(game[selectgame]);
    }
  }

  return 1;
}

int UIDefault::DrawInfomationBack(void)
{
  int dx, dy;
  int atimer = 10;
  unsigned long color;
  double scl, tmp;
  int i, max;

  DrawBackground();

  dy = system->GetHeight() / 2;

  if (infonum > 0)
  {
    // infonum��0�ɖ߂��B

    // �w�i����ߏグ��B
    if (category < 0)
    {
      color = 0x7F000000 | CATE_COLOR[system->GetGameMainCategory(game[selectgame])];
      MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), color);
    }
    dx = (int)(radius * sin(moveasp));
    scl = (system->GetHeight() - (110 + 60)*scale) / 480.0;

    max = system->GetGameSSTMax(game[selectgame]);
    for (i = -2; i <= 2; ++i)
    {
      if (0 <= infonum + i && infonum + i < max)
      {
        MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
          (int)(centerline + dx + 640 * scl * i + infox), (int)(dy + 25 * scale * infoy / system->GetHeight()),
          0, 480 * (infonum + i), 640, 480,
          scl * (1.0 - 0.1*fabs(640 * scl * i + infox) / (640 * scl))
          );
      }
    }

    //MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
    //  centerline + dx, dy + 25 * scale, 0, 0, 640, 480, scl);

    // �Q�[�����B
    dx = 0;
    dy = (int)(system->GetHeight() - scale * 50);
    // �~�{�^���B
    MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
    MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

    // ���{�^���B
    MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
    MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 110, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

    MikanDraw->DrawBox(dx, dy, system->GetWidth(), system->GetHeight(), 0xFFFFFFFF);
    dx += (int)(20 * scale);
    dy += (int)(5 * scale);
    MikanDraw->Printf(10, dx, dy, system->GetGameInfo(game[selectgame]));

    if (infospeed == 0.0)
    {
      infospeed = 640 * scl / 8;
    } else
    {
      infox += infospeed;
      if (2 < fabs(infospeed)) { infospeed *= 0.9; }
      if (640 * scl < abs(infox))
      {
        infox = 0;
        infonum += 1 - 2 * (infospeed > 0);
        infospeed = 0;
      }
    }

  } else if ( 0 < infoy)
  {
    // �w�i����ߏグ��B
    if (category < 0)
    {
      color = (((unsigned char)(127 * infoy / system->GetHeight())) << 24) | CATE_COLOR[system->GetGameMainCategory(game[selectgame])];
      MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), color);
    }

    DrawGameList(0, (int)infoy);
    if (infoy >= system->GetHeight())
    {
      infoy = system->GetHeight() - 1;
      infospeed = system->GetHeight() / 10;
      //infospeed = 10;
    } else
    {
      infoy -= infospeed;
      if (infospeed > 2) { infospeed *= 0.93; }
      if (infoy < 0) { infoy = 0; }
    }

    // �����B
    dx = (int)(radius * sin(moveasp));
    tmp = DrawBox(dx, dy, game[selectgame], 1.4, (unsigned char)(255 * (1.0 - infoy / system->GetHeight())) );
    //stoptime > atimer ? 1.4 : 1.0 + ((double)stoptime / atimer) * 0.4);
    scl = (system->GetHeight() - (110 + 60)*scale) / 480.0;

    if (system->GetGameSSTMax(game[selectgame]) > 1)
    {
      MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
        (int)(centerline + dx + 640 * (scl * infoy / system->GetHeight())), (int)(dy + 25 * scale * infoy / system->GetHeight()), 0, 480, 640, 480, (tmp + (scl - tmp) * infoy / system->GetHeight()) * 0.9);
    }

    //tmp = tmp + (scl - tmp) * infoy / system->GetHeight();
    tmp = 1.0 + (scl - 1.0) * infoy / system->GetHeight();
    MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
      (int)(centerline + dx), (int)(dy + 25 * scale * infoy / system->GetHeight()), 0, 0, 640, 480, boxsize * tmp * 1.4, boxsize * tmp * 480.0 / 640.0 * 1.4, 0);

    // �Q�[�����B
    dx = 0;
    dy = (int)(system->GetHeight() - scale * 50 + (system->GetHeight() - infoy) / 10);
    // �~�{�^���B
    MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
    MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

    // ���{�^���B
    MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
    MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 110, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

    MikanDraw->DrawBox(dx, dy, system->GetWidth(), system->GetHeight(), 0xFFFFFFFF);
    dx += (int)(20 * scale);
    dy += (int)(5 * scale);
    MikanDraw->Printf(10, dx, dy, system->GetGameInfo(game[selectgame]));

    // �I���摜��0�ɂ���B
    infonum = 0;
    infox = 0;
    if (infoy < 0 ) { infospeed = 0; }
  } else
  {
    DrawGameList();
    // �����B
    dx = (int)(radius * sin(moveasp));
    tmp = DrawBox(dx, dy, game[selectgame], 1.4, (unsigned char)(255 * (1.0 - infoy / system->GetHeight())) );
    //stoptime > atimer ? 1.4 : 1.0 + ((double)stoptime / atimer) * 0.4);
    scl = (system->GetHeight() - (110 + 60)*scale) / 480.0;

    // ���ӏ��H

    tmp = tmp + (scl - tmp) * infoy / system->GetHeight();
    //MikanDraw->DrawTextureScalingC(system->GetGameSSTexture(game[selectgame]),
    //  (int)(centerline + dx), (int)(dy + 25 * scale * infoy / system->GetHeight()), 0, 0, 640, 480, 640 * tmp, 480 * tmp,0);

    dy = system->GetHeight();
    // �~�{�^���B
    MikanDraw->DrawTextureScalingC(0, (int)(60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 200, 420, 100, 100, scale * 0.5);
    MikanDraw->DrawTextureRotateScaleC(tex, (int)(210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 590, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

    // ���{�^���B
    MikanDraw->DrawTextureScalingC(0, (int)(system->GetWidth() - 60 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 100, 420, 100, 100, scale * 0.5);
    MikanDraw->DrawTextureRotateScaleC(tex, (int)(system->GetWidth() - 210 * scale * 0.5), (int)(dy - 60 * scale * 0.5), 720, 110, 100, 240, scale * 0.5, MIKAN_PI / 2.0);

    mode = UIMODE_SELECT;
    stoptime = atimer;
  }

  DrawGameTitle(game[selectgame]);

  return 0;
}


int UIDefault::PrevSelectNumber( int select ){ return select <= 0 ? (nowgames - 1) : (select - 1); }
int UIDefault::NextSelectNumber( int select ){ return (select + 1) % nowgames; }

struct EXECGAME
{
  class TenLANSystem *system;
  int gamenum;
};

int ExecGame(void *arg)
{
  struct EXECGAME *ge = (struct EXECGAME *)arg;

  ge->system->ExecGame(ge->gamenum);
  free( ge );
  return 0;
}

int UIDefault::ExecGame(int gamenum)
{
  struct EXECGAME *data;
  data = (struct EXECGAME *)calloc( 1, sizeof( EXECGAME ) );
  data->system = system;
  data->gamenum = gamenum;

  MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), 0xFF000000);
  MikanSystem->WaitNextFrame( 1 );

  MikanSystem->RunThread(THREAD_EXEC, ::ExecGame, (void *)data);
  return 0;
}

int UIDefault::DrawBackground( void )
{
  int w = 0;

  do
  {
    MikanDraw->DrawTextureScaling( tex, w, 0, 0, 480, 640, 480, scale );
    w += (int)(scale * 640);
  } while( w < system->GetWidth() );

  if (category >= 0 )
  {
    MikanDraw->DrawBox(0, 0, system->GetWidth(), system->GetHeight(), 0x7F000000 | CATE_COLOR[category] );
  }

  return 0;
}

int UIDefault::DrawGameTitle( int gamenum )
{
  int cate, cate_;
  double per;
  int sx = 0, sy = 0;
  // �J�e�S���w�i�B
  if ( gamenum < 0 )
  {
    cate = CATEGORY_MAX - 2; // �~�j�Q�[���B
  } else
  {
    cate = system->GetGameMainCategory( gamenum );
  }
  MikanDraw->SetColor( tex, 0xFF000000 | CATE_COLOR[ cate ] );
  MikanDraw->DrawTextureScaling( tex, 0, 0, 830, 0, 110, 110, scale );
  MikanDraw->SetColor( tex, 0xFFFFFFFF );

  // �J�e�S���A�C�R���B
  sy = scale * 5 * cos( abs(timer % 480 - 240) / 240.0 * MIKAN_PI * 2.0 );
  MikanDraw->DrawTextureRotateScaleC(tex, sx + 80 * scale / 2, sy + 80 * scale / 2, 640, 80 * (cate + 1), 80, 80, scale, MIKAN_PI / 64 * sin(abs(timer % 240) / 240.0 * MIKAN_PI * 2.0 ));
  if ( moveasp != 0.0 )
  {
    // �ړ����̂��ߎ��ɕ\������J�e�S���A�C�R������������\���B
    if (speedline < 0)
    {
      cate_ = NextSelectNumber(selectgame);
    } else
    {
      cate_ = PrevSelectNumber(selectgame);
    }
    if ( game[ cate_ ] < 0)
    {
      cate_ = CATEGORY_MAX - 2; // �~�j�Q�[���B
    } else
    {
      cate_ = system->GetGameMainCategory(game[cate_]);
    }
    if (MIKAN_PI / 6.0 < abs(moveasp))
    {
      per = MIKAN_PI / 6.0;
    }
    per = abs(moveasp) / (MIKAN_PI / 6.0);

    MikanDraw->SetColor(tex, MikanDraw->Color( (unsigned char)(255 * per), 0, 0, 0 ) | CATE_COLOR[cate_]);
    MikanDraw->DrawTextureScaling(tex, 0, 0, 830, 0, 110, 110, scale);
    MikanDraw->SetColor(tex, MikanDraw->Color((unsigned char)(255 * per), 255, 255, 255));
    MikanDraw->DrawTextureRotateScaleC(tex, sx + 80 * scale / 2, sy + 80 * scale / 2, 640, 80 * (cate_ + 1), 80, 80, scale, MIKAN_PI / 64 * sin(abs(timer % 240) / 240.0 * MIKAN_PI * 2.0));
    MikanDraw->SetColor(tex, 0xFFFFFFFF);
  }

  // �^�C�g���B
  MikanDraw->DrawTextureScaling( tex, (int)(110 * scale), 0, 940, 0, 40, 110, scale );
  MikanDraw->DrawTextureScaling( tex, (int)(150 * scale), 0, 980, 0, 40, 110, system->GetWidth() - 150, (int)(scale * 110), 0 );
  if (gamenum < 0)
  {
    MikanDraw->Printf(10, (int)(140 * scale), (int)(20 * scale), gamenum == -2 ? "�߂�" : "�~�j�Q�[���W");
  } else
  {
    MikanDraw->Printf(10, (int)(140 * scale), (int)(20 * scale), system->GetGameTitle(gamenum));
  }
  // �O�ʁB
  MikanDraw->DrawTextureScaling( tex, 0, 0, 720, 0, 110, 110, scale );

  return 0;
}

int UIDefault::DrawGameList( int basex, int basey )
{
  int dx, dy;
  int atimer = 10;
  dy = system->GetHeight() / 2;
  int select;

  select = PrevSelectNumber( PrevSelectNumber( PrevSelectNumber( selectgame ) ) );
  dx = - (int)(radius * sin( MIKAN_PI / 2.0 - moveasp));
  DrawBox( basex + dx, basey + dy, game[ select ] );

  select = NextSelectNumber( select );
  dx = - (int)(radius * sin( MIKAN_PI / 3.0 - moveasp ));
  DrawBox( basex + dx, basey + dy, game[ select ] );

  select = NextSelectNumber( select );
  dx = - (int)(radius * sin( MIKAN_PI / 6.0 - moveasp ));
  DrawBox( basex + dx, basey+ dy, game[ select ] );

  select = selectgame;

  select = NextSelectNumber( NextSelectNumber( NextSelectNumber( select ) ) );
  dx = (int)(radius * sin( MIKAN_PI / 2.0 + moveasp ));
  DrawBox( basex + dx, basey + dy, game[ select ] );

  select = PrevSelectNumber( select );
  dx = (int)(radius * sin( MIKAN_PI / 3.0 + moveasp ));
  DrawBox( basex + dx, basey + dy, game[ select ] );

  select = PrevSelectNumber( select );
  dx = (int)(radius * sin( MIKAN_PI / 6.0 + moveasp ));
  DrawBox( basex + dx, basey + dy, game[ select ] );

  // �����B
  dx = (int)(radius * sin( moveasp ));
  DrawBox( basex + dx, basey + dy, game[ selectgame ], stoptime > atimer ? 1.4 : 1.0 + ((double)stoptime / atimer) * 0.4 );

  return 0;
}

double UIDefault::DrawBox(int dx, int dy, int gamenum, double scl, unsigned char alpha)
{
  double scale = 1.0, tmp;
  unsigned int texnum = tex;
  int dw, dh;
  int i;

  dx += (int)centerline;
  scale -= ( abs( dx - centerline ) / radius );
  dx -= (int)centerline;

  if ( dx < -radius )
  {
    dx = (int)(centerline - dx - 2 * radius);
  } else if ( radius < dx )
  {
    dx = (int)(centerline - dx + 2 * radius);
  }

  tmp = asin( dx / radius );
  scale = abs( ( sin(tmp + atan2( -radius, -boxsize / 2 )) + sin(tmp + atan2( -radius, boxsize / 2 )) ) / 2.0 );

  MikanDraw->DrawBoxC( (int)(centerline + dx), dy, (int)(boxsize * scale * scl), (int)(boxsize * scl), (alpha<<24) | CATE_DCOLOR[ gamenum < 0 ? (CATEGORY_MAX - 1) : system->GetGameMainCategory( gamenum ) ] );//color );
  MikanDraw->SetAlpha(tex, alpha);
  // DVD���^�}�[�N�B
  tmp = (boxsize - (boxsize * 480.0 / 640)) * scl * 0.5;
  if ( system->GetConfig()->IsViewDVD() && system->IsInDVD( gamenum ) )
  {
    MikanDraw->DrawTextureScaling(tex,
      (int)(centerline + dx + (boxsize)* scl * scale * 0.5 - tmp * scale),
      (int)(dy - boxsize * scl * 0.5),
      820, 330, 110, 110, tmp * scale, tmp, 0);
  }
  if (0 <= gamenum)
  {
    if (system->GetConfig()->IsViewNewGame() && system->IsNewGame(gamenum))
    {
      // New Game�}�[�N�B
      MikanDraw->DrawTextureScaling(tex,
        (int)(centerline + dx + (boxsize)* scl * scale * 0.5 - tmp * scale * (1 + system->GetConfig()->IsViewDVD())),
        (int)(dy - boxsize * scl * 0.5),
        820, 440, 110, 110, tmp * scale, tmp, 0);
    }

    // �J�e�S���ꗗ�B
    if (system->GetConfig()->IsViewCategory())
    {
      for (i = 0; i < system->GetGameCaetgoryMax(gamenum); ++i)
      {
        MikanDraw->DrawTextureScaling(tex,
          (int)(centerline + dx - (boxsize)* scl * scale * 0.5 + tmp * scale * i),
          (int)(dy + boxsize * scl * 0.5 - tmp),
          640, 80 + 80 * system->GetGameCategory(gamenum, i), 80, 80, tmp * scale, tmp, 0);
      }
    }
  }
  MikanDraw->SetColor(tex, 0xFFFFFFFF);

  if ( gamenum >= 0 )
  {
	  texnum = system->GetGameSSTexture(gamenum);
  }
  MikanDraw->SetAlpha( texnum, alpha );
  MikanDraw->DrawTextureScalingC( texnum,
    (int)(centerline + dx), dy, 0, 0, 640, 480, (int)(boxsize * scale * scl), (int)(boxsize * scl * 480.0 / 640.0), 0 );

  if (gamenum == -1)
  {
    // �~�j�Q�[���B
    dw = (int)(boxsize * scl * 80 / 640);
    dh = (int)(boxsize * 480 / 640 * scale * scl);
    MikanDraw->DrawTextureRotateScaleC(texnum,
      (int)(centerline + dx), dy, 930, 510, 80, 400, dw + dw % 2, dh + dh % 2, MIKAN_PI / 2.0, 0);
  } else if (gamenum == -2)
  {
    // �߂�B
    dw = (int)(boxsize * scl * 80 / 640);
    dh = (int)(boxsize * 480 / 640 * scale * scl);
    MikanDraw->DrawTextureRotateScaleC(texnum,
      (int)(centerline + dx), dy + dy % 2, 930, 110, 80, 400, dw + dw % 2, dh + dh % 2, MIKAN_PI / 2.0, 0);
  }

  MikanDraw->SetColor( texnum );

  MikanDraw->DrawBoxC( (int)(centerline + dx), dy, (int)(boxsize * scale * scl), (int)(boxsize * scl), MikanDraw->Color( 64 * alpha / 255, (unsigned char)(255 * scale), (unsigned char)(255 * scale), (unsigned char)(255 * scale) ) );//color );

  //MikanDraw->DrawBoxC( centerline + dx, dy, boxsize * scale, boxsize * 0.9, MikanDraw->Color( 255, 255 * scale, 0, 0 ) );//color );
  return boxsize * scale * scl / 640.0;
}

void UIDefault::CleanUp( void )
{
  free( game );
}
