#pragma	once

struct	SITEMCUSTOM;
struct  SNATIVEID;

namespace	NS_ITEMINFO
{
	void	RESET ();

	/*npc shop, Juver, 2017/07/26 */
	/*item link, Juver, 2017/07/31 */
	/*product item, Juver, 2017/10/15 */
	void	LOAD ( const SITEMCUSTOM &sItemCustom_base, const BOOL bShopOpen, const BOOL bInMarket, const BOOL bInPrivateMarket, const BOOL bIsWEAR_ITEM, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID,
			const BOOL bNPCShop, const WORD wNPCShopType = 0, const BOOL bItemLink = FALSE, const BOOL bInInventory = FALSE, BOOL bInCompoundResult = FALSE, BOOL bCompoundGenerateRandomOption = FALSE  ); 

	void	LOAD_SIMPLE ( const SITEMCUSTOM &sItemCustom );
};

struct	GLSKILL;
struct	SCHARSKILL;
struct	SNATIVEID;

namespace	NS_SKILLINFO
{
	void	RESET ();
	void	LOAD ( const SNATIVEID& sNativeID, const BOOL bNextLevel );
};