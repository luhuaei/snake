#include "snake.h"
#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	Snake snake;

	snake.setWindowTitle("Snake Game");
	snake.show();
	return app.exec();
}
