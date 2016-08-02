#include "matrix_character.h"
#include<QFontDatabase>


#ifdef Q_OS_LINUX
#include <X11/extensions/shape.h>
#include <QX11Info>
#endif
#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QDesktopWidget>
#include <QRect>
#define WINDOW_OPACITY		0.5
#define STRMAXLEN  25 
#define STRMINLEN  8  

matrix_character::matrix_character(QWidget *parent) :
    QWidget(parent)
{
    //this->setMaximumSize(QSize(QApplication::desktop()->width(),QApplication::desktop()->height()));
    //setWindowFlags(Qt::FramelessWindowHint);
    //setGeometry(0, 0, qApp->desktop()->width(), qApp->desktop()->height());
    /*setWindowFlags(windowFlags()
                   |Qt::FramelessWindowHint 
                   |Qt::X11BypassWindowManagerHint 
                   //|Qt::WindowStaysOnBottomHint 
                   //|Qt::Tool 
                    );

    setWindowState(Qt::WindowNoState 
                   |Qt::WindowFullScreen 
                   );

    */
    //setFocusPolicy(Qt::NoFocus);
    setFocusPolicy(Qt::StrongFocus);
    //setWindowOpacity(WINDOW_OPACITY);
/*
#ifdef Q_OS_LINUX
    XShapeCombineRectangles(QX11Info::display(), winId(), ShapeInput, 0,
                                0, NULL, 0, ShapeSet, YXBanded);
#endif
#ifdef Q_OS_WIN
    SetWindowLong(winId(), GWL_EXSTYLE, GetWindowLong(winId(), GWL_EXSTYLE) |
                        WS_EX_TRANSPARENT | WS_EX_LAYERED);
#endif
*/

    //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);


    //this->setWindowOpacity(1);
    //this->setAttribute(Qt::WA_TranslucentBackground, true);
    setAutoFillBackground(true);
    this->setCursor(Qt::BlankCursor);
    QPalette palette;
    //palette.setColor(QPalette::Background, QColor(192,253,123));
    palette.setColor(QPalette::Background, QColor(0,0,0));
    //palette.setBrush(QPalette::Background, QBrush(QPixmap(":/background.png")));
    setPalette(palette);
    showFullScreen();

    cxScreen=this->width ();
    cyScreen=this->height ();

    /*
    QDesktopWidget *desktop = QApplication::desktop();
	QRect screen = desktop->screenGeometry();
	cxScreen=screen.width();
	cyScreen=screen.height();
    */

	qDebug()<<"screen height:"<<this->height ();
    qDebug()<<"screen width:"<<this->width ();

	//QFont ifont("arial", iFontHeight,iFontWidth, QFont::Black);
    //QFont ifont("Fixedsys", iFontHeight,iFontWidth, QFont::Black);
    pointSize=30;
    //fonts the type for example:  simsun, Times,jmyinzhuanjiugongsx
    //QFont ifont("Times", pointSize,QFont::Bold);
    QFont ifont(tr("FZJingLeiS-R-GB"), pointSize,QFont::Bold);
    setFont (ifont);

    //print font family .
    QFontDatabase database;
    foreach(const QString &family,database.families())
    {
        ;//qDebug ()<<"family is:"<<family;
    }

    QFontMetrics fm(ifont);
    int pixelsWidth = fm.width("w");// char w width.
    int pixelsHeight = fm.height(); //the font height
    qDebug ()<<"font width:"<<pixelsWidth;
    qDebug ()<<"font height:"<<pixelsHeight;

    iFontHeight=pixelsHeight+10;
    iFontWidth=pixelsWidth+35;

    qsrand(QTime::currentTime().msec());//set seed of qrand

    iColumnCount = cxScreen/(iFontWidth*3/2); 
    //alloc memory ,and init the column.generate the dobule circle linklist.
    ccChain = (pCharColumn)calloc(iColumnCount, sizeof(CharColumn));
    for(i=0; i<iColumnCount; i++)
    {
       init(ccChain+i, cyScreen, (iFontWidth*3/2)*i);
    }


    setMouseTracking(true); // default case is not respond,you will manual setup it .


    //init cursor pos_x ,pos_y;
    pos_x=0;
    pos_y=0;
    frash_timer=new QTimer();
    frash_timer->setInterval (20);// millsecond
    frash_timer->start ();
    connect(frash_timer,SIGNAL(timeout()),this,SLOT(processChange()));
}

matrix_character::~matrix_character(){
	for(i=0; i<iColumnCount; i++)
    {
      free( (ccChain+i)->current );
    }
    free(ccChain);
    delete frash_timer;
}

void matrix_character::paintScreenText(){
    QString str;
    QPainter painter(this);
    QColor color;
    for(i=0; i<iColumnCount; i++)
    {
       ctn = (ccChain+i)->iStopTimes++ > (ccChain+i)->iMustStopTimes;
       (ccChain+i)->point = (ccChain+i)->head; 

       //first char ,the color is white
       //SetTextColor(hdcMem, RGB(255, 255, 255));
       //TextOut(hdcMem, (ccChain+i)->x, (ccChain+i)->y, &((ccChain+i)->point->ch), 1);
       color.setRgb (255, 255, 255);
       painter.setPen(color);
       str=QString((ccChain+i)->point->ch);
       painter.drawText ((ccChain+i)->x,(ccChain+i)->y,str);

       j = (ccChain+i)->y;
       (ccChain+i)->point = (ccChain+i)->point->next;
       temp = 0 ; //temp from green to black
       while((ccChain+i)->point != (ccChain+i)->head && (ccChain+i)->point->ch)
       {
               //SetTextColor(hdcMem, RGB(0, 255-(255*(temp++)/(ccChain+i)->iStrLen), 0));
               //TextOut(hdcMem, (ccChain+i)->x, j-=iFontHeight, &((ccChain+i)->point->ch), 1);

               //colnum color diff
               if(i%3==0){
                    color.setRgb (255-(255*(temp++)/(ccChain+i)->iStrLen),0, 0);
               }else if(i%3==1){
                    color.setRgb (0, 255-(255*(temp++)/(ccChain+i)->iStrLen), 0);
               }else{
                    color.setRgb (0,0,255-(255*(temp++)/(ccChain+i)->iStrLen));
               }
               painter.setPen(color);
               str=QString((ccChain+i)->point->ch);
			   //QString ss=QTextCodec::codecForName("GBK")->toUnicode(str.toLocal8Bit());
               painter.drawText ((ccChain+i)->x,j-=iFontHeight,str);
               (ccChain+i)->point = (ccChain+i)->point->next;
       }
       if(ctn)
                (ccChain+i)->iStopTimes = 0 ;
       else continue;
                (ccChain+i)->y += iFontHeight; 
       if( (ccChain+i)->y-(ccChain+i)->iStrLen*iFontHeight > cyScreen)
       {
                free( (ccChain+i)->current );
                init(ccChain+i, cyScreen, (iFontWidth*3/2)*i);
       }
       (ccChain+i)->head = (ccChain+i)->head->prev;
       (ccChain+i)->head->ch = randomChar();
    }
}

 void matrix_character::processChange(){
     update();
 }

 void matrix_character::paintEvent(QPaintEvent *e){
     paintScreenText();
 }

 void matrix_character::keyPressEvent(QKeyEvent *e){
     switch(e->key())
     {
         case Qt::Key_Up:
                frash_timer->stop ();
                frash_timer->start();
                break;
         case Qt::Key_Down:
                frash_timer->stop ();
                break;
          case Qt::Key_Enter:
               qDebug()<<"key:"<<e->key();
                //frash_timer->stop();
                //this->close();
         case Qt::Key_Escape:
        case Qt::Key_Delete:
            frash_timer->stop();
            qApp->quit();
         default:
                //frash_timer->stop ();
                //this->close();
                break;
     }
 }
 void matrix_character::keyReleaseEvent(QKeyEvent *e){
    qDebug()<<"key:"<<e->key();
 }
 int matrix_character::init(CharColumn *cc, int cyScreen, int x) 
 {
         int j;
         cc->iStrLen = rand()%(STRMAXLEN-STRMINLEN) + STRMINLEN; 
         cc->x = x+3 ;        
         cc->y =rand()%3?rand()%cyScreen:0; 
         cc->iMustStopTimes = rand()%6 ;
         cc->iStopTimes    = 0 ;
         cc->head = cc->current =
                         (pCharChain)calloc(cc->iStrLen, sizeof(CharChain)); 
         for(j=0; j<cc->iStrLen-1; j++)
         {
                 cc->current->prev = cc->point; 
                 cc->current->ch  = '\0';
                 cc->current->next = cc->current+1; 
                 cc->point          = cc->current++; //cc->point = cc->current; cc->current++;
         }
         cc->current->prev = cc->point; 
         cc->current->ch  = '\0';
         cc->current->next = cc->head;
         cc->head->prev    = cc->current; 
         cc->current = cc->point = cc->head; 
         cc->head->ch = randomChar(); 
         return 0;
 }

 TCHAR matrix_character::randomChar() 
 {
         //return (TCHAR)(qrand()%(126-33)+33); //33到126之间
         //return (TCHAR)(rand()%(40869-19968)+19968); //19968到40869之间 cjk
         return (TCHAR)(rand()%(21035-19968)+19968); //19968到26735之间 gb2312
         //start 4e00 len: 6768  end: 26735
 }

 void matrix_character::mousePressEvent (QMouseEvent *e){

     QString str="("+QString::number(e->x())+","+QString::number(e->y())+")";
     if(e->button()==Qt::LeftButton)
     {
        qDebug ()<<(tr("1 left mouse is press down:")+str);
     }
 }
 void matrix_character::mouseReleaseEvent (QMouseEvent *e){
     QString str="("+QString::number(e->x())+","+QString::number(e->y())+")";
     if(e->button()==Qt::LeftButton)
     {
         qDebug ()<<(tr("2 left mouse is release:")+str);
     }
 }
 void matrix_character::mouseMoveEvent (QMouseEvent *e){

     QString str="("+QString::number(e->x())+","+QString::number(e->y())+")";
     //if(e->buttons()&Qt::LeftButton)
     if(pos_x==0&&pos_y==0)
     {
        pos_x=e->x ();
        pos_y=e->y ();
        return;
     }
     if(e->x ()-pos_x>=100 || e->y ()-pos_y>=100)
        this->close();
     pos_x=e->x ();
     pos_y=e->y ();
 }
