#ifndef CLASS_KEY
#define CLASS_KEY
#include <Mikan.h>

class Key
{
private:
	// �L�[�Ǘ��B
	int keys[ 6 ];
	int mainpad;
	// ESC����p�̃L�[�B
	int ekey[ 6 ];
	// ESC�C�̔��˂̃t���[�����B
	int esccannonframe;
public:
	Key( void );
	virtual ~Key( void );

	// ! ���C���Ŏg�p����Q�[���p�b�h�̐ݒ�B
	virtual int SetMainPad( int padnum = 0 );
	// ! Pad2Key�̃L�[�ݒ�B
	/* !
	\param buttomnum �p�b�h�̃{�^���ԍ�
	\param keycode �L�[�{�[�h�̃L�[�ԍ�
	*/
	virtual int SetPadKey( int buttonnum, int keycode );

	// ! ESC�C�̃L�[�ݒ�B
	virtual int AddESCKey( int button );
	// ! ESC�C�̃t���[���ݒ�B
	virtual int SetEscCannonFrame( int frame );

	// ! Pad2Key�̗L����/������
	/* !
	\param padnum �p�b�h�ԍ�
	\param flag 0=������,1=�L����
	*/
	virtual int SetPad2Key( unsigned int padnum, int flag );
	// ! ���C���Ŏg�p����Q�[���p�b�h�ɑ΂���Pad2Key�̗L����/������
	/* !
	\param flag 0=������,1=�L����
	*/
	virtual int SetPad2Key( int flag );

	// ! Pad2Key��L�����������̐ݒ���s��
	/* !
	\param padnum �p�b�h�ԍ��B
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