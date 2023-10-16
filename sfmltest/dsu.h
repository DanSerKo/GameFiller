#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <string>
#include <list>

class Dsu {
public:
	Dsu(int k) {
		n = k;
		cnt = n * n;
		p.resize(n * n, -1);
		w.resize(n * n, 1);
		color.resize(n * n);
	}
	~Dsu() = default;
	void rebuild() {
		cnt = n * n;
		p.assign(n * n, -1);
		w.assign(n * n, 1);
	}
	int get(int a, int b) {
		return get(hash(a, b));
	}
	void merge(int a1, int b1, int a2, int b2) {
		merge(hash(a1, b1), hash(a2, b2));
	}
	bool resume() {
		return cnt > 2;
	}
	void setColor(int a1, int b1, int a) {
		color[get(hash(a1, b1))] = a;
	}
	int getColor(int a1, int b1) {
		return color[get(hash(a1, b1))];
	}
	int getWeight(int a1, int b1) {
		return w[get(hash(a1, b1))];
	}
private:
	int get(int v) {
		if (p[v] == -1)
			return v;
		return p[v] = get(p[v]);
	}
	void merge(int a, int b) {
		a = get(a);
		b = get(b);
		if (a != b) {
			if (w[a] < w[b])
				std::swap(a, b);
			w[a] += w[b];
			p[b] = a;
			cnt--;
		}
	}

	int hash(int a, int b) {
		return a * n + b;
	}
	int cnt, n;
	std::vector <int> p, w;
	std::vector <int> color;
};
