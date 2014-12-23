#strict 2

local movement,effect,max_con,max_time,r1,g1,b1,a1,r2,g2,b2,a2;

/*
movement

0 = keine Bewegung
1 = Standard-Clonk Rauch
2 = Zufallsbewegung auf der Stelle

*/

/*
effect

0 = keine Farbübergänge
1 = Farbübergang von rgba1 nach rgba2
2 = Zufallsfarbe zwischen rgba1 und rgba2
3 = wie 2,nur wechselnd

*/

public func Activate( int _x, int _y, int con, int move_style, int time, int dwColor1, int dwColor2, int effect_style)
{
	if(ObjectCount(MH6I)>=2) return RemoveObject();

	SetAction("Smoke");
	SetCon(Fraction(75,100,con));
 	max_con=con;

  	max_time=32;
  	if(time) max_time=time;

  	if(FindObject(MH6I)) max_time=max_time/2;

  	if(max_time<1) max_time=1;

  	movement=move_style;
  	effect=effect_style;

  	SetPosition(_x,_y);

  	SetDir(Random(4));
  	SetPhase(Random(4));

  	r1=GetRGBaValue(dwColor1,1);
  	g1=GetRGBaValue(dwColor1,2);
  	b1=GetRGBaValue(dwColor1,3);
  	a1=GetRGBaValue(dwColor1,0);

  	r2=GetRGBaValue(dwColor2,1);
  	g2=GetRGBaValue(dwColor2,2);
  	b2=GetRGBaValue(dwColor2,3);
  	a2=GetRGBaValue(dwColor2,0);

	if(!a1 && !a2)
	{
		a1=0;
		a2=160;
	}

  	if(!effect) SetClrModulation(RGBa(255,255,255,132));

  	Move2Move();

}

public func ActivateFlak( int _x, int _y, int con, int move_style)
{
	if(FindObject(MH6I)) return(RemoveObject()); 
	SetAction("Flak");
	SetCon(con/2);

	movement=move_style;
	SetPosition(_x,_y);
	SetDir(Random(4));
	SetPhase(Random(4));
	SetClrModulation(RGBa(255,255,0,60));
}

public func Move2Move()
{
	if(movement == 1)
	{
   		SetYDir(-20);
   		SetXDir(-10+Random(21));
  	}
	if(movement == 2)
  	{
   		SetYDir(-10+Random(21));
   		SetXDir(-10+Random(21));
  	}

  	var _x,_y;

  	_x=GetX();
 	_y=GetY();

  	SetCon(Fraction(75,100,max_con)+Fraction(GetActTime(),max_time,Fraction(45,100,max_con) ));

  	SetPosition(_x,_y);

  	if(effect == 1)
   	{
     		SetClrModulation(RGBa(
					r1 + Fraction(GetActTime(),max_time,r2-r1),
					g1 + Fraction(GetActTime(),max_time,g2-g1),
					b1 + Fraction(GetActTime(),max_time,b2-b1),
					a1 + Fraction(GetActTime(),max_time,a2-a1) ));
	}

	
  	if(effect == 3)
   	{
		var i = Random(100);

     		SetClrModulation(RGBa(
					Min(r1,r2) + Fraction( i, 100, Abs(r2-r1)),
					Min(g1,g2) + Fraction( i, 100, Abs(g2-g1)),
					Min(b1,b2) + Fraction( i, 100, Abs(b2-b1)),
					Min(a1,a2) + Fraction( i, 100, Abs(a2-a1)) ));
	}

	if(GetActTime()>max_time) Remove();

}

public func Remove(){ return RemoveObject();}
