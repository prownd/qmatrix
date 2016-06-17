#include <QtGui/QApplication>
#include <QTextCodec>
#include "matrix_character.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    //set gbk
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    matrix_character matrix;
    matrix.show();
	qDebug()<<"wchar_t size :"<<sizeof(wchar_t);
    return a.exec();
}
