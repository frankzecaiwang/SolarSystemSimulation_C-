#ifndef  VECTOR_H
#define VECTOR_H

#include "math.h"

//Why can’t I separate the definition of my templates class from its declaration and put it inside a .cpp file?
//https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl

 //class template:
template <class T>
class VECTOR2D{

private:
    T m_x;
    T m_y;
    T m_r;
    float m_ang_r;
    
    void translatexytoradian()
    {
    	
		if(m_x == 0 && m_y == 0)
		{
			m_r = 0;
 			m_ang_r = 0;
  			return;
    	}
    	
   		m_r = sqrt(m_x*m_x + m_y*m_y);
		m_ang_r = asin(m_y/m_r);
		
		if(m_x<0 && m_y>0) m_ang_r =  M_PI - m_ang_r;
		if(m_x<0 && m_y<0) m_ang_r = -M_PI - m_ang_r;
			
	}

    void translateradiantoxy()
    {
    	
		if(m_r == 0)
		{
			m_x = 0;
 			m_y = 0;
  			return;
    	}
    	
		m_x = m_r*cos(m_ang_r);
		m_y = m_r*sin(m_ang_r);			
	}
 
public:

	VECTOR2D(T x, T y)
	{
		m_x = x;
		m_y = y;	
		translatexytoradian();		
	};
	
	VECTOR2D()
	{
		m_r = 0;
		m_ang_r = 0;
		translateradiantoxy();		
	};
	
	VECTOR2D(const VECTOR2D& v):m_x(v.m_x),m_y(v.m_y)
	{		
		translatexytoradian();		
	};
	
	VECTOR2D(T r, float ang_r)
	{
		m_r = r;
		m_ang_r = ang_r;
		
		translateradiantoxy();		
	};
	
	VECTOR2D& operator+=(const VECTOR2D& v)
	{
		this->m_x += v.m_x;
		this->m_y += v.m_y; 
	
		translatexytoradian();
	
		return *this;
	
	}
	
	VECTOR2D& operator-=(const VECTOR2D& v)
	{
		this->m_x -= v.m_x;
		this->m_y -= v.m_y; 
	
		translatexytoradian();
	
		return *this;		
	}
	
	VECTOR2D& operator= (const VECTOR2D& v)
	{
		this->m_x = v.m_x;
		this->m_y = v.m_y; 
	
		translatexytoradian();
	
		return *this;
	
	}
	
	VECTOR2D& operator*= (const T &u)
	{
		this->m_x *= u;
		this->m_y *= u; 
	
		translatexytoradian();
	
		return *this;
	
	}
	
	VECTOR2D& operator/= (const T &u)
	{
		this->m_x /= u;
		this->m_y /= u; 
	
		translatexytoradian();
	
		return *this;
	}
	
	VECTOR2D operator/ (const T &u)
	{
		VECTOR2D<T>vReslt(0.0,0.0);
		
		if(u==0) return vReslt;
		
		vReslt.set(this->m_x / u, this->m_y / u);
	
		return vReslt;
	}
	
	VECTOR2D operator* (const T &u)
	{
		VECTOR2D<T>vReslt(0.0,0.0);
		
		vReslt.set(this->m_x * u, this->m_y * u);
	
		return vReslt;
	}
	
	VECTOR2D operator+ (const VECTOR2D& v)
	{
		VECTOR2D<T>vReslt(0.0,0.0);
		
		vReslt += &this;
		vReslt += v;
	
		return vReslt;
	}
	
	VECTOR2D operator- (const VECTOR2D& v)
	{
		VECTOR2D<T>vReslt(0.0,0.0);
		
		vReslt += &this;
		vReslt -= v;
	
		return vReslt;
	}
	
	void rotate(const float a)
	{	
		this->m_ang_r += a;
		translateradiantoxy();
	}
	
	void normalize()
	{	
		this->m_r = 1;
		translateradiantoxy();
	}
	
	T getx(){return m_x;}
	T gety(){return m_y;}
	
	T getRadius()
	{
		return this->m_r;	
	}
	
	float getAng()
	{
		return this->m_ang_r;
	}
	
	void setx(T x){this->m_x = x; translatexytoradian();}
	void sety(T y){this->m_y = y; translatexytoradian();}
	void setR(T r){this->m_r = r; translateradiantoxy();}
	void setA(float a){this->m_ang_r = a; translateradiantoxy();}
	void set(T x, T y){this->m_x = x; this->m_y = y; translatexytoradian();}
	
};

#endif
