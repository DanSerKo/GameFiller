#pragma once
#include <SFML/Graphics.hpp>
#include "windows.h"

using namespace sf;

class Button {
public:
	Button() {
		_text.setFillColor(Color::Black);
		_CanClick = 1;
	}
	void setSize(Vector2f a) {
		_rec.setSize(a);
		_text.setCharacterSize(a.y / 1.2);
	}
	void setPosition(long double x, long double y) {
		_rec.setPosition(x, y);
		_text.setPosition(x + _text.getCharacterSize() / 4, y);
	}
	void setFillColor(Color a) {
		_rec.setFillColor(a);
	}
	
	void setText(const Font& f, const std::string& s) {
		_text.setFont(f);
		_text.setString(s);
	}
	void clearText() {
		_text.setString("");
	}
	void setTextColor(const Color& a) {
		_text.setFillColor(a);
	}
	void setTextSize(int x) {
		_text.setCharacterSize(x);
		_text.setPosition(_text.getPosition().x, _rec.getPosition().y + x);
	}

	void noClick() {
		_CanClick = 0;
	}
	void toClick() {
		_CanClick = 1;
	}
	bool canClick() {
		return _CanClick;
	}
	bool CheckClick(RenderWindow& window) {
		if (!_CanClick)
			return 0;
		double x = Mouse::getPosition(window).x;
		double y = Mouse::getPosition(window).y;

		double x1 = _rec.getPosition().x;
		double y1 = _rec.getPosition().y;
		double x2 = _rec.getSize().x + x1;
		double y2 = _rec.getSize().y + y1;

		return Mouse::isButtonPressed(Mouse::Left) && x1 <= x && x <= x2 && y1 <= y && y <= y2;
	}

	void windowDraw(RenderWindow& window) {
		window.draw(_rec);
		window.draw(_text);
	}
private:
	bool _CanClick;
	Text _text;
	RectangleShape _rec;
};
