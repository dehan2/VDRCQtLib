#include "Color3f.h"


 Color3f::Color3f()
{
    m_r   =   0.0f;
    m_g   =   0.0f;
    m_b   =   0.0f;
}

 Color3f::Color3f(const float& r, const float& g, const float& b )
{
    m_r =   r;
    m_g =   g;
    m_b =   b;
}

 Color3f::Color3f(const Color3f& otherPt )
{
    m_r =   otherPt.m_r;
    m_g =   otherPt.m_g;
    m_b =   otherPt.m_b;
}

 Color3f::Color3f(const float& c, const float& m, const float& y, const float& k)
{
	array<float, 4> CMYK = { c, m, y, k };
	set_CMYK(CMYK);
}

 Color3f& Color3f::operator=(const Color3f& otherPt)
{
    if(this ==  &otherPt){
        return *this;        
    }

    m_r =   otherPt.m_r;
    m_g =   otherPt.m_g;
    m_b =   otherPt.m_b;

    return *this;
}


 Color3f Color3f::operator+(const Color3f& otherPt) const
{

	float r = m_r + otherPt.getR();
	float g = m_g + otherPt.getG();
	float b = m_b + otherPt.getB();

	return Color3f(r, g, b);
}




 Color3f Color3f::operator-(const Color3f& otherPt) const
{
	float r = m_r - otherPt.getR();
	float g = m_g - otherPt.getG();
	float b = m_b - otherPt.getB();

	return Color3f(r, g, b);
}



 Color3f Color3f::operator*(const float& scalar) const
{
	return Color3f(m_r*scalar, m_g*scalar, m_b*scalar);
}




 Color3f Color3f::operator/(const float& scalar) const
{
	return operator*(1 / scalar);
}



 Color3f operator * (const float& scalar, const Color3f& otherColor)
{
	return otherColor.operator*(scalar);
}


 Color3f::~Color3f()
{

}

 void Color3f::set_RGB(const array<float, 3>& RGB)
{
	m_r = RGB.at(0);
	m_g = RGB.at(1);
	m_b = RGB.at(2);
}

 void Color3f::set_CMYK(const array<float, 4>& CMYK)
{
	array<float, 3> RGB = convert_CMYK_to_RGB(CMYK);
	set_RGB(RGB);
}

//Conversion: please refer http://www.rapidtables.com/convert/color/rgb-to-cmyk.htm

array<float, 3> Color3f::convert_CMYK_to_RGB(const array<float, 4>& CMYK) const
{
	array<float, 3> RGB;
	RGB.at(0) = (1 - CMYK.at(0))*(1 - CMYK.at(3));
	RGB.at(1) = (1 - CMYK.at(1))*(1 - CMYK.at(3));
	RGB.at(2) = (1 - CMYK.at(2))*(1 - CMYK.at(3));
	return RGB;
}

array<float, 4> Color3f::convert_RGB_to_CMYK(const array<float, 3>& RGB) const
{
	array<float, 4> CMYK;
	float maxValue = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		if (RGB.at(i) > maxValue)
		{
			maxValue = RGB.at(i);
		}
	}
	CMYK.at(3) = 1 - maxValue;
	CMYK.at(0) = (1 - RGB.at(0) - CMYK.at(3)) / (1 - CMYK.at(3));
	CMYK.at(1) = (1 - RGB.at(1) - CMYK.at(3)) / (1 - CMYK.at(3));
	CMYK.at(2) = (1 - RGB.at(2) - CMYK.at(3)) / (1 - CMYK.at(3));
	return CMYK;
}
