#ifndef OBJECT_LIST_H
#define OBJECT_LIST_H

// オブジェクトの種類.
enum class enObjectNo
{
	None,

	Signboard,			// 看板.
	Key,				// 鍵.
	// 背景オブジェクト.
	Ground,		// 地面.
	Ground_I,	
	Ground_L,	
	Flor,
	Flor_I_one,
	Flor_I,
	Flor_L_one,
	Flor_L,
	Wall,		// 壁.
	Wall_out,
	Pillar,
	Entrance,
	Entrance_close,
	Roof,
	Roof_L,
	Roof2_I,
	Roof2_L,
	OpenDoor,
	Tree,

	Event,
	Goal,
	TextEvent,
	InvisibleWall,
	InvisibleBox,
	// プレイヤー.
	Sayaka,			// サヤカ.
	Towa,			// トワ.

	// ゴースト.
	GhostA,			// ゴーストA.
	GhostB,			// ゴーストB.
	BigGhost,		// ビッグゴースト.

	// ギミック.
	BookShelf,			// 本棚.
	InvisibleFloor,		// 見えない床.
	SlipThroughFloor,	// 見えない床のすり抜けバージョン.
	InvisibleStartFloor,// 見えない床スタート用.
	InvisibleEndFloor,	// 見えない床エンド用.
	KeyDoor,			// 鍵で開くドア.
	Pitfall,			// 落とし穴.
	CameraUpCollision,	// 見えない床接近時、カメラを上にあげる不可視ボックス.
	Switch,				// スイッチ.
	SwitchDoor,			// スイッチで開くドア.
	SwitchRed,			// スイッチ.
	SwitchDoorRed,		// スイッチで開くドア.
	UpDownFloor,		// 上下床.

	NarrowRoad1,		// 細い道.
	NarrowRoad2,		// 細い道.
	NarrowRoad3,		// 細い道.
	NarrowRoad4,		// 細い道.

	Max,

};

#endif	// #ifndef OBJECT_LIST_H.