#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Asteroid.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////
//  Asteroid implementations
///////////////////////////////////////////////////////////////////////////

Asteroid::Asteroid(Game* gp) //initializes a new, normal asteroid
{
	m_g = gp;
	m_valid = true;
	m_free = true;
	int r = rand() % 4;
	switch (r)
	{
	case 0:
		m_col = gp->cols() - 1;
		m_row = rand() % gp->rows();
		m_dir = 'A';
		break;
	case 1:
		m_col = rand() % gp->cols();
		m_row = 0;
		m_dir = 'S';
		break;
	case 2:
		m_col = 0;
		m_row = rand() % gp->rows();
		m_dir = 'D';
		break;
	case 3:
		m_col = rand() % gp->cols();
		m_row = gp->rows() - 1;
		m_dir = 'W';
		break;
	default:
		cout << "FATAL ERROR" << endl;
		exit(1);
		break;
	}
}

Asteroid::Asteroid(Game* gp, char b) //gives the option of inputting R, which will spawn randomly in the map
{
	if (b == 'R')
	{
		m_g = gp;
		m_valid = true;
		m_free = true;
		int r = rand() % 4;
		m_col = rand() % gp->cols();
		m_row = rand() % gp->rows();
		switch (r)
		{
		case 0:
			m_dir = 'A';
			break;
		case 1:
			m_dir = 'S';
			break;
		case 2:
			m_dir = 'D';
			break;
		case 3:
			m_dir = 'W';
			break;
		default:
			cout << "FATAL ERROR" << endl;
			exit(1);
			break;
		}
	}
	else
	{
		m_g = gp;
		m_valid = true;
		m_free = true;
		int r = rand() % 4;
		switch (r)
		{
		case 0:
			m_col = gp->cols() - 1;
			m_row = rand() % gp->rows();
			m_dir = 'A';
			break;
		case 1:
			m_col = rand() % gp->cols();
			m_row = 0;
			m_dir = 'S';
			break;
		case 2:
			m_col = 0;
			m_row = rand() % gp->rows();
			m_dir = 'D';
			break;
		case 3:
			m_col = rand() % gp->cols();
			m_row = gp->rows() - 1;
			m_dir = 'W';
			break;
		default:
			cout << "FATAL ERROR" << endl;
			exit(1);
			break;
		}
	}
}

Asteroid::Asteroid(Game* gp, BigAsteroid* bp, int r, int c) //gives an option to belong to a BigAsteroid, which limits freedom
{
	m_g = gp;
	m_b = bp;
	m_valid = true;
	m_free = false;
	m_dir = bp->dir();
	m_row = r;
	m_col = c;
}

void Asteroid::move() //moves the asteroid, and makes it invalid if out of bounds
{
	switch (dir())
	{
	case 'A':
		m_col--;
		if (col() < 0)
			m_valid = false;
		break;
	case 'S':
		m_row++;
		if (row() >= m_g->rows())
			m_valid = false;
		break;
	case 'D':
		m_col++;
		if (col() >= m_g->cols())
			m_valid = false;
		break;
	case 'W':
		m_row--;
		if (row() < 0)
			m_valid = false;
		break;
	}
	if (free() && (rand() % 4) == 2)
		changeDirection();
}

void Asteroid::changeDirection() //chooses a random, new direction
{
	int x = rand() % 4;
	switch (x)
	{
	case 0:
		m_dir = 'W';
		break;
	case 1:
		m_dir = 'A';
		break;
	case 2:
		m_dir = 'S';
		break;
	case 3:
		m_dir = 'D';
		break;
	}
}

///////////////////////////////////////////////////////////////////////////
//  BigAsteroid implementations
///////////////////////////////////////////////////////////////////////////

BigAsteroid::BigAsteroid(Game* gp) //initializes a new BigAsteroid, containing four limited asteroids
{
	m_g = gp;
	m_valid = true;
	int ra = rand() % 4;
	switch (ra)
	{
	case 0:
		m_col = gp->cols();
		m_row = 1 + rand() % (gp->rows() - 1);
		m_dir = 'A';
		break;
	case 1:
		m_col = 1 + rand() % (gp->cols() - 1);
		m_row = -1;
		m_dir = 'S';
		break;
	case 2:
		m_col = 0;
		m_row = 1 + rand() % (gp->rows() - 1);
		m_dir = 'D';
		break;
	case 3:
		m_col = 1 + rand() % (gp->cols() - 1);
		m_row = gp->rows() - 1;
		m_dir = 'W';
		break;
	default:
		cout << "FATAL ERROR" << endl;
		exit(1);
		break;
	}
	m_nAst = 4;
	group[0] = new Asteroid(m_g, this, row(), col());
	group[1] = new Asteroid(m_g, this, row(), col() - 1);
	group[2] = new Asteroid(m_g, this, row() + 1, col() - 1);
	group[3] = new Asteroid(m_g, this, row() + 1, col());
}

BigAsteroid::~BigAsteroid() //normal destructor for BigAsteroid
{
	for (int k = 0; k < 4; k++)
		if (group[k] != nullptr)
			delete group[k];
}

void BigAsteroid::move() //moves all of the contributing asteroids
{
	switch (dir())
	{
	case 'W':
		m_row--;
		if (row() < -1)
			m_valid = false;
		break;
	case 'A':
		m_col--;
		if (col() < 0)
			m_valid = false;
		break;
	case 'S':
		m_row++;
		if (row() > (m_g->rows() - 1))
			m_valid = false;
		break;
	case 'D':
		m_col++;
		if (col() > m_g->cols())
			m_valid = false;
		break;
	}
	for (int k = 0; k < 4; k++)
		if (group[k] != nullptr)
			group[k]->move();
}

void BigAsteroid::dstrAst(int ast) //destroys a contributing asteroid
{
	delete group[ast];
	m_nAst--;
	group[ast] = nullptr;
}

bool BigAsteroid::dstrAst(int r, int c) //destroys a contributing asteroid in a specific location
{
	for (int k = 0; k < 4; k++)
	{
		if (group[k] != nullptr && group[k]->row() == r && group[k]->col() == c)
		{
			delete group[k];
			group[k] = nullptr;
			m_nAst--;
			return true;
		}
	}
	return false;
}

bool BigAsteroid::isVisible(int ast) const //checks for contributing asteroids' visibility
{
	Asteroid* ap = group[ast];
	if (ap != nullptr && ap->row() >= 0 && ap->row() < m_g->rows() && ap->col() >= 0 && ap->col() < m_g->cols())
		return true;
	return false;
}

bool BigAsteroid::isOccupying(int r, int c) //checks if any contributing asteroids are occupying a space
{
	for (int k = 0; k < 4; k++)
		if (group[k] != nullptr && group[k]->row() == r && group[k]->col() == c)
			return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(Game* gp) //initializes a new Player randomly in the map
{
	m_g = gp;
	m_row = gp->rows() / 2;
	m_col = gp->cols() / 2;
	m_status = true;
	int x = rand() % 4;
	switch (x)
	{
	case 0:
		m_dir = '>';
		break;
	case 1:
		m_dir = '^';
		break;
	case 2:
		m_dir = '<';
		break;
	case 3:
		m_dir = 'v';
		break;
	}
}

void Player::shoot() //determines shot direction and calls a tracking function accordingly
{
	switch (dir())
	{
	case '>':
		m_g->track(this, 'D');
		break;
	case '^':
		m_g->track(this, 'W');
		break;
	case '<':
		m_g->track(this, 'A');
		break;
	case 'v':
		m_g->track(this, 'S');
		break;
	}
}

void Player::move(char dir) //translates and determines Player movement/direction
{
	switch (dir)
	{
	case 'W':
		if (m_dir == '<' || m_dir == '>')
		{
			m_dir = '^';
			break;
		}
		m_row--;
		if (row() < 0)
			m_row = m_g->rows() - 1;
		break;
	case 'A':
		if (m_dir == '^' || m_dir == 'v')
		{
			m_dir = '<';
			break;
		}
		m_col--;
		if (col() < 0)
			m_col = m_g->cols() - 1;
		break;
	case 'S':
		if (m_dir == '<' || m_dir == '>')
		{
			m_dir = 'v';
			break;
		}
		m_row++;
		if (row() >= m_g->rows())
			m_row = 0;
		break;
	case 'D':
		if (m_dir == '^' || m_dir == 'v')
		{
			m_dir = '>';
			break;
		}
		m_col++;
		if (col() >= m_g->cols())
			m_col = 0;
		break;
	}
}

///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int r, int c) //initializes a new Game
{
	if (r > MAXROWS || c > MAXCOLS)
	{
		cout << "FATAL BOUND ERROR" << endl;
		exit(1);
	}

	m_rows = r;
	m_cols = c;
	m_p = new Player(this);
	m_nAst = 0;
	m_nBigs = 0;
	for (int k = 0; k < r; k++)
		for (int kk = 0; kk < c; kk++)
			grid[k][kk] = '-';
}

Game::~Game() //cleans up the game
{
	delete m_p;
	for (int k = 0; k < nAst(); k++)
		delete asteroids[k];
	for (int k = 0; k < nBigs(); k++)
		delete bigs[k];
}

void Game::play() //begins the map's play loop
{
	for (int k = 0; k < 20; k++)
		spnAst('R');
	updateScreen();
	do
	{
		for (int k = 0; k < 2;)
			k += choice();

		for (int k = 0; k < nAst(); k++)
			asteroids[k]->move();
		for (int k = 0; k < nBigs(); k++)
			bigs[k]->move();

		int x = rand() % 4;
		if (x == 2)
			spnAst('B');
		if (x == 3)
			spnAst('b');
		updateScreen();
	} while (m_p->status() && nAst() > 0); //Level 1
}

void Game::track(Player* pp, char dir) //tracks a player's shot to find targets
{
	switch (dir)
	{
	case 'W':
		for (int k = pp->row() - 1; k >= 0; k--)
		{
			for (int c = 0; c < nAst(); c++)
				if (asteroids[c]->col() == pp->col() && asteroids[c]->row() == k)
				{
				dstrAst(c);
				return;
				}
			for (int c = 0; c < nBigs(); c++)
				if (bigs[c]->dstrAst(k, pp->col()))
					return;
		}
		break;
	case 'A':
		for (int k = pp->col() - 1; k >= 0; k--)
		{
			for (int c = 0; c < nAst(); c++)
				if (asteroids[c]->row() == pp->row() && asteroids[c]->col() == k)
				{
				dstrAst(c);
				return;
				}
			for (int c = 0; c < nBigs(); c++)
				if (bigs[c]->dstrAst(pp->row(), k))
					return;
		}
		break;
	case 'S':
		for (int k = pp->row() + 1; k < rows(); k++)
		{
			for (int c = 0; c < nAst(); c++)
				if (asteroids[c]->col() == pp->col() && asteroids[c]->row() == k)
				{
				dstrAst(c);
				return;
				}
			for (int c = 0; c < nBigs(); c++)
				if (bigs[c]->dstrAst(k, pp->col()))
					return;
		}
		break;
	case 'D':
		for (int k = pp->col() + 1; k < cols(); k++)
		{
			for (int c = 0; c < nAst(); c++)
				if (asteroids[c]->row() == pp->row() && asteroids[c]->col() == k)
				{
				dstrAst(c);
				return;
				}
			for (int c = 0; c < nBigs(); c++)
				if (bigs[c]->dstrAst(pp->row(), k))
					return;
		}
		break;
	default:
		cout << "FATAL ERROR" << endl;
		exit(1);
		break;
	}
}

void Game::spnAst() //legacy spawn function, overlapped by new function below
{
	if (nAst() >= MAXASTEROIDS)
		return;
	asteroids[nAst()] = new Asteroid(this);
	m_nAst++;
}

void Game::spnAst(char b) //spawn function with option for a BigAsteroid
{
	if (b != 'B')
	{
		if (nAst() >= MAXASTEROIDS)
			return;
		asteroids[nAst()] = new Asteroid(this, b);
		m_nAst++;
	}
	else
	{
		if (nBigs() >= MAXBIGS)
			return;
		bigs[m_nBigs] = new BigAsteroid(this);
		m_nBigs++;
	}
}

void Game::dstrAst(int ast) //destroys an asteroid
{
	delete asteroids[ast];
	m_nAst--;
	for (int k = ast; k < nAst(); k++)
		asteroids[k] = asteroids[k + 1];
}

void Game::dstrAst(BigAsteroid* bp, int ast) //destroys a small asteroid in a BigAsteroid
{
	bp->dstrAst(ast);
}

void Game::dstrBigAst(int ast) //destroys a BigAsteroid externally
{
	delete bigs[ast];
	m_nBigs--;
	for (int k = ast; k < nBigs(); k++)
		bigs[k] = bigs[k + 1];
}

void Game::update() //checks for asteroid validity, clears the map, and places objects on the map again (while checking for conflicts)
{
	for (int k = 0; k < nAst(); k++)
		if (!asteroids[k]->valid())
		{
			dstrAst(k);
			spnAst();
			k--;
		}

	for (int k = 0; k < nBigs(); k++)
		if (bigs[k]->nAst() == 0)
		{
			dstrBigAst(k);
			k--;
		}

	for (int k = 0; k < nBigs(); k++)
		if (!bigs[k]->valid())
		{
			dstrBigAst(k);
			spnAst('B');
			k--;
		}

	for (int k = 0; k < rows(); k++)
		for (int kk = 0; kk < cols(); kk++)
			grid[k][kk] = '-';


	grid[m_p->row()][m_p->col()] = m_p->dir();

	for (int k = 0; k < nAst(); k++)
	{
		grid[asteroids[k]->row()][asteroids[k]->col()] = 'O';
		if (asteroids[k]->row() == m_p->row() && asteroids[k]->col() == m_p->col())
			m_p->setDead();
	}

	for (int k = 0; k < nBigs(); k++)
		if (bigs[k]->isOccupying(m_p->row(), m_p->col()))
			m_p->setDead();

	for (int k = 0; k < nBigs(); k++)
	{
		if (bigs[k]->isVisible(0))
			grid[bigs[k]->row()][bigs[k]->col()] = '0';
		if (bigs[k]->isVisible(1))
			grid[bigs[k]->row()][bigs[k]->col() - 1] = '0';
		if (bigs[k]->isVisible(2))
			grid[bigs[k]->row() + 1][bigs[k]->col() - 1] = '0';
		if (bigs[k]->isVisible(3))
			grid[bigs[k]->row() + 1][bigs[k]->col()] = '0';
	}
}

void Game::display() //displays the map
{
	for (int k = 0; k < rows(); k++)
	{
		for (int kk = 0; kk < cols(); kk++)
			cout << grid[k][kk] << " ";
		cout << endl;
	}
	cout << endl;
}

void Game::updateScreen() //summarizes three common procedures
{
	clearScreen();
	update();
	display();
}

int Game::choice() //translates and determines a player's input
{
	string c;
	getline(cin, c);
	if (c == "f")
		m_p->shoot();
	else if (c == "w" || c == "a" || c == "s" || c == "d")
		if (c == "w")
			m_p->move('W');
		else if (c == "a")
			m_p->move('A');
		else if (c == "s")
			m_p->move('S');
		else
			m_p->move('D');
	else if (c == "")
		;
	else if (c == "q")
		exit(1);
	else
	{
		cout << "\a";
		updateScreen();
		return 0;
	}
	updateScreen();
	return 1;
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementations
///////////////////////////////////////////////////////////////////////////
//This code block is a given from an introductory CS class, CS31 and/or CS32, with professor David Smallberg

#ifdef _MSC_VER  //  Microsoft Visual C++

#include <windows.h>

void clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <cstring>

void clearScreen()  // will just write a newline in an Xcode output window
{
	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		cout << endl;
	else
	{
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
	}
}

#endif