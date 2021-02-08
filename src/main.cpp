#include <QtGlobal>
#if QT_VERSION >= 0x050000
	#include <QApplication>
#else
	#include <QtGui/QApplication>
#endif

#include <QTextCodec>
#include "matrix_character.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    //set code gbk .. 
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    matrix_character matrix;
    matrix.show();
    qDebug()<<"wchar_t size :"<<sizeof(wchar_t);
    return a.exec();
}
