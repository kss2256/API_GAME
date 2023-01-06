#pragma once






#define KEY_PRESSE(KEY) ya::Input::GetKeyState(KEY) == ya::eKeyState::PRESSED
#define KEY_DOWN(KEY) ya::Input::GetKeyState(KEY) == ya::eKeyState::DOWN
#define KEY_UP(KEY) ya::Input::GetKeyState(KEY) == ya::eKeyState::UP

#define MOUSE_POS ya::Input::GetMousePos()

#define DT ya::Time::DeltaTime()

enum class eGroundType
{
	Ground,
	Ground1,
	Ground2,
	Object,
	Object1,
	Object2,
	Col,
	Rol,
	Red_Col,
	Red_Rol,
	Baldur,
	Husk,
	Hornet,
	False_Knight,
	Player,

	END,
};


enum class eSceneType
{
	Logo,
	Title,
	Play,
	End,
	Ground_Tool,
	Animation_Tool,
	Max,
};


enum class eModeType
{

	Play,
	Ground,
	Animation,
	Exit,
	Option,

	None,

};

enum class ePenColor
{
	Red,
	Green,
	Blue,
	End,
};

enum class eBrushColor
{
	Transparent, //투명
	Blakc,
	Gray, //167 0 67
	White,
	End,
};

enum class eComponentType
{
	Rigidbody,
	Collider,
	Aimator,
	Sound,
	End,
};

#define _COLLIDER_LAYER 16
enum class eColliderLayer
{
	Default,

	GameObject,
	BackGround,
	Tile,
	Player,
	Player_Projecttile,
	Monster,
	Monster_Projecttile,
	Monster_Detect,
	Ground,
	GroundPriview,

	//UI
	BackPack,
	UI = _COLLIDER_LAYER - 1,
	End = _COLLIDER_LAYER,
};

enum class eCameraEffect
{
	FadeIn,
	FadeOut,
	None,
};

enum class eUIType
{
	HP,
	LIFE,
	MP,
	SHOP,
	INVENTORY,
	OPTION,
	START,
	GROUNDBUTTON,
	SAVEBUTTON,
	LOADBUTTON,
	GROUND_OBJECT,
	ANIMATIONPANEL,

	

	END,
};

enum class SITUATION
{
	None,	
	Walk,
	both,			//양쪽 누르고 있는 상태
	Stading,	
	Dash,
	Attack,
	Up_Attack,
	Down_Attack,
	Look_Up,
	Up_Presse,
	Up_End,
	Look_Down,
	Down_Presse,
	Down_End,
	Jump,
	Weightless,
	Floating,
	Gravity,
	Ground,
	Hit,
	Modal,
	In,
	Save,
	End,


};

enum class PLAYER_BGM
{
	Back,
	Hornet,
	False,
	None,
};

enum class DIRECTION
{
	Left,
	Right,
	Up,
	Down,
};

enum class Attack
{
	None,
	Up,
	Down,
};

enum class ACTION
{
	None,
	Standing,
	Turn,
	Walk,
	Detect,
	Target,
	Damage,
	Attack,
	Attack_Cool,
	Death_Air,
	Mortis,
	Death,
	End,
};


union ColliderID
{
	struct
	{
		UINT32 left;
		UINT32 right;
	};

	UINT64 ID;
};
typedef ColliderID TileID;



#define TILE_SIZE 5
#define TILE_SCALE 2

#define TILE_LINE_X 8
#define TILE_LINE_Y 3


