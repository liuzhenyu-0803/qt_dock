#include "Widget.h"

#include "MyDockWidget.h"
#include "MyDockContainer.h"

#include <QVBoxLayout>



Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    // 创建 DockContainer  
    MyDockContainer *dockContainer = new MyDockContainer(this);
    mainLayout->addWidget(dockContainer);

    // 模拟一个中心部件  
    // 在 QMainWindow 通常 setCentralWidget  
    // 这里我们自己放一个普通控件作为 center  
    QWidget *centerWidget = new QWidget();
    centerWidget->setObjectName("centerWidget");
    centerWidget->setStyleSheet("#centerWidget{background-color: #FF0000;}");
    dockContainer->setCentralWidget(centerWidget);

    // 添加若干 DockWidget  
    MyDockWidget *dockLeft = new MyDockWidget("Dock Left");
    dockLeft->setObjectName("dockLeft");
    //auto dockLeftFrame = new QFrame();
    //dockLeftFrame->setStyleSheet("background-color: #00FF00;");
    //dockLeft->setContentWidget(dockLeftFrame);
    dockContainer->addDockWidget(Qt::LeftDockWidgetArea, dockLeft);

    //MyDockWidget *dockRight = new MyDockWidget("Dock Right");
    //auto dockRightFrame = new QFrame();
    //dockRightFrame->setStyleSheet("background-color: #0000FF;");
    //dockRight->setContentWidget(new QPushButton("Right Dock Content"));

    //MyDockWidget *dockTop = new MyDockWidget("Dock Top");
    //dockTop->setContentWidget(new QPushButton("Top Dock Content"));

    //MyDockWidget *dockBottom = new MyDockWidget("Dock Bottom");
    //dockBottom->setContentWidget(new QPushButton("Bottom Dock Content"));

    //MyDockWidget *dockExtra = new MyDockWidget("Dock Extra");
    //QPushButton *btnExtra = new QPushButton("Extra Dock Content");
    //dockExtra->setContentWidget(btnExtra);

    // 将 DockWidget 停靠到不同方位  
    //dockContainer->addDockWidget(Qt::LeftDockWidgetArea, dockLeft);
    //dockContainer->addDockWidget(Qt::RightDockWidgetArea, dockRight);
    //dockContainer->addDockWidget(Qt::TopDockWidgetArea, dockTop);
    //dockContainer->addDockWidget(Qt::BottomDockWidgetArea, dockBottom);

    // 也可以再添加一个，比如再左侧添加一个  
    //dockContainer->addDockWidget(Qt::LeftDockWidgetArea, dockExtra);
}

Widget::~Widget()
{}
