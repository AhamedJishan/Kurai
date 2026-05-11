#include "UpgradeManager.h"

#include "Player.h"
#include "Gun.h"

namespace Dawn
{
	UpgradeManager::UpgradeManager(Scene* scene, Player* player, Gun* gun)
		:Actor(scene)
		,mPlayer(player)
		,mGun(gun)
	{
	}
	
	void UpgradeManager::UpgradeSpread()
	{
		mGun->IncreaseBulletSpread();
	}
	
	void UpgradeManager::UpgradeDamage()
	{
		mGun->IncreaseDamage();
	}
	
	void UpgradeManager::UpgradeDash()
	{
		mPlayer->DecreaseDashCooldown();
	}

	bool UpgradeManager::IsSpreadUpgradeable()
	{
		return mGun->IsSpreadUpgradeable();
	}
	
	bool UpgradeManager::IsDamageUpgradeable()
	{
		return mGun->IsDamageUpgradeable();
	}
	
	bool UpgradeManager::IsDashUpgradeable()
	{
		return mPlayer->IsDashUpgradeable();
	}
}