#ifndef __ENUM_H__
#define __ENUM_H__

#define DEFAULTPORT					3333
#define DEFAULTCARMIN				0
#define DEFAULTCARMAX				100

#define backgroundImagePath			"img\\background.bmp"
//터널
#define turnelImagePath				"img\\turnel.bmp"
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
///캐릭터
#define character1backimagePath		"img\\character\\char1\\back.bmp"
#define character1rightimagePath	"img\\character\\char1\\right.bmp"
#define character1leftimagePath		"img\\character\\char1\\left.bmp"
#define character1frontimagePath	"img\\character\\char1\\front.bmp"

#define character2backimagePath		"img\\character\\char2\\back.bmp"
#define character2rightimagePath	"img\\character\\char2\\right.bmp"
#define character2leftimagePath		"img\\character\\char2\\left.bmp"
#define character2frontimagePath	"img\\character\\char2\\front.bmp"

#define character3backimagePath		"img\\character\\char3\\back.bmp"
#define character3rightimagePath	"img\\character\\char3\\right.bmp"
#define character3leftimagePath		"img\\character\\char3\\left.bmp"
#define character3frontimagePath	"img\\character\\char3\\front.bmp"

#define character4backimagePath		"img\\character\\char4\\back.bmp"
#define character4rightimagePath	"img\\character\\char4\\right.bmp"
#define character4leftimagePath		"img\\character\\char4\\left.bmp"
#define character4frontimagePath	"img\\character\\char4\\front.bmp"

#define cloudbackgroundPath			"img\\cloudbackground.bmp"
#define cloud0						"img\\cloud\\0.bmp"
#define cloud1						"img\\cloud\\1.bmp"

//상태
#define watchingmoviePath			"img\\watchmovie.bmp"
#define gotiolet					"img\\gotiolet.bmp"
#define gostore						"img\\gostore.bmp"

#define TEXTSPEED				1000
#define sortOfcar				4
#define	numOfseat				6
#define numOfmovie				5
#define numOfevents				3
#define numOfchar				4
#define numOfCloud				5
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
		cashiernearY	= 200,
		cashierY		= 130,
		exitY			= -80,
	};
}

namespace movie
{
	enum time
	{
		minTime = 3000,
		randTime = 10000,
	};
}

namespace collectionXYofChar
{
	enum destX
	{
		onlandcreatX0 = 190,
		onbridgeX0 = 225,
		onlandcreatX1 = 315,
		onbridgeX1 = 355,
		onlandcreatX2 = 440,
		onbridgeX2 = 480,
		onlandcreatX3 = 560,
		onbridgeX3 = 600,
		onlandcreatX4 = 685,
		onbridgeX4 = 720,
		onlandcreatX5 = 745,
		onbridgeX5 = 720,

		
		onshopfrontrX = 355,
		onshopenterX = 355,
		ontoiletfrontrX = 600,
		ontoiletenterX = 600,
		
	};

	enum destY
	{
		onlandcreatY = 380,
	

		onshopfrontrY = 160,
		onshopenterY = 120,
		ontoiletfrontrY = 160,
		ontoiletenterY = 120,
	};
}

namespace collectionXYcloud
{
	enum X
	{
		cloud0X = 30,
		cloud1X = 60,
		cloud2X = 785,
		cloud3X = 885,
	};
	enum Y
	{
		cloud0Y = 620,
		cloud1Y = 600,
		cloud2Y = 130,
		cloud3Y = 10,
	};
}

namespace collectionXYstatus
{
	enum X
	{
		seat0X = 150,
		seat1X = 280,
		seat2X = 405,
		seat3X = 525,
		seat4X = 645,
		seat5X = 765,
	};
	enum Y
	{
		seat0Y = 330,
	};
}
#endif

