#ifndef _COLOR3F
#define _COLOR3F

#include <array>

using namespace std;

class Color3f
{
private:
    float m_r;
    float m_g;
    float m_b;

public:
	 Color3f();
	 Color3f(const float& r, const float& g, const float& b );
	 Color3f(const float& c, const float& m, const float& y, const float& k);
	 Color3f(const Color3f& otherColor);

	 ~Color3f();

	 Color3f& operator = (const Color3f& otherColor);

	 Color3f operator + (const Color3f& otherPt) const;
	 Color3f operator - (const Color3f& otherPt) const;

	 Color3f operator * (const float& scalar)   const;
	 Color3f operator / (const float& scalar)   const;
	 friend Color3f operator * (const float& scalar, const Color3f& otherPt);

	 inline void setR(const float& r) {m_r =   r;}
	 inline void setG(const float& g) {m_g =   g;}
	 inline void setB(const float& b) {m_b =   b;}
	 void set_RGB(const array<float, 3>& RGB);
	 void set_CMYK(const array<float, 4>& CMYK);

	 const float& getR() const {return m_r;}
	 const float& getG() const {return m_g;}
	 const float& getB() const {return m_b;}
	 inline array<float, 3> get_RGB() const { return{ m_r, m_g, m_b }; }
	 inline array<float, 4> get_CMYK() const { return convert_RGB_to_CMYK({ m_r, m_g, m_b }); }

private:
	array<float, 3> convert_CMYK_to_RGB(const array<float, 4>& CMYK) const;
	array<float, 4> convert_RGB_to_CMYK(const array<float, 3>& RGB) const;
};

const Color3f RED(1.0, 0.0, 0.0);
const Color3f DARKRED = 0.7*RED;
const Color3f ORANGE(1.0, 0.5, 0.0);
const Color3f YELLOW(1.0, 1.0, 0.0);
const Color3f DARKYELLOW = YELLOW * 0.7;
const Color3f GREEN(0.0, 1.0, 0.0);
const Color3f DARKGREEN = GREEN * 0.7;
const Color3f BLUE(0.0, 0.0, 1.0);
const Color3f INDIGO(0.3, 0.0, 0.5);
const Color3f VIOLET(0.5, 0.0, 1.0);
const Color3f BLACK(0, 0, 0);
const Color3f SKYBLUE(0, 1, 1);
const Color3f DARKSKYBLUE = SKYBLUE * 0.7;
const Color3f PINK(1, 0.6, 0.8);
const Color3f LIME(0.5, 1, 0);
const Color3f GREY(0.5, 0.5, 0.5);
const Color3f GOLD(0.95, 0.65, 0.01);
const Color3f ORANGE2(0.86, 0.44, 0.01);
const Color3f BROWN(0.61, 0.33, 0.01);

#endif