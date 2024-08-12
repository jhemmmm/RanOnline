#pragma once

#include "../Lib -- Engine/GUInterface/UITextUtil.h"

//	NOTE
//		¾Æ·¡ÀÇ ÀÌ¸§Àº 'color_chart.html'À» µû¸¨´Ï´Ù.
//		/=document/color_chart.html

//	CAUTION
//		¾ËÆÄ°ªÀº ¹Ù²ÙÁö ¸¶½Ê½Ã¿À.

namespace	NS_UITEXTCOLOR
{
	//	±âº»»ö Á¤ÀÇ
	//
	const DWORD DARKLBUE		= D3DCOLOR_ARGB(0xFF,0x87,0xCE,0xFF);
	const DWORD BRIGHTGREEN		= D3DCOLOR_ARGB(0xFF,0x00,0xFF,0x00);
	const DWORD WHITESMOKE		= D3DCOLOR_ARGB(0xFF,0xF5,0xF5,0xF5);
	const DWORD WHITE			= D3DCOLOR_ARGB(0xFF,0xFF,0xFF,0xFF);
	const DWORD DARKGRAY		= D3DCOLOR_ARGB(0xFF,0xA9,0xA9,0xA9);
	const DWORD	LIGHTSKYBLUE	= D3DCOLOR_ARGB(0xFF,0x87,0xCE,0xFA);
	const DWORD	PALEGREEN		= D3DCOLOR_ARGB(0xFF,0x98,0xFB,0x98);	
	const DWORD dwRandomAbility	= D3DCOLOR_RGBA(118,238,198,255);
	const DWORD RED				= D3DCOLOR_ARGB(0xFF,0xFF,0x00,0x00);
	const DWORD GOLD			= D3DCOLOR_ARGB(0xFF,0xFF,0xD7,0x00);
	const DWORD PALEGOLDENROD	= D3DCOLOR_ARGB(0xFF,0xEE,0xE8,0xAA);	
	const DWORD DODGERBLUE		= D3DCOLOR_ARGB(0xFF,0x1E,0x90,0xFF);
	const DWORD DARKTURQUOISE	= D3DCOLOR_ARGB(0xFF,0x00,0xCE,0xD1);
	const DWORD TOMATO			= D3DCOLOR_ARGB(0xFF,0xFF,0x63,0x47);
	const DWORD AQUA			= D3DCOLOR_ARGB(0xFF,0x00,0xFF,0xFF);
	const DWORD ORANGE			= D3DCOLOR_ARGB(0xFF,0xFF,0xA5,0x00);
	const DWORD PALETURQUOISE	= D3DCOLOR_ARGB(0xFF,0xAF,0xEE,0xEE);
	const DWORD SILVER			= D3DCOLOR_ARGB(0xFF,0xC0,0xC0,0xC0);
	const DWORD LIGHTGREY		= D3DCOLOR_ARGB(0xFF,0xD3,0xD3,0xD3);	
	const DWORD GREENYELLOW		= D3DCOLOR_ARGB(0xFF,0xAD,0xFF,0x2F);
	const DWORD ORNAGERED		= D3DCOLOR_ARGB(0xFF,0xFF,0x45,0x00);
	const DWORD HONEYDEW		= D3DCOLOR_ARGB(0xFF,0xF0,0xFF,0xF0);
	const DWORD PERU			= D3DCOLOR_ARGB(0xFF,0xCD,0x85,0x3F);
	const DWORD BURLYWOOD		= D3DCOLOR_ARGB(0xFF,0xDE,0xB8,0x87);
	const DWORD GOLDENROD		= D3DCOLOR_ARGB(0xFF,0xDA,0xA5,0x20);
	const DWORD DARKORANGE		= D3DCOLOR_ARGB(0xFF,0xFF,0x8C,0x00);	
	const DWORD LIGHTCYAN		= D3DCOLOR_ARGB(0xFF,0xE0,0xFF,0xFF);
	const DWORD IVORY			= D3DCOLOR_ARGB(0xFF,0xFF,0xFF,0xF0);
	const DWORD YELLOW			= D3DCOLOR_ARGB(0xFF,0xFF,0xFF,0x00);
	const DWORD CHARTREUSE		= D3DCOLOR_ARGB(0xFF,0x7F,0xFF,0x00);
	const DWORD THISTLE			= D3DCOLOR_ARGB(0xFF,0xD8,0xBF,0xD8);	
	const DWORD LIGHTSALMON		= D3DCOLOR_ARGB(0xFF,0xFF,0xA0,0x7A);
	const DWORD TURQUOISE		= D3DCOLOR_ARGB(0xFF,0x40,0xE0,0xD0);
	const DWORD VIOLET			= D3DCOLOR_ARGB(0xFF,0xEE,0x82,0xEE);
	const DWORD AQUAMARINE		= D3DCOLOR_ARGB(0xFF,0x7F,0xFF,0xD4);
	const DWORD FORESTGREEN		= D3DCOLOR_ARGB(0xFF,0x22,0x8B,0x22);
	const DWORD HOTPINK			= D3DCOLOR_ARGB(0xFF,0xFF,0x69,0xB4);
	const DWORD MAROON			= D3DCOLOR_ARGB(0xFF,0x80,0x00,0x00);
	const DWORD NAVY			= D3DCOLOR_ARGB(0xFF,0x00,0x00,0x80); // Â£Àº°ËÀººûÀ»¶í³²ºû
	const DWORD BLACK			= D3DCOLOR_ARGB(0xFF,0x00,0x00,0x00);

	//wow colors
	const DWORD GREEN_WOW		= D3DCOLOR_ARGB(0xFF,0x00,0xFF,0x00);
	const DWORD BLUE_WOW		= D3DCOLOR_ARGB(0xFF,0x00,0x99,0xFF);
	const DWORD PURPLE_WOW		= D3DCOLOR_ARGB(0xFF,0xFF,0x65,0xFF);
	const DWORD ORANGE_WOW		= D3DCOLOR_ARGB(0xFF,0xFF,0x99,0x00);
	const DWORD GOLD_WOW		= D3DCOLOR_ARGB(0xFF,0xFF,0xCC,0x00);
	
	// clubBattle »ç¿ëÇÒ·Á°í Ãß°¡ ( »ö»ó°ª Á¶±Ý¾¿ Æ²¸² ) 
	const DWORD AQUABLUE		= D3DCOLOR_ARGB(0xFF,0x00,0x26,0xFF);
	const DWORD DARKVIOLET		= D3DCOLOR_ARGB(0xFF,0xB2,0x00,0xFF);
	const DWORD FUCHSIA			= D3DCOLOR_ARGB(0xFF,0xFF,0x00,0xDC);
	const DWORD PURPLE			= D3DCOLOR_ARGB(0xFF,0x7F,0x00,0x6E);
	const DWORD LIGHTYELLOW		= D3DCOLOR_ARGB(0xFF,0xFF,0xE9,0x7F);


	//	»ç¿ëÃ³
	const DWORD ENABLE			= LIGHTGREY;	
	const DWORD DISABLE			= RED;

	const DWORD DEFAULT			= ENABLE;
	const DWORD DIALOGUE_MOUSEOVER = GOLD;
	const DWORD ITEMMONEY		= GOLD;
	const DWORD SELF			= PALEGOLDENROD;	
	const DWORD SYS_ERROR		= RED;
	const DWORD PARTY			= DARKTURQUOISE;
	const DWORD GUILD			= TOMATO;
	const DWORD PRIVATE			= GOLD;	
	const DWORD	PLUSOPTION		= PALEGOLDENROD;
	const DWORD PARTYNAME		= PLUSOPTION;
	const DWORD IDCOLOR			= LIGHTSKYBLUE;
	const DWORD ADMIN_COLOR		= DISABLE;	
	const DWORD NEGATIVE		= RED;
	const DWORD FRIEND_OFF		= MAROON;

	const DWORD MAX_CLUB_BATTLE_COLOR = 8;
	const DWORD CLUB_BATTLE_COLOR[MAX_CLUB_BATTLE_COLOR] = { AQUA, DODGERBLUE, AQUABLUE, DARKVIOLET,
		FUCHSIA, PURPLE, LIGHTYELLOW, GOLD };

};

namespace	NS_UITEXTCONTROL
{
	using namespace NS_UITEXTUTIL;

	DWORD	GetEvaluateColor ( BOOL bValue );
};

/*
778899 lightslategray ¹àÀºÇª¸¥ºûÈ¸»ö 
696969 dimgray Ä¢Ä¢ÇÑÈ¸»ö 
808080 gray È¸»ö 
a9a9a9 darkgray ¾îµÎ¿îÈ¸»ö 
c0c0c0 silver Àº»ö 
d3d3d3 lightgrey ¹àÀºÈ¸»ö 
dcdcdc gainsboro 
ffffff white ¹é»ö 
fff5ee seashell ¹Ù´ÙÁ¶°¡ºñ 
fffafa snow ¼³¹é 
f8f8ff ghostwhite Çã±úºñ¹é»ö 
fffaf0 floralwhite ²ÉÀÇ¹é»ö 
f5f5f5 whitesmoke ¹é»ö¿¬±â 
f0f8ff aliceblue ¿¯ÀºÀíºû¹ÙÅÁ¿¡Çª¸¥ºû 
f0ffff azure ÇÏ´Ã»ö 
fdf5e6 oldlace ³°Àº²ö 
f5fffa mintcream ¹ÚÇÏÅ©¸² 
ffefd5 papayawhip ¿­´ë¾Æ¸Þ¸®Ä«»ê°ú¼ö¸ÅÁú 
ffdab9 peachpuff º¹¼þ¾ÆºûºÒ±â 
faf0e6 linen ¸®³Ù 
eee8aa palegoldenrod ¿¶Àº±¹È­°úÀÇ´Ù³âÃÊ 
ffe4e1 mistyrose Â£ÀºÀå¹Ìºû 
ffe4b5 moccasin »ç½¿°¡Á×ÀÇ±¸µÎ 
ffdead navajowhite ³ª¹ÙÈ£Á·¹é»ö 
d2b48c tan ºµ¿¡Åººû±ò 
f5deb3 wheat ¹Ð 
fafad2 lightgoldenrodyellow ¹àÀº±¹È­°ú´Ù³âÃÊ³ë¶û 
ffffe0 lightyellow ¹àÀº³ë¶û 
fff8dc cornsilk ¿Á¼ö¼ö¸íÁÖ½Ç 
faebd7 antiquewhite ¿¾³¯ÀÇ¹é»ö 
f5f5dc beige ¿¯Àº°¥»ö 
fffacd lemonchiffon ·¹¸óºû°¡º±°í¾ãÀºÁ÷¹° 
fffff0 ivory »ó¾Ñºû 
f0e68c khaki ´©¸¥ºû¿¡¿¯Àº´Ù»öÀÌ¼¯ÀÎºû±ò 
e6e6fa lavender ¿¬ÇÑÀÚÁÖ»ö 
fff0f5 lavenderblush ¿¬ÇÑÀÚÁÖºûºÓ±â 
ffe4c4 bisque ºÐÈ«ºûÀÌµµ´Â°ËÀººûÀ»¶í´©¸¥ºû 
ffebcd blanchedalmond Èñ¾îÁø¿¯ÀºÈ²°¥»ö 
deb887 burlywood Æ°Æ°ÇÑ¸ñÀç 
cd853f peru ³²¹Ì¼­ºÎÀÇ°øÈ­±¹ 
00ced1 darkturquoise ¾îµÎ¿îÇª¸¥ºû³ì»ö 
00bfff deepskyblue Â£ÀºÇÏ´ÃÆÄ¶û 
7fffd4 aquamarine ¿¯ÀºÇª¸¥ºû³ì»ö 
1e90ff dodgerblue¿Á¼ö¼ö»§ÆÄ¶û 
00ffff cyan Çª¸¥Á¤µµ 
f0fff0 honeydew ½Ä¹°ÀÇÀÙ¿¡¼­³ª´Â´Ü¹° 
87cefa lightskyblue ¹àÀºÇÏ´ÃÆÄ¶û 
afeeee paleturquoise ¿¶ÀºÇª¸¥ºû³ì»ö 
e0ffff lightcyan ¹àÀºÇª¸¥Á¤µµ 
add8e6 lightblue ¹àÀºÆÄ¶û 
b0c4de lightsteelblue ¹àÀºÃ¶°­ºûÆÄ¶û 
40e0d0 turquoise Çª¸¥ºû³ì»ö 
48d1cc mediumturquoise Áß°£ÀÇÇª¸¥ºû³ì»ö 
00ffff aqua ¿¯ÀºÇª¸¥ºû³ì»ö 
7b68ee mediumslateblue ÁßÇª¸¥ºûÈ¸»öÆÄ¶û 
191970 midnightblue ±ô±ôÇÑÆÄ¶û 
6495ed cornflowerblue ¿Á¼ö¼ö²ÉÆÄ¶û 
0000cd mediumblue Áß°£ÀÇÆÄ¶û 
6a5acd slateblue Çª¸¥ºûÈ¸»öÆÄ¶û 
4682b4 steelblue Ã¶°­ºûÆÄ¶û 
0000ff blue ÆÄ¶û 
483d8b darkslateblue ¾îµÎ¿îÇª¸¥ºûÈ¸»öÆÄ¶û 
5f9ea0 cadetblue ÀÌÇÏÀÇÆÄ¶û 
87ceeb skyblue ÇÏ´ÃÆÄ¶û 
4169e1 royalblue È²½ÇÀÇÆÄ¶û 
b0e0e6 powderblue °¡·çÆÄ¶û 
000080 navy Â£Àº°ËÀººûÀ»¶í³²ºû 
00008b darkblue ¾îµÎ¿îÆÄ¶û 
8a2be2 blueviolet ÆÄ¶õÁ¦ºñ²É»ö 
8b008b darkmagenta ¾îµÎ¿îÂ£ÀººÐÈ«»ö 
9932cc darkorchid ¾îµÎ¿î¿¬º¸¶ó»ö 
9400d3 darkviolet ¾îµÎ¿îÁ¦ºñ²É»ö 
ff00ff magenta Â£ÀººÐÈ«»ö 
ff00ff fuchsia ºÓÀººû±òÀÌ³ª´ÂÀÚÁÞºû 
c71585 mediumvioletred ÁßÁ¦ºñ²É»¡°­ 
ba55d3 mediumorchid Áß°£ÀÇ¿¬º¸¶ó»ö 
9370db mediumpurple Áß°£ÀÇÀÚÁÞºû 
dc143c crimson Â£°ÔºÓÀº»ö±ò 
ff1493 deeppink Â£Àº¿¬ºÐÈ«»ö 
ffb6c1 lightpink ¹àÀº¿¬ºÐÈ«»ö 
ff69b4 hotpink °­·ÄÇÑ¿¬ºÐÈ«»ö 
ffc0cb pink ¿¬ºÐÈ«»ö 
dda0dd plum Â£Àº º¸¶ó»ö 
800080 purple ÀÚÁÞºû 
ee82ee violet Á¦ºñ²É»ö 
d8bfd8 thistle ¾û°ÏÄû 
da70d6 orchid ¿¬º¸¶ó»ö 
4b0082 indigo ³²»ö 
a52a2a brown °¥»ö 
e9967a darksalmon ¾îµÎ¿îÁÖÈ²»ö 
f08080 lightcoral ¹àÀº»êÈ£ºû 
cd5c5c indianred ÀÎµð¾ð»¡°­ 
ffa07a lightsalmon ¹àÀºÁÖÈ²»ö 
db7093 palevioletred ¿¶ÀºÁ¦ºñ²É»¡°­ 
f4a460 sandybrown ¿¯Àº°¥»ö 
fa8072 salmon ÁÖÈ²»ö 
ff6347 tomato Åä¸¶Åä»ö 
ff4500 ornagered 
ff0000 red »¡°­ 
800000 maroon ºÓÀººûÀ»¶í°¥»ö 
8b0000 darkred ¾îµÎ¿î»¡°­ 
b22222 firebrick ³»È­ 
d2691e chocolate °¥»ö 
8b4513 saddlebrown ¾ÈÀå°¥»ö 
a0522d sienna ºÓÀººûÀ»¶í°¥»ö 
bc8f8f rosybrown Àå¹Ìºû°¥»ö 
ff7f50 coral »êÈ£ºû 
ff8c00 darkorange ¾îµÎ¿îºÓÀººûÀ»¶í´©¸¥»ö 
ffa500 orange ºÓÀººûÀ»¶í´©¸¥»ö 
b8860b darkgoldenrod ¾îµÐ¿î±¹È­°úÀÇ´Ù³âÃÊ 
ffd700 gold ±Ýºû 
ffff00 yellow ³ë¶û 
7fff00 chartreuse ¿¬µÎ»ö 
7cfc00 lawngreen ÀÜµð³ì»ö 
00ff00 lime ¿­´ë»êÀÇ·¹¸óºñ½ÁÇÑ°úÀÏ 
32cd32 limegreen ¶óÀÓ³ì»ö 
00ff7f springgreen º½³ì»ö 
3cb371 mediumseagreen Áß°£ÀÇ¹Ù´Ù³ì»ö 
adff2f greenyellow ³ì»öÀÇ³ë¶û 
8fbc8f darkseagreen ¾îµÎ¿î¹Ù´Ù³ì»ö 
90ee90 lightgreen ¹àÀº³ì»ö 
98fb98 palegreen ¿¶Àº³ì»ö 
9acd32 yello: wgree: n 
2e8b57 seagreen ¹Ù´Ù³ì»ö 
00fa9a mediumspringgreen Áßº½³ì»ö 
20b2aa lightseagreen ¹àÀº¹Ù´Ù³ì»ö 
66cdaa mediumaquamarine Áß¿¯ÀºÃ»·Ï»ö 
228b22 forestgreen ½£³ì»ö 
008b8b darkcyan ¾îµÎ¿îÇª¸¥Á¤µµ 
008080 teal ¾Ï·Ï»öÀ»¶íÃ»»ö 
006400 darkgreen ¾îµÎ¿î³ì»ö 
556b2f darkolivegreen ¾îµÎ¿î¿Ã¸®ºê³ì»ö 
008000 green ³ì»ö 
808000 olive ¹°Çª·¹³ª¹µ°úÀÇ»ó·Ï±³¸ñ 
6b8e23 olivedrab ¿Ã¸®ºê¿¯Àº°¥»ö 
bdb76b darkkhaki ¾îµÎ¿î´©¸¥ºû¿¡¿¯Àº´Ù»ö 
daa520 goldenrod ±¹È­°úÀÇ´Ù³âÃÊ 
*/







