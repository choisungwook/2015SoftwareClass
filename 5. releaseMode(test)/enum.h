#ifndef __ENUM_H__
#define __ENUM_H__

#define backgroundImagePath		"img\\background.bmp"
#define carReaderUpImagePath	"img\\parkinggateUp2.bmp"
/////////////// 차량 이미지
#define	car0rightImagePath		"img\\car\\car0\\right.bmp"
#define	car0leftImagePath		"img\\car\\car0\\left.bmp"
#define	car0upImagePath			"img\\car\\car0\\up.bmp"
#define	car0downImagePath		"img\\car\\car0\\down.bmp"



#define sortOfcar				1

#include <string>

enum Screen
{
	WIDTH = 1024,
	HEIGHT = 768,

};
namespace carhorizon
{
	enum carsize
	{
		horizionwidth = 100,
		horizionheight = 35
	};
}

namespace carvertical
{
	enum carsize
	{
		horizionwidth = 35,
		horizionheight = 100
	};
}


#endif