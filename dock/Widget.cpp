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

    // ���� DockContainer  
    MyDockContainer *dockContainer = new MyDockContainer(this);
    mainLayout->addWidget(dockContainer);

    // ģ��һ�����Ĳ���  
    // �� QMainWindow ͨ�� setCentralWidget  
    // ���������Լ���һ����ͨ�ؼ���Ϊ center  
    QWidget *centerWidget = new QWidget();
    centerWidget->setObjectName("centerWidget");
    centerWidget->setStyleSheet("#centerWidget{background-color: #FF0000;}");
    dockContainer->setCentralWidget(centerWidget);

    // ������� DockWidget  
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

    // �� DockWidget ͣ������ͬ��λ  
    //dockContainer->addDockWidget(Qt::LeftDockWidgetArea, dockLeft);
    //dockContainer->addDockWidget(Qt::RightDockWidgetArea, dockRight);
    //dockContainer->addDockWidget(Qt::TopDockWidgetArea, dockTop);
    //dockContainer->addDockWidget(Qt::BottomDockWidgetArea, dockBottom);

    // Ҳ���������һ����������������һ��  
    //dockContainer->addDockWidget(Qt::LeftDockWidgetArea, dockExtra);
}

Widget::~Widget()
{}
