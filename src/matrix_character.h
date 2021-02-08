#ifndef MATRIX_CHARACTER_H
#define MATRIX_CHARACTER_H

#include <QWidget>
#include <QPalette>
#include <QPainter>
#include <QBitmap>
#include <QDebug>
#include<QTimer>
#include<QTime>
#include<QList>
#include<QKeyEvent>
#include<QPaintEvent>

#include <QtGlobal>
#if QT_VERSION >= 0x050000
	#include <QApplication>
#else
	#include <QtGui/QApplication>
#endif
#include <QTextCodec>

#define UNICODE

#ifdef UNICODE
typedef wchar_t TCHAR;
#else
typedef char TCHAR;
#endif


//#define TCHAR char

typedef struct tagCharChain 
{
        struct tagCharChain *prev; 
        TCHAR  ch;                  
        struct tagCharChain *next; 
}CharChain, *pCharChain;
typedef struct tagCharColumn
{
        CharChain *head, *current, *point;
        int x, y, iStrLen; 
        int iStopTimes, iMustStopTimes; 
}CharColumn, *pCharColumn;

class matrix_character : public QWidget
{
    Q_OBJECT
public:
    explicit matrix_character(QWidget *parent = 0);
    ~matrix_character();
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent (QMouseEvent * e);
    void mouseMoveEvent (QMouseEvent * e);
    int init(CharColumn *cc, int cyScreen, int x);
    TCHAR randomChar();
    void paintScreenText();
    
signals:
    
public slots:
    void processChange();
   private:
    QTimer * frash_timer;
    int cxScreen, cyScreen; 
    int iFontWidth;
    int iFontHeight;
    int iColumnCount;
    CharColumn *ccChain;
    int pointSize;

    int i, j;
    int temp, ctn;
    int pos_x;
    int pos_y;
};

#endif // MATRIX_CHARACTER_H
