#pragma once

#include "Tetrion.hpp"
#include "Render.hpp"

struct HudElement {
	HudElement(double xa, double ya, int xo, int yo, int w_, int h_):
	 xalign(xa), yalign(ya), xoff(xo), yoff(yo), w(w_), h(h_) { ; }

	double xalign, yalign;
	int xoff, yoff;
	int w, h;

	virtual void Render() = 0;
	void GetAlignedPos(int& x, int& y);
};

template <typename int_type>
struct HudCounter : public HudElement {
	HudCounter(int_type& num, std::string font, FONT_SIZE size, TEXT_ALIGN align,
	    double xa, double ya, int xo, int yo):
	  HudElement(xa, ya, xo, yo, 0, 0), number(num), font_name(font), font_size(size), text_align(align) { ; }
	HudCounter(int_type& num, std::string font, FONT_SIZE size, TEXT_ALIGN align,
        std::string pre, char _leading_char, int leads, double xa, double ya, int xo, int yo):
	  HudElement(xa, ya, xo, yo, 0, 0), number(num), prefix(pre), leading_chars(leads), leading_char(_leading_char),
	  font_name(font), font_size(size), text_align(align) { ; }

	std::string font_name;
	FONT_SIZE font_size;
	TEXT_ALIGN text_align;

	std::string prefix = "";
	int_type& number;

	char leading_char = '0';
	int leading_chars = 0;

	void Render() {
		std::string str = GenerateString();
		int x, y;
		GetAlignedPos(x, y);
		Renderer.RenderText(font_name, str, x, y, font_size, {0xff,0xff,0xff,0xff}, text_align,
		  Renderer.GetWindowScale());
	}

	std::string GenerateString() {
		return prefix + GenerateLeadingZeros() + std::to_string(number);
	}

	std::string GenerateLeadingZeros() {
		int count = leading_chars - std::to_string(number).length();
		if (count < 0) count = 0;
		return std::string(count, leading_char);
	}
};

struct HudString : public HudElement {
	HudString(std::string& str_, std::string font, FONT_SIZE size, TEXT_ALIGN align,
	    double xa, double ya, int xo, int yo):
	  HudElement(xa, ya, xo, yo, 0, 0), str(str_), font_name(font), font_size(size), text_align(align) { ; }
	HudString(std::string& str_, std::string font, FONT_SIZE size, TEXT_ALIGN align,
        std::string pre, char _leading_char, int leads, double xa, double ya, int xo, int yo):
	  HudElement(xa, ya, xo, yo, 0, 0), str(str_), prefix(pre), leading_chars(leads), leading_char(_leading_char),
	  font_name(font), font_size(size), text_align(align) { ; }

	std::string font_name;
	FONT_SIZE font_size;
	TEXT_ALIGN text_align;

	std::string prefix = "";
	std::string& str;

	char leading_char = ' ';
	int leading_chars = 0;

	void Render() {
		std::string str = GenerateString();
		int x, y;
		GetAlignedPos(x, y);
		Renderer.RenderText(font_name, str, x, y, font_size, {0xff,0xff,0xff,0xff}, text_align,
		  Renderer.GetWindowScale());
	}

	std::string GenerateString() {
		return prefix + GenerateLeadingZeros() + str;
	}

	std::string GenerateLeadingZeros() {
		int count = leading_chars - str.length();
		if (count < 0) count = 0;
		return std::string(count, leading_char);
	}
};
