#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <string>
#include <list>
#include "button.h"
#include "dsu.h"

class Bot {
public:
	Bot(int a) : snm(a) {
		k = a;
		for (int i = 0; i < 6; i++) {
			MapColors[Colors[i].toInteger()] = i;
		}
	}
	virtual int get(RectangleShape* q[], Button B[]) {
		return -1;
	}
protected:
	int k;
	std::vector <Color> Colors = { Color::Blue, Color::Red, Color::Green, Color::Yellow, Color::Magenta, Color::Cyan };
	std::map <int, int> MapColors;
	Dsu snm;
	void merge(int a1, int b1, int a2, int b2) {
		if (a1 < 0 || a1 >= k || b1 < 0 || b1 >= k)
			return;
		if (a2 < 0 || a2 >= k || b2 < 0 || b2 >= k)
			return;
		if (snm.getColor(a1, b1) == snm.getColor(a2, b2))
			snm.merge(a1, b1, a2, b2);
	}

	void relax() {
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < k; j++) {
				merge(i, j, i, j - 1);
				merge(i, j, i - 1, j);
			}
		}
	}
	std::mt19937 rnd;
private:
};

class EasyBot : public Bot {
public:
	EasyBot(int a) : Bot(a) {
		rnd.seed(time(0));
	}
	int get(RectangleShape* q[], Button B[]) override {
		int r = rnd() % 6;
		while (!B[r].canClick())
			r = (r + 1) % 6;
		return r;
	}
private:
};

class MiddleBot : public Bot {
public:
	MiddleBot(int k) : Bot(k) {
		good.assign(8, -1e9);
	}
	int get(RectangleShape* q1[], Button B1[]) override {
		snm.rebuild();
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < k; j++) {
				snm.setColor(i, j, MapColors[q1[i][j].getFillColor().toInteger()]);
			}
		}
		relax();
		rec(0);
		good.assign(8, -1e9);
		std::pair <std::pair <int, int>, int> mx = { {-1e9, -1e9}, -1 };
		for (auto& r : mn) {
			mx = std::max(mx, { r.second, r.first[0] });
		}
		mn.clear();
		if (mx.second == -1)
			mx.second = rnd() % 6;
		while (!B1[mx.second].canClick())
			mx.second = (mx.second + 1) % 6;
		return mx.second;
	}
private:
	std::map <std::vector <int>, std::pair <int, int> > mn;
	std::vector <int> a, good;
	void rec(int gl, int max = -1) {
		if (gl == 1)
			max = std::max(max, snm.getWeight(k - 1, k - 1) - snm.getWeight(0, 0));
		if (gl == 6) {
			std::vector <int> b;
			for (int i = 0; i < gl; i += 2) {
				b.push_back(a[i]);
			}
			if (mn.find(b) == mn.end())
				mn[b] = { snm.getWeight(k - 1, k - 1), max };
			mn[b] = std::min(mn[b], { snm.getWeight(k - 1, k - 1) - snm.getWeight(0, 0), max });
			return;
		}
		int dl = 8;
		if (snm.getWeight(k - 1, k - 1) > k * k / 3)
			dl = 0;

		good[gl] = std::max(good[gl], snm.getWeight(k - 1, k - 1));
		if (good[gl] - snm.getWeight(k - 1, k - 1) > dl || snm.getWeight(0, 0) - snm.getWeight(k - 1, k - 1) > 12)
			return;
		

		int I = k - 1, J = 0;
		if (gl % 2) {
			std::swap(I, J);
		}
		auto snmp = snm;

		for (int i = 0; i < 6; i++) {
			if (i != snm.getColor(I, I) && i != snm.getColor(J, J)) {
				snm.setColor(I, I, i);
				relax();
				a.push_back(i);
				rec(gl + 1, max);
				a.pop_back();
				snm = snmp;
			}
		}
	}
};

class HardBot : public Bot {
public:
	HardBot(int a) : Bot(a) {
	}
	int get(RectangleShape* q1[], Button B1[]) override {
		snm.rebuild();
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < k; j++) {
				snm.setColor(i, j, MapColors[q1[i][j].getFillColor().toInteger()]);
			}
		}
		relax();
		rec(0);
		return good;
	}
private:
	int good = -1;
	std::pair <int, int> rec(int gl, std::pair <int, int> alph = { -1e9, -1e9 }, std::pair <int, int> beta = { 1e9, 1e9 }, int max = -1) {
		if (gl == 1)
			max = std::max(max, snm.getWeight(k - 1, k - 1) - snm.getWeight(0, 0));
		if (gl == 8) {
			return { snm.getWeight(k - 1, k - 1) - snm.getWeight(0, 0), max };
		}
		int I = k - 1, J = 0;
		if (gl % 2) {
			std::swap(I, J);
		}
		auto snmp = snm;
		for (int i = 0; i < 6; i++) {
			if (i != snm.getColor(I, I) && i != snm.getColor(J, J)) {
				snm.setColor(I, I, i);
				relax();
				if (gl % 2)
					beta = std::min(beta, rec(gl + 1, alph, beta, max));
				else {
					auto r = rec(gl + 1, alph, beta, max);
					if (alph < r) {
						alph = r;
						if (gl == 0)
							good = i;
					}
				}
				if (alph >= beta) {
					if (gl % 2)
						return beta;
					return alph;
				}
				snm = snmp;
			}
		}
		if (gl % 2)
			return beta;
		return alph;
	}
};