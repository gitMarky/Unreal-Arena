
global func MOD_FastBullets()
{
	// TODO
}

/* Konstanten */
//Feuermodi-Konstanten
static const FM_Name      = 1;  // Name des Feuermodus'

static const FM_AmmoID    = 2;  // ID der benutzten Munition
static const FM_AmmoLoad  = 3;  // Menge der eingeladenen Munition
static const FM_AmmoUsage = 4;  // Munitionsverbrauch pro AmmoRate Schüsse
static const FM_AmmoRate  = 5;  // s.o.

static const FM_Reload    = 6;  // Nachladezeit der Waffe in Frames.
static const FM_Recharge  = 7;  // Zeitabstand zwischen zwei Schüssen in Frames.

static const FM_Auto      = 8;  // Automatische Waffe.
static const FM_Aim       = 9;  // Waffe zielt
static const FM_Condition =10;  // Vorraussetzung für den Schussmodus

static const FM_Icon      =11;  // Icon das im Schussmodimenü angezeigt wird

static const FM_Damage    =12;  // Schaden, den der Schussmodus anrichtet

static const FM_Error     =-1;  // Für auftretende Fehler

// Bot-Konstanten für Feuermodi
static const BOT_Range    = 100;  // Reichweite des Modus
static const BOT_DmgType  = 101;  // Schadenstyp, den der Modus hauptsächlich anrichtet

static const BOT_Ballistic= 102;  // Der Modus schießt ballistische Geschosse

static const BOT_Power    = 103;  // Stärke der Waffe
  static const BOT_Power_1 =  1;  // geringe Stärke (Pistole, Pistolen-Granatwerfer, Pistole-Laser, Pistole-Schleim, EMP)
  static const BOT_Power_2 =  2;  // mittlere Stärke (Pumpgun, Energiegewehr, Flammenwerfer, Napalmgranate, Lasergranate)
  static const BOT_Power_3 =  3;  // hohe Stärke (Granatwerfer: Normal, Cluster & Schleim, Motorsäge, Energiegewehr mit Laser, Biowerfer, Clusterpumpgun, Feuerball)
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

// Zündungsrichtungen für das Jetpack
static const Jetpack_Left = 1;
static const Jetpack_UpLeft = 2;
static const Jetpack_UpRight = 3;
static const Jetpack_Right = 4;



global func Debug(string type)
{
	return false;
}

