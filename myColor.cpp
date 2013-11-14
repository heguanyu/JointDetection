#include "StdAfx.h"
#include "myColor.h"

myColor::myColor(void)
{
	r=0;g=0;b=0;tag=0;
}



myColor::~myColor(void)
{
	r=0;g=0;b=0;tag=0;
}

void myColor::parseRGB()
{
	if(tag<=0.5)
	{
		r=50+205*2*tag;
		g=r-50;
		b=255*(1-2*tag);
	}
	else 
	{
		r=255-15*(tag*2-1);
		g=r;
		b=240*(tag-0.5)*2;
	}

	r=(int)(tag*255);
	g=r;b=r;


}