#include "snake.h"
#include <cstring> // std::memset();

#include <QTime>
#include <QPainter>
#include <QObject>
#include <QtGlobal>
#include <QApplication>

Snake::Snake(QWidget *parent) : QWidget {parent} {
	// QWidget public slots, hold the Widget background color
	setStyleSheet("background-color: white;");

	inGame = false;
	ready = true;
	pause = false;
	no_border = true;

	loadImages();
	resize(B_WIDTH, B_HEIGHT);
}

void Snake::loadImages() {
	QImage *large_dot = new QImage("./img/circle.png");
	dot = large_dot->scaled(DOT_SIZE, DOT_SIZE, Qt::IgnoreAspectRatio);
	QImage *large_snake = new QImage("./img/snake.png");
	head = large_snake->scaled(DOT_SIZE, DOT_SIZE, Qt::IgnoreAspectRatio);
	QImage *large_apple = new QImage("./img/apple.png");
	apple = large_apple->scaled(DOT_SIZE, DOT_SIZE, Qt::IgnoreAspectRatio);
}

// key press trigger event
void Snake::keyPressEvent(QKeyEvent *e){
	int key = e->key();

	// system key event
	if ((key == Qt::Key_Q))
		QApplication::quit();

	// when turn left, the before direction don't is right direction
	// the other is similar
	if (inGame) {
		if (((key == Qt::Key_Left) || (key == Qt::Key_A)) && (!rightDirection) && (!pause)) {
			leftDirection = true;
			upDirection = false;
			downDirection = false;
		}

		if (((key == Qt::Key_Right) || (key == Qt::Key_D)) && (!leftDirection) && (!pause)) {
			rightDirection = true;
			upDirection = false;
			downDirection = false;
		}

		if (((key == Qt::Key_Up) || (key == Qt::Key_W)) && (!downDirection) && (!pause)) {
			upDirection = true;
			leftDirection = false;
			rightDirection = false;
		}

		if (((key == Qt::Key_Down) || (key == Qt::Key_S)) && (!upDirection) && (!pause)) {
			downDirection = true;
			leftDirection = false;
			rightDirection = false;
		}

		// restart game, because direct exec initGame with lead
		// the timer don't stop will make the snake move speed very
		// quick on more times press R. So kill the timer and initGame
		// again.
		if ((key == Qt::Key_R)){
			killTimer(timerId);
			initGame();
		}

		if ((key == Qt::Key_Space) && inGame) {
			if (pause) {
				pause = false;
				timerId = startTimer(DELAY);
			} else {
				pause = true;
				killTimer(timerId);
			}
		}

	} else {
		initGame();				// any key begin game;
	}

	QWidget::keyPressEvent(e);
}


void Snake::initGame() {
	std::memset(x, 0, sizeof(x));
	std::memset(y, 0, sizeof(y));

	leftDirection = false;
	rightDirection = true;
	upDirection = false;
	downDirection = false;

	inGame = true;
	ready = false;
	dots = 3;					// begin game snake length

	// the begin dot position
	for (int z = 0; z < dots; z++) {
		x[z] = 50 - z * 10;
		y[z] = 50;
	}

	locateApple();
	timerId = startTimer(DELAY); // startTimer is the QObject function, trigger time event
}

void Snake::locateApple() {
	QTime time = QTime::currentTime(); // with current time is seed
	// qsrand is the qtglobal function, but unit is qt function for trbanslate to unsigned int
	qsrand((uint) time.msec());

	int r = qrand() % RAND_POS;	// random generation position
	apple_x = (r * DOT_SIZE);

	r = qrand() % RAND_POS;
	apple_y = (r * DOT_SIZE);
}

// time trigger event when startTimer begin,
// repaint is QWidget public function, refresh the interface,
// and then time trigger the paintEvent.
void Snake::timerEvent(QTimerEvent *e) {
	Q_UNUSED(e);

	if (inGame) {
		checkApple();
		checkCollision();
		move();
		repaint();
	}

	if (!inGame)
		repaint();

}

void Snake::checkApple() {
	// if the snake head collides the apply, add the snake length
	// recreate the random position for apply;
	if ((x[0] == apple_x) && (y[0] == apple_y)) {
		dots++;
		locateApple();
	}
}

void Snake::move() {
	// all position forward
	for (int z = dots; z > 0; z--) {
		x[z] = x[(z - 1)];
		y[z] = y[(z - 1)];
	}

	// change the direction we only change the first element,
	// the other element will forward update on for loop above;
	// because each dot size is the DOT_SIZE, so the snake head(first element)
	// every modified with DOT_SIZE;
	if (leftDirection)
		x[0] -= DOT_SIZE;

	if (rightDirection)
		x[0] += DOT_SIZE;

	if (upDirection)
		y[0] -= DOT_SIZE;

	if (downDirection)
		y[0] += DOT_SIZE;
}

void Snake::checkCollision() {
	// dots is the snake length, if length more than 4, it probably touch
	// the snake body or tail.
	for (int z = dots; z > 0; z--) {
		if ((z > 4) && (x[0] == x[z]) && (y[0] == y[z])) {
			inGame = false;
		}
	}

	if (no_border) {
		if (y[0] >= B_HEIGHT)
			y[0] = 0;

		if (y[0] < 0)
			y[0] = B_HEIGHT;

		if (x[0] >= B_WIDTH)
			x[0] = 0;

		if (x[0] < 0)
			x[0] = B_WIDTH;
	} else {
		// touch the ceiling
		if (y[0] >= B_HEIGHT)
			inGame = false;
		// reach the floor
		if (y[0] < 0)
			inGame = false;
		// reach the left board
		if (x[0] < 0)
			inGame = false;
		if (x[0] >= B_WIDTH)
			inGame = false;
	}

	if (!inGame)
		killTimer(timerId);
}

void Snake::paintEvent(QPaintEvent *e) {
	Q_UNUSED(e);				// QtGloabl decorator

	doDrawing();
}

void Snake::doDrawing() {
	QPainter qp(this);

	if (inGame) {
		qp.drawImage(apple_x, apple_y, apple); // apple position
		for (int z = 0; z < dots; z++) {
			if (z == 0)
				qp.drawImage(x[z], y[z], head); // snake head
			else
				qp.drawImage(x[z], y[z], dot); // snake body
		}
	}
	else
		gameOver(qp);
}

void Snake::gameOver(QPainter &qp) {
	QString message;
	if (ready)
		message = "Game Ready!";
	else
		message = "Game Over!";
	QFont font("Courier", 15, QFont::DemiBold);
	QFontMetrics fm(font);
	int textWidth = fm.width(message);

	qp.setFont(font);
	qp.setPen(Qt::black);		// setting the text color
	int h = height();			// height and width is the QWidget property.
	int w = width();

	qp.translate(QPoint(w/2, h/2)); // translate the corrdinate system;
	qp.drawText(-textWidth/2, 0, message); // center the message
}
