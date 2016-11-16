
global func MOD_FastBullets()
{
	// TODO
}

/* Konstanten */
//Feuermodi-Konstanten
static const FM_Name      = 1;  // Name des Feuermodus'

static const FM_AmmoID    = 2;  // ID der benutzten Munition
static const FM_AmmoLoad  = 3;  // Menge der eingeladenen Munition
static const FM_AmmoUsage = 4;  // Munitionsverbrauch pro AmmoRate Sch�sse
static const FM_AmmoRate  = 5;  // s.o.

static const FM_Reload    = 6;  // Nachladezeit der Waffe in Frames.
static const FM_Recharge  = 7;  // Zeitabstand zwischen zwei Sch�ssen in Frames.

static const FM_Auto      = 8;  // Automatische Waffe.
static const FM_Aim       = 9;  // Waffe zielt
static const FM_Condition =10;  // Vorraussetzung f�r den Schussmodus

static const FM_Icon      =11;  // Icon das im Schussmodimen� angezeigt wird

static const FM_Damage    =12;  // Schaden, den der Schussmodus anrichtet

static const FM_Error     =-1;  // F�r auftretende Fehler

// Bot-Konstanten f�r Feuermodi
static const BOT_Range    = 100;  // Reichweite des Modus
static const BOT_DmgType  = 101;  // Schadenstyp, den der Modus haupts�chlich anrichtet

static const BOT_Ballistic= 102;  // Der Modus schie�t ballistische Geschosse

static const BOT_Power    = 103;  // St�rke der Waffe
  static const BOT_Power_1 =  1;  // geringe St�rke (Pistole, Pistolen-Granatwerfer, Pistole-Laser, Pistole-Schleim, EMP)
  static const BOT_Power_2 =  2;  // mittlere St�rke (Pumpgun, Energiegewehr, Flammenwerfer, Napalmgranate, Lasergranate)
  static const BOT_Power_3 =  3;  // hohe St�rke (Granatwerfer: Normal, Cluster & Schleim, Motors�ge, Energiegewehr mit Laser, Biowerfer, Clusterpumpgun, Feuerball)
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

// Z�ndungsrichtungen f�r das Jetpack
static const Jetpack_Left = 1;
static const Jetpack_UpLeft = 2;
static const Jetpack_UpRight = 3;
static const Jetpack_Right = 4;



global func Debug(string type)
{
	return false;
}

