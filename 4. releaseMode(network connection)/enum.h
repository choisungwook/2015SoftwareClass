#ifndef __ENUM_H__
#define __ENUM_H__

#define backgroundImagePath			"img\\background.bmp"
//bottom Reader
#define	carReaderDownImagePath		"img\\parkinggateDown2.bmp"
#define carReaderUpImagePath		"img\\parkinggateUp2.bmp"
//top Reader
#define	TopcarReaderDownImagePath	"img\\cashierUp.bmp"
#define TopcarReaderUpImagePath		"img\\cashierDown.bmp"
//다리
#define BridgeImagePath				"img\\bridge.bmp"
/////////////// 차량 이미지
#define	car0rightImagePath			"img\\car\\car0\\right.bmp"
#define	car0leftImagePath			"img\\car\\car0\\left.bmp"
#define	car0upImagePath				"img\\car\\car0\\up.bmp"
#define	car0downImagePath			"img\\car\\car0\\down.bmp"
//////////////////////////
#define	car1rightImagePath			"img\\car\\car1\\right.bmp"
#define	car1leftImagePath			"img\\car\\car1\\left.bmp"
#define	car1upImagePath				"img\\car\\car1\\up.bmp"
#define	car1downImagePath			"img\\car\\car1\\down.bmp"
//////////////////////////
#define	car2rightImagePath			"img\\car\\car2\\right.bmp"
#define	car2leftImagePath			"img\\car\\car2\\left.bmp"
#define	car2upImagePath				"img\\car\\car2\\up.bmp"
#define	car2downImagePath			"img\\car\\car2\\down.bmp"
//////////////////////////
#define	car3rightImagePath			"img\\car\\car3\\right.bmp"
#define	car3leftImagePath			"img\\car\\car3\\left.bmp"
#define	car3upImagePath				"img\\car\\car3\\up.bmp"
#define	car3downImagePath			"img\\car\\car3\\down.bmp"


#define sortOfcar				4
#define	numOfseat				6
#define numOfmovie				5

#include <string>

enum Screen
{
	WIDTH = 1040,
	HEIGHT = 800,
};
namespace carhorizon
{
	enum carsize
	{
		width = 100,
		height = 35
	};
}

namespace carvertical
{
	enum carsize
	{
		width = 35,
		height = 100
	};
}

namespace collectionXY
{
	enum desX
	{
		createdX		= 1100,
		ReadernearX		= 150,
		ReaderfrontX	= 55,
		cornerX			= 55,
		seat0X			= 135,
		seat1X			= 265,
		seat2X			= 385,
		seat3X			= 515,
		seat4X			= 635,
		seat5X			= 750,
		cornerbottomX	= 930,
		exitX			= 930,
	};
	enum desY
	{
		createdY		= 692,
		ReaderfrontY	= 692,
		cornerY			= 270,
		seatY			= 370,
		seatBottomY		= 500,
		seatTop			= 100,
		nextY			= 570,
		cashiernearY	= 150,
		cashierY		= 100,
		exitY			= -80,
	};
}

#endif