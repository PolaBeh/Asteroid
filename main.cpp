#include "Asteroid.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
using namespace std;

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	Game g(35, 35);
	g.play();
}