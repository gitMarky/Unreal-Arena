
global func MOD_FastBullets()
{
	// TODO
}

/* Konstanten */ 
//Feuermodi-Konstanten
static const FM_Name      = 1;  // Name des Feuermodus'

static const FM_AmmoID    = 2;  // ID der benutzten Munition
static const FM_AmmoLoad  = 3;  // Menge der eingeladenen Munition
static const FM_AmmoUsage = 4;  // Munitionsverbrauch pro AmmoRate Schuesse
static const FM_AmmoRate  = 5;  // s.o.

static const FM_Reload    = 6;  // Nachladezeit der Waffe in Frames.
static const FM_Recharge  = 7;  // Zeitabstand zwischen zwei Schuessen in Frames.

static const FM_Auto      = 8;  // Automatische Waffe.
static const FM_Aim       = 9;  // Waffe zielt
static const FM_Condition =10;  // Vorraussetzung fuer den Schussmodus

static const FM_Icon      =11;  // Icon das im Schussmodimenue angezeigt wird

static const FM_Damage    =12;  // Schaden, den der Schussmodus anrichtet

static const FM_Hitscan = 13;
static const FM_Ballistic = 13;
static const FM_ProjSpeed = 13;
static const BOT_Priority = 13;
static const BOT_Override = 13;
static const BOT_RangeMin = 13;
static const BOT_RangeMax = 13;
static const BOT_Rating = 13;

static const FM_Error     =-1;  // Fuer auftretende Fehler

// Bot-Konstanten fuer Feuermodi
static const BOT_Range    = 100;  // Reichweite des Modus
static const BOT_DmgType  = 101;  // Schadenstyp, den der Modus hauptsaechlich anrichtet

static const BOT_Ballistic= 102;  // Der Modus schiesst ballistische Geschosse

static const BOT_Power    = 103;  // Staerke der Waffe
  static const BOT_Power_1 =  1;  // geringe Staerke (Pistole, Pistolen-Granatwerfer, Pistole-Laser, Pistole-Schleim, EMP)
  static const BOT_Power_2 =  2;  // mittlere Staerke (Pumpgun, Energiegewehr, Flammenwerfer, Napalmgranate, Lasergranate)
  static const BOT_Power_3 =  3;  // hohe Staerke (Granatwerfer: Normal, Cluster & Schleim, Motorsaege, Energiegewehr mit Laser, Biowerfer, Clusterpumpgun, Feuerball)
  static const BOT_Power_LongLoad = 4;
                                  // Waffen, mit einer langen Nachladezeit (Partikelk., Bazooka, Minigun)
                                  // Diese Waffen werden nur leer geschossen und dann erstmal nicht benutzt
static const BOT_EMP      = 104;  // EMP-Waffe. Wird nur gegen Maschinen eingesetzt

static const WAYP = nil;
static const JTPK = nil;
static const GBRB = nil;
static const SPNP = nil;


// Flags sind nicht kombinierbar
static const Path_MoveTo = 0; // ;)
static const Path_Jump = 1;
static const Path_Backflip = 2;
static const Path_Lift = 3;
//... weitere denkbar

// Zuendungsrichtungen fuer das Jetpack
static const Jetpack_Left = 1;
static const Jetpack_UpLeft = 2;
static const Jetpack_UpRight = 3;
static const Jetpack_Right = 4;



global func Debug(string type)
{
	return false;
}

