#include <iostream>
#include "Game.h"
#include "Objects.h"

using namespace std;

class Move : public Component {
public:
	int edge;
	float speed;
	bool movingRight = true;
	Move(Object& parent, int _edge, float _speed) : Component(parent), edge(_edge), speed(_speed) {}

	void Update(float deltaTime) override {
		//deltaTime = 1.0F;
		int newX = round(parent.transform->position.x + (movingRight ? speed * deltaTime : -1 * speed * deltaTime));
		cout << "Updating " << newX << endl;
		if (newX > edge) {
			movingRight = false;
			newX = 2 * edge - newX;
		}
		else if (newX < 0) {
			movingRight = true;
			newX = -newX;
		}

		parent.transform->position.x = newX;
	}
};

int main()
{
	Game game(800,500,"Testing");
	Object* obj = new ShapeObject(RECTANGLE, Vector(400, 250), Vector(10, 400), 0.0F, CENTER, Color(255,255,10));
	obj->transform->size = Vector(50, 300);
	obj->components.push_back(new Move(*obj, 800, 1));

	game.AddObject(obj);
	game.backgroundColor = Color(10, 50, 200);

	game.Start();

	//Keeps window open
	std::cin.get();
}