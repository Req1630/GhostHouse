#ifndef OBJECT_LIST_H
#define OBJECT_LIST_H

// �I�u�W�F�N�g�̎��.
enum class enObjectNo
{
	None,

	Signboard,			// �Ŕ�.
	Key,				// ��.
	// �w�i�I�u�W�F�N�g.
	Ground,		// �n��.
	Ground_I,	
	Ground_L,	
	Flor,
	Flor_I_one,
	Flor_I,
	Flor_L_one,
	Flor_L,
	Wall,		// ��.
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
	// �v���C���[.
	Sayaka,			// �T���J.
	Towa,			// �g��.

	// �S�[�X�g.
	GhostA,			// �S�[�X�gA.
	GhostB,			// �S�[�X�gB.
	BigGhost,		// �r�b�O�S�[�X�g.

	// �M�~�b�N.
	BookShelf,			// �{�I.
	InvisibleFloor,		// �����Ȃ���.
	SlipThroughFloor,	// �����Ȃ����̂��蔲���o�[�W����.
	InvisibleStartFloor,// �����Ȃ����X�^�[�g�p.
	InvisibleEndFloor,	// �����Ȃ����G���h�p.
	KeyDoor,			// ���ŊJ���h�A.
	Pitfall,			// ���Ƃ���.
	CameraUpCollision,	// �����Ȃ����ڋߎ��A�J��������ɂ�����s���{�b�N�X.
	Switch,				// �X�C�b�`.
	SwitchDoor,			// �X�C�b�`�ŊJ���h�A.
	SwitchRed,			// �X�C�b�`.
	SwitchDoorRed,		// �X�C�b�`�ŊJ���h�A.
	UpDownFloor,		// �㉺��.

	NarrowRoad1,		// �ׂ���.
	NarrowRoad2,		// �ׂ���.
	NarrowRoad3,		// �ׂ���.
	NarrowRoad4,		// �ׂ���.

	Max,

};

#endif	// #ifndef OBJECT_LIST_H.