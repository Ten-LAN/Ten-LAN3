#include "Key.h"

Key::Key( void )
{
  // PS2�R���g���[���[�̔z�u�B
  keys[ 0 ] = PAD_A; // ��̃{�^��(��) 4
  keys[ 1 ] = PAD_B; // ��̃{�^��(��) 5
  keys[ 2 ] = PAD_C; // ��̃{�^��(�~) 6
  keys[ 3 ] = PAD_D; // ��̃{�^��(��) 7
  keys[ 4 ] = PAD_G; // L1 10
  keys[ 5 ] = PAD_H; // R1 11

  // ESC�C�p�̃L�[�̏������B
  ekey[0] = ekey[1] = ekey[2] = ekey[3] = ekey[4] = ekey[5] = -1;
  esccannonframe = 10;
}

Key::~Key( void )
{
  SetPad2Key( 0 );
}

int Key::SetMainPad( int padnum )
{
  mainpad = padnum;
  return 0;
}

int Key::SetPadKey( int buttonnum, int keycode )
{
  keys[ buttonnum ] = keycode;
  return 0;
}

int Key::AddESCKey(int button)
{
  int i;

  for (i = 0 ; i < 6 ; ++i)
  {
    if ( ekey[ i ] < 0 )
    {
      ekey[i] = button;
      return 0;
    }
  }

  return -1;
}

int Key::SetEscCannonFrame(int frame)
{
  if (frame <= 0) { return 1; }
  esccannonframe = frame;
  return 0;
}

int Key::SetPad2Key( unsigned int padnum, int flag )
{
  // �S�ẴL�[�𗣂��B
  _MikanInput->ReleaseAllPad2Key();
  // PAD2Key�L�����B
  MikanInput->SetPad2KeyInput( flag );
  //_MikanInput->RecognitionGamepad( 0 );
  if ( flag ){ SetKeyConfig( padnum ); }
  return 0;
}

int Key::SetPad2Key( int flag )
{
  return SetPad2Key( mainpad, flag );
}

int Key::SetKeyConfig( unsigned int padnum )
{
//  if( MikanDraw->DrawBox( 0, 0, GetWidth(), GetHeight(), 0xcf000000 ) )
//  {
    // �f�o�C�X���X�g���ۂ�
    //_MikanDraw->RecoverFromDeviceLost( 1 );
//  }

  // �\���L�[�̐ݒ�
  MikanInput->SetPad2KeyCode( padnum, PAD_UP, K_UP );
  MikanInput->SetPad2KeyCode( padnum, PAD_DOWN, K_DOWN );
  MikanInput->SetPad2KeyCode( padnum, PAD_LEFT, K_LEFT );
  MikanInput->SetPad2KeyCode( padnum, PAD_RIGHT, K_RIGHT );

  // �e�{�^���̐ݒ�
  // ��̃{�^��
  MikanInput->SetPad2KeyCode( padnum, keys[ 0 ], K_A );
  // �E�̃{�^��
  MikanInput->SetPad2KeyCode( padnum, keys[ 1 ], K_Z );
  // ���̃{�^��
  MikanInput->SetPad2KeyCode( padnum, keys[ 2 ], K_X );
  // ���̃{�^��
  MikanInput->SetPad2KeyCode( padnum, keys[ 3 ], K_S );

  // L
  MikanInput->SetPad2KeyCode( padnum, keys[ 4 ], K_D );
  // R
  MikanInput->SetPad2KeyCode( padnum, keys[ 5 ], K_C );

  //TODO:
  /*if( save )
  {
    SaveConfig( 0, 0 );
  }*/

  // TODO:�L�[�R���t�B�O�p�̒萔�B
  //deckey = -1;
  return 0;
}

int Key::InvalidKeyConfig(unsigned int padnum)
{
  MikanInput->SetPad2KeyCode(padnum, PAD_UP);
  MikanInput->SetPad2KeyCode(padnum, PAD_DOWN);
  MikanInput->SetPad2KeyCode(padnum, PAD_LEFT);
  MikanInput->SetPad2KeyCode(padnum, PAD_RIGHT);

  // �e�{�^���̐ݒ�
  // ��̃{�^��
  MikanInput->SetPad2KeyCode(padnum, keys[0]);
  // �E�̃{�^��
  MikanInput->SetPad2KeyCode(padnum, keys[1]);
  // ���̃{�^��
  MikanInput->SetPad2KeyCode(padnum, keys[2]);
  // ���̃{�^��
  MikanInput->SetPad2KeyCode(padnum, keys[3]);

  // L
  MikanInput->SetPad2KeyCode(padnum, keys[4]);
  // R
  MikanInput->SetPad2KeyCode(padnum, keys[5]);
  return 0;
}

int Key::ESCCannon(unsigned int padnum)
{
  int i;

  for (i = 0; i < 6; ++i)
  {
    if (0 <= ekey[i] && MikanInput->GetPadNum(padnum, ekey[i]) < esccannonframe) { return 0; }
    if (ekey[i] < 0 && 0 <= i)
    {
      break;
    }
  }
  // ESC�C���ˁB
  _MikanInput->SendKey(K_ESC, 1);
  return 1;
}

int Key::GetUp( void ){ return MikanInput->GetKeyNum( K_UP ); }

int Key::GetDown( void ){ return MikanInput->GetKeyNum( K_DOWN ); }

int Key::GetLeft( void ){ return MikanInput->GetKeyNum( K_LEFT ); }

int Key::GetRight( void ){ return MikanInput->GetKeyNum( K_RIGHT ); }

int Key::GetY( void ){ return MikanInput->GetKeyNum( K_A ); }

int Key::GetA( void ){ return MikanInput->GetKeyNum( K_Z ); }

int Key::GetB( void ){ return MikanInput->GetKeyNum( K_X ); }

int Key::GetX( void ){ return MikanInput->GetKeyNum( K_S ); }

int Key::GetL( void ){ return MikanInput->GetKeyNum( K_D ); }

int Key::GetR( void ){ return MikanInput->GetKeyNum( K_C ); }
