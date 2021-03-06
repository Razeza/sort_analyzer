
#ifndef BASICS_CPP
    #define BASICS_CPP

#include "basics.hpp"

Color::Color (int init_red, int init_green, int init_blue, int init_alfa):
        red (init_red),
        green (init_green),
        blue (init_blue),
        alfa (init_alfa)
{ }

bool Color::operator!= (const Color& another) const
{
    return red   != another.red   ||
           green != another.green ||
           blue  != another.blue  ||
           alfa  != another.alfa;
}

Point Point::operator+ (Point another)
{
    return {x + another.x, y + another.y};
}

Point Point::operator- (Point another)
{
    return {x - another.x, y - another.y};
}

Point Point::operator/ (int div)
{
    return {x / div, y / div};
}


Point::Point (double x, double y):
    x (x),
    y (y)
{ }

uint32_t RGBA (uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return r | (g << 8) | (b << 16) | (a << 24);
}

uint32_t RGBA (Color color) {
    return color.red | (color.green << 8) | (color.blue << 16) | (color.alfa << 24);
}

#endif



