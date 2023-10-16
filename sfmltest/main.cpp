#define _CRT_SECURE_NO_WARNINGS
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
#include "game.h"
using namespace std;
using namespace sf;

using coor = long long;
const long double PI = acos(-1), eps = 1e-6;

struct vec {
    coor x, y;
    vec() {
        x = y = 0;
    }
    vec(coor a, coor b) {
        x = a;
        y = b;
    }
};

struct line {
    vec A, v;
};

vec operator+ (vec a, vec b) {
    return { a.x + b.x, a.y + b.y };
}

vec operator- (vec a, vec b) {
    return { a.x - b.x, a.y - b.y };
}

vec operator* (vec a, coor b) {
    return { a.x * b, a.y * b };
}

coor operator* (vec a, vec b) {
    return a.x * b.x + a.y * b.y;
}

coor operator% (vec a, vec b) {
    return a.x * b.y - a.y * b.x;
}

vec operator+= (vec& a, vec b) {
    return a = a + b;
}

vec operator-= (vec& a, vec b) {
    return a = a - b;
}

istream& operator >> (istream& cin, vec& a) {
    return cin >> a.x >> a.y;
}

ostream& operator << (ostream& cout, vec& a) {
    return cout << a.x << ' ' << a.y;
}

vec per(line a, line b) {
    return a.A + a.v * ((long double)((b.A - a.A) % b.v) / (a.v % b.v));
}

coor len2(vec a) {
    return a * a;
}

long double len(vec a) {
    return sqrtl(len2(a));
}

bool cmp(vec a, vec b) {
    if (a.x == b.x) {
        return a.y < b.y;
    }
    return a.x < b.x;
}

vector <vec> build(vector <vec> a) {
    sort(a.begin(), a.end(), cmp);
    vector <vec> x, y;
    x.push_back(a[0]);
    y.push_back(a[0]);
    for (int i = 1; i < a.size(); i++) {
        if (i == a.size() - 1 || (a[0] - a[i]) % (a[a.size() - 1] - a[i]) < 0) {
            while (x.size() > 1 && (x[x.size() - 2] - x[x.size() - 1]) % (a[i] - x[x.size() - 1]) > 0) {
                x.pop_back();
            }
            x.push_back(a[i]);
        }
        if (i == a.size() - 1 || (a[0] - a[i]) % (a[a.size() - 1] - a[i]) > 0) {
            while (y.size() > 1 && (y[y.size() - 2] - y[y.size() - 1]) % (a[i] - y[y.size() - 1]) < 0) {
                y.pop_back();
            }
            y.push_back(a[i]);
        }
    }
    for (int i = y.size() - 2; i > 0; i--) {
        x.push_back(y[i]);
    }
    return x;
}


Game g;
int main() {
	g.run();
	freopen("output.csv", "w", stdout);
	Image img;
	string s;
	s = "C:/Users/Dan/Downloads/stars/stars/";
	for (int q = 1; q <= 6000; q++) {
		string t = to_string(q);
		reverse(t.begin(), t.end());
		while (t.size() < 4)
			t.push_back('0');
		reverse(t.begin(), t.end());
		t = "IMG" + t + ".png";
		string x = s + t;
		img.loadFromFile(x);
        vector <vec> a;
        for (int i = 0; i < img.getSize().x; i++) {
            for (int j = 0; j < img.getSize().y; j++) {
                if (img.getPixel(i, j) != Color::White) {
                    a.emplace_back(i, j);
                }
            }
        }
        a = build(a);
        vector <vec> b;
        for (int i = 0; i < a.size(); i++) {
            if (len2(a[i] - a[(i - 1 + a.size()) % a.size()]) > 5)
                b.push_back(a[i]);
        }
        cout << t << ',' << b.size() << endl;
	}
	return 0;
}
