#include Library_Projectile

protected func Initialize()
{
	SetObjectBlitMode(GFX_BLIT_Additive);
}

public func OnHitLandscape()
{
		// TODO: Sound("BulletHitGround?");
		CreateImpactEffect(this.damage / 3);
}

public func OnHitObject(object target)
{
		CreateImpactEffect(this.damage / 3);
}

public func OnHitScan(int x_start, int y_start, int x_end, int y_end)
{
	var laser = CreateObject(LaserEffect, x_start - GetX(), y_start - GetY(), NO_OWNER);
	
	laser->Line(x_start, y_start, x_end, y_end)
		 ->SetWidth(1)
		 ->SetLifetime(10)
		 ->Color(RGBa(255, 200, 0))
		 ->Activate();
}


func CorpsePhysicsElasticityConstant(){ return 800;}

