#include "Hud.hpp"
#include "Events.hpp"

void HudElement::GetAlignedPos(int& x, int& y) {
	x = Event.win_w * xalign + xoff * Renderer.GetWindowScale();
	y = Event.win_h * yalign + yoff * Renderer.GetWindowScale();
}
