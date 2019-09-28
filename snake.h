// snake header file
#pragma once

#include <QWidget>
#include <QKeyEvent>

// 继承QWidget类
class Snake : public QWidget {
public:
	Snake(QWidget *parent = nullptr);

protected:
	void paintEvent(QPaintEvent *);
	void timerEvent(QTimerEvent *);
	void keyPressEvent(QKeyEvent *);

private:
	QImage dot;
	QImage head;
	QImage apple;

	static const int B_WIDTH = 300; // B is board
	static const int B_HEIGHT = 300;
	static const int DOT_SIZE = 10;	 // DOT is apple and make snake pixel
	static const int ALL_DOTS = 900; // 900 = (300 * 300) / (10 * 10)
	static const int RAND_POS = 29;	 // RAND is random position
	static const int DELAY = 140; // delay time

	int timerId;
	int dots;
	int apple_x;
	int apple_y;

	int x[ALL_DOTS];
	int y[ALL_DOTS];

	bool leftDirection;
	bool rightDirection;
	bool upDirection;
	bool downDirection;
	bool inGame;
	bool ready;
	bool pause;
	bool no_border;

	void loadImages();
	void initGame();			// begin and trigger the timerEvent
	void locateApple();			// computer the apple_x, and apple_y
	void checkApple();			//
	void checkCollision();
	void move();
	void doDrawing();
	void gameOver(QPainter &);	// accept QPainter reference
};
