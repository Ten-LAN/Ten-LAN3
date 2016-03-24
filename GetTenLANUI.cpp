#include "DefaultGameView.h"
#include "UI\DefaultUI.h"
#include "UI\ui2015.h"

// UIの生成作業はここで行うので、UIを追加する場合はここと自分でつくたファイルを改編する。
// UI/などに自分の作ったGameViewを継承したUIのヘッダを読み込む。
// UIは必ずrootより下の階層にヘッダとソースを格納する。
// 　rootにはプロジェクトファイルとTen-LANのコア部分のコードしか置かない。

class GameView *GetTenLANUI( int type )
{
	switch( type )
	{
	case 0:
	default:
		return new UI2015();
	}
	return new UIDefault();
}

class GameView *GetTenLANBackGround( int type )
{
	switch(type)
	{
	case 0:
	default:
		return new BackGround();
	}
	return new BackGround();
}

class GameView *GetTenLANStartUp( int type )
{
	switch(type)
	{
	case 0:
	default:
		return  new StartUp();
	}
	return  new StartUp();
}