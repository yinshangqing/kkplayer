#include "stdafx.h"
#include "KkPlayer.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	QApplication::addLibraryPath("./plugins");
	KkPlayer w;
	w.show();
	return a.exec();
}
