#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <string>
#include <cstdio>
#include <iomanip>
#include <list>
#include <string>
#include "windows.h"
#include "button.h"
#include "dsu.h"
#include "bot.h"

using namespace sf;
const long double w = 1280, h = 720, d = 720;
const int k = 21;

class Game {
public:
	Game() : snm(k) {
		window.create(VideoMode(w, h), "Filler", Style::Default);
		rnd.seed(time(0));
		font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");

		for (int i = 0; i < k; i++)
			q[i] = new RectangleShape[k];
		for (int i = 0; i < 6; i++) {
			button[i].setSize(Vector2f(100, 100));
			button[i].setPosition(d + 50, 30 + i * 110);
			button[i].setFillColor(Colors[i]);
			MapColors[Colors[i].toInteger()] = i;
		}
		for (int i = 0; i < 3; i++) {
			LvL[i].setSize(Vector2f(200, 100));
			LvL[i].setPosition(d + 250, 300 + i * 120);
			LvL[i].setFillColor(Color(128, 128, 128));
			LvL[i].setTextSize(30);
		}
		LvL[0].setText(font, "Easy Bot");
		LvL[1].setText(font, "Middle Bot");
		LvL[2].setText(font, "Hard Bot");

		for (int i = 0; i < k; i++) {
			for (int j = 0; j < k; j++) {
				q[i][j].setSize(Vector2f(d / k, d / k));
				q[i][j].setPosition(d / k * i, d / k * j);
				SetColor(i, j, rnd() % 6);
			}
		}
		if (q[k - 1][k - 1].getFillColor() == q[0][0].getFillColor()) {
			SetColor(0, 0, (NumQ(0, 0) + 1) % 6);
		}
		relax();
		button[NumQ(0, 0)].setText(font, "X");
		button[NumQ(0, 0)].noClick();
		button[NumQ(k - 1, k - 1)].setText(font, "X");
		button[NumQ(k - 1, k - 1)].noClick();

		Opponent = new MiddleBot(k);
		YourLvL = 1;
		LvL[YourLvL].setFillColor(Color(64, 64, 64));
	}

	void run() {
		while (window.isOpen()) {
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed) {
					window.close();
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape) || Keyboard::isKeyPressed(Keyboard::Enter)) {
				window.close();
			}
			if (!snm.resume()) {
				Text result;
				result.setFont(font);
				result.setString("You win! ");
				result.setCharacterSize(100);
				result.setFillColor(Color::Green);
				if (snm.getWeight(k - 1, k - 1) > snm.getWeight(0, 0)) {
					result.setString("You lost! ");
					result.setFillColor(Color::Red);
				}
				result.setString(result.getString() + std::to_string(snm.getWeight(0, 0)) + "/ " + std::to_string(snm.getWeight(k - 1, k - 1)));
				window.draw(result);
				window.display();
				window.clear();
			}
			else {
				Text T;
				T.setFont(font);
				T.setCharacterSize(70);
				T.setPosition(d + 200, 100);
				for (int i = 0; i < 6; i++) {
					if (button[i].CheckClick(window)) {
						int N = NumQ(0, 0);
						SetColor(0, 0, i);
						button[N].clearText();
						button[N].toClick();
						button[NumQ(0, 0)].setText(font, "X");
						button[NumQ(0, 0)].noClick();
						relax();

						T.setString("Bot thinks");
						window.draw(T);
						Draw();

						N = NumQ(k - 1, k - 1);
						SetColor(k - 1, k - 1, Opponent->get(q, button));
						button[N].clearText();
						button[N].toClick();
						button[NumQ(k - 1, k - 1)].setText(font, "X");
						button[NumQ(k - 1, k - 1)].noClick();
						relax();
						break;
					}
				}
				for (int i = 0; i < 3; i++) {
					if (LvL[i].CheckClick(window)) {
						LvL[YourLvL].setFillColor(Color(128, 128, 128));
						delete Opponent;
						YourLvL = i;
						LvL[YourLvL].setFillColor(Color(64, 64, 64));
						switch (i) {
						case 0:
							Opponent = new EasyBot(k);
							break;
						case 1:
							Opponent = new MiddleBot(k);
							break;
						case 2:
							Opponent = new HardBot(k);
							break;
						}
						break;
					}
				}
				T.setString("Your move");
				window.draw(T);
				Draw();
			}
		}
	}

private:
	std::mt19937 rnd;
	RenderWindow window;
	Font font;

	Button button[6];
	Button LvL[3];
	RectangleShape* q[k];
	Bot* Opponent;
	int YourLvL;

	std::vector <Color> Colors = { Color::Blue, Color::Red, Color::Green, Color::Yellow, Color::Magenta, Color::Cyan };
	std::map <int, int> MapColors;
	Dsu snm;

	int NumQ(int a, int b) {
		return MapColors[q[a][b].getFillColor().toInteger()];
	}

	void merge(int a1, int b1, int a2, int b2) {
		if (a1 < 0 || a1 >= k || b1 < 0 || b1 >= k)
			return;
		if (a2 < 0 || a2 >= k || b2 < 0 || b2 >= k)
			return;
		if (q[a1][b1].getFillColor() == q[a2][b2].getFillColor())
			snm.merge(a1, b1, a2, b2);
	}

	void SetColor(int i, int j, int a) {
		q[i][j].setFillColor(Colors[a]);
		snm.setColor(i, j, a);
	}

	void relax() {
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < k; j++) {
				q[i][j].setFillColor(Colors[snm.getColor(i, j)]);
				merge(i, j, i, j - 1);
				merge(i, j, i - 1, j);
			}
		}
	}

	void Draw() {
		for (int i = 0; i < 6; i++)
			button[i].windowDraw(window);
		for (int i = 0; i < k; i++)
			for (int j = 0; j < k; j++)
				window.draw(q[i][j]);
		for (int i = 0; i < 3; i++)
			LvL[i].windowDraw(window);
		window.display();
		window.clear();
	}
};