#pragma once  

#include <QFrame>  
#include <QSplitter>  
#include <QMap>  
#include <QPointer>  
#include <QVector>  
#include <QWidget>  
#include <QTabWidget>  
#include "MyDockWidget.h"  

class MyDockContainer : public QFrame
{
    Q_OBJECT
public:
    explicit MyDockContainer(QWidget *parent = nullptr);

    // ģ�� QMainWindow::addDockWidget  
    void addDockWidget(Qt::DockWidgetArea area, MyDockWidget *dockWidget);

    // ģ�� QMainWindow::setCentralWidget  
    void setCentralWidget(QWidget *widget);

protected:
    // ���������ǹ����ĸ����� + ������  
    // mainSplitter_Ϊ�ܵĴ�ֱ�ָ�����  
    //   ��һ������topSplitter_ (���ڶ���dock)  
    //   �ڶ�������centerAndBottomSplitter_ (������ˮƽ��ֱ�ָ�)  
    // centerAndBottomSplitter_ ���ڰ� centerWidget_ �� bottomSplitter_ �ָ�  
    // leftSplitter_ �� rightSplitter_ ���� centerWidget_ ����  
    // ������Ը����Լ�������չ  
    QSplitter *mainSplitter_ = nullptr;           // ��ֱ����ָ���(�� / ��+��)  
    QSplitter *centerAndBottomSplitter_ = nullptr;// ��ֱ����ָ���(�� / ��)  
    QSplitter *centerHorizontalSplitter_ = nullptr;// ˮƽ����ָ���(�� / �� / ��)  
    QSplitter *topSplitter_ = nullptr;            // �Ŷ��Top Dock  
    QSplitter *bottomSplitter_ = nullptr;         // �Ŷ��Bottom Dock  
    QSplitter *leftSplitter_ = nullptr;           // �Ŷ��Left Dock  
    QSplitter *rightSplitter_ = nullptr;          // �Ŷ��Right Dock  

    QFrame *center_frame = nullptr;                // ���Ĳ�����Frame
    QWidget *center_widget = nullptr;             // ���Ĳ���  
};