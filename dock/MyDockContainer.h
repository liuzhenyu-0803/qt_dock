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

    // 模拟 QMainWindow::addDockWidget  
    void addDockWidget(Qt::DockWidgetArea area, MyDockWidget *dockWidget);

    // 模拟 QMainWindow::setCentralWidget  
    void setCentralWidget(QWidget *widget);

protected:
    // 在这里我们管理四个方向 + 中心区  
    // mainSplitter_为总的垂直分割器：  
    //   第一个区域：topSplitter_ (用于顶部dock)  
    //   第二个区域：centerAndBottomSplitter_ (再里面水平或垂直分割)  
    // centerAndBottomSplitter_ 用于把 centerWidget_ 与 bottomSplitter_ 分割  
    // leftSplitter_ 和 rightSplitter_ 放在 centerWidget_ 两侧  
    // 具体可以根据自己需求拓展  
    QSplitter *mainSplitter_ = nullptr;           // 垂直方向分割器(上 / 中+下)  
    QSplitter *centerAndBottomSplitter_ = nullptr;// 垂直方向分割器(中 / 下)  
    QSplitter *centerHorizontalSplitter_ = nullptr;// 水平方向分割器(左 / 中 / 右)  
    QSplitter *topSplitter_ = nullptr;            // 放多个Top Dock  
    QSplitter *bottomSplitter_ = nullptr;         // 放多个Bottom Dock  
    QSplitter *leftSplitter_ = nullptr;           // 放多个Left Dock  
    QSplitter *rightSplitter_ = nullptr;          // 放多个Right Dock  

    QFrame *center_frame = nullptr;                // 中心部件的Frame
    QWidget *center_widget = nullptr;             // 中心部件  
};