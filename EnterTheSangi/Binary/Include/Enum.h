#pragma once

// FOR CLIENT
namespace OPTION
{
	enum WINMODE { MODE_FULL, MODE_WIN };
}

//FOR ALL
namespace SCENE
{
	enum ID { LOADING, TITLE, CUSTOMIZE, STAGE, END };
}

namespace OBJECT
{
	enum TYPE { MAP, PLAYER, WEAPON, BULLET, CHEST, CAMERA, UI, END };
}

namespace DIR
{
	enum DIR { DIR_LT, DIR_L, DIR_LB, DIR_CT, DIR_C, DIR_CB, DIR_RT, DIR_R, DIR_RB };
}

namespace STATE
{
	enum TYPE { IDLE, RUN, DODGE, DEAD, HAPPY };
	enum DIR { FRONT, BACK, SIDE, BACKSIDE, END };
}