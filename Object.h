#ifndef ASTRO_OBJECT_H
#define ASTRO_OBJECT_H


#include "math.h"
#include "vector2d.h"


#define TIMER_LEN     10  //60ms
#define MAX_OBJ       5
#define G             (6.6743 * 0.000000001) //6.6743 × 10-11 m3 kg-1 s-2
#define sun_earth_d_pixle  400
#define Scalar_d  (150.85*1000000/sun_earth_d_pixle/1.414)
#define Max_Obj_Name_Len   128


int g_init_width  = 0;
int g_init_height  = 0;

int g_current_width  = 0;
int g_current_height  = 0;

int random(int min, int max)
{
	//return the random value between min and max, which includes min and not max
	
	if(max>min)
	{
		return rand()%(max-min)+min;
	}
	else 
	{
		return rand()%(min-max)+max;	
	}	
}

/*
That's what's behind the two oddities of strncpy():

It doesn't put a nul-terminator on the destination if it is completely filled; and
It always completely fills the destination, with nuls if necessary.
For a "safer strcpy()", you are better off using strncat() like so:

if (dest_size > 0)
{
    dest[0] = '\0';
    strncat(dest, source, dest_size - 1);
}
That will always nul-terminate the result, and won't copy more than necessary.

*/

void mystrcpy(char *dest, const char * source, int max_len)
{
	dest[0]='\0';
	strncat(dest, source, max_len - 1);
}

class AstroObject
{

public:
	COLORREF m_color;
	double m_mass;
	int m_size;
	double m_posx;
	double m_posy;
	VECTOR2D <double> m_velocity;
	VECTOR2D <double> m_net_force;
	VECTOR2D <double> m_acc;
	char m_name[Max_Obj_Name_Len];
	
	AstroObject()
	{
		m_mass = 0;
		m_posx = 0;
		m_posy = 0;
		m_size = 0;
		m_name[0]='\0';	
		m_color = RGB(255,255,255);
		m_velocity.set(0,0);
	}
	
	AstroObject(double mass, double pos_x, double pos_y, int size, COLORREF cl, const char * name)
	{
		m_mass = mass;
		m_posx = pos_x;
		m_posy = pos_y;
		m_size = size;
		m_color = cl;
		strncat((char*)m_name, name, Max_Obj_Name_Len);		
	}
	
	void set(double mass, double pos_x, double pos_y, int size, COLORREF cl, const char * name)
	{
		m_mass = mass;
		m_posx = pos_x;
		m_posy = pos_y;
		m_size = size;
		m_color = cl;
		strncat((char*)m_name, name, Max_Obj_Name_Len);		
	}
	
	void update(AstroObject obj[], int ObjNum);	
	/*
	bool operator==(AstroObject &obj)
	{
		if(this == (AstroObject*)&obj) return true;
		else return false;
	}*/	
};

void AstroObject::update(AstroObject obj[], int ObjNum)
{
	double d = 0.0;
	double f = 0;
	VECTOR2D <double> temp_v;
	
	this->m_net_force.set(0,0);
	
	for(int i=0; i<ObjNum; i++)
	{
		if(this == &obj[i]) continue;
		
		d = sqrt((this->m_posx-obj[i].m_posx)*(this->m_posx-obj[i].m_posx) + (this->m_posy-obj[i].m_posy)*(this->m_posy-obj[i].m_posy));
		
		if(d >5000)
		{
			f = G * this->m_mass * obj[i].m_mass / d / d;	
		}
		else
		{
			f = 0;	
		}
		
		temp_v.set(obj[i].m_posx-this->m_posx, obj[i].m_posy-this->m_posy);
		temp_v.normalize();
		temp_v *= f;
		
		
		this->m_net_force += temp_v;
			
	}
	
	this->m_acc = this->m_net_force/this->m_mass;
	
	/*
 	this.Velocity.add(this.Acc.setMag(this.Acc.mag() * Timer_len/1000*Time_expedite_rate));
   
    
    //integrate velocity over time to get placement. delta_placement = velocity * delat_time.
    //Time_expedite_rate makes the time fly faster
    //Velocity value is still valid after below steps.
    
    this.pos_x += (this.Velocity.x * Timer_len/1000*Time_expedite_rate);
    
    this.pos_y += (this.Velocity.y * Timer_len/1000*Time_expedite_rate);
	*/	
	
	this->m_velocity += (this->m_acc*TIMER_LEN/1000*3);
	
 	this->m_posx += (this->m_velocity.getx() * TIMER_LEN/1000*3);
    
    this->m_posy += (this->m_velocity.gety() * TIMER_LEN/1000*3); 	
}

#endif
