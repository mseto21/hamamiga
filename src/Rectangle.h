
struct Rectangle {
	int x;
	int y;
	int w;
	int h;
};

bool Rectangle_Intersects(const Rectangle l, const Rectangle r) {
	return (l.x > r.x && l.x + l.w < r.x + r.w) && (l.y > r.y && l.y + l.h < r.y + r.h);
}