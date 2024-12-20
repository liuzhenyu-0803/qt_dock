#include "MyDockContainer.h"
#include "MyDockContainer.h"  
#include <QHBoxLayout>  
#include <QVBoxLayout>  
#include <QTabWidget>
#include <QSplitter>
#include <QTimer>
#include <QDebug>  

// 帮助函数：将新的 DockWidget 添加到指定 splitter 中  
// 这里简化为，每个 splitter 都用一个 QTabWidget 来存放多个 DockWidget  
// 你可以改造成多个 splitter 嵌套，或更灵活的布局  
static void addDockToSplitter(QSplitter *splitter, MyDockWidget *dockWidget)
{
    if (!splitter || !dockWidget) return;

    splitter->addWidget(dockWidget);

    // 查找或者创建一个QTabWidget放进去  
    // 这里假设最后一个Widget一定是 QTabWidget（也可以一个splitter里放多个TabWidget）  
    // 或者可以给splitter动态插入新的QTabWidget  
    //if (splitter->count() == 0)
    //{
    //    QTabWidget *tab = new QTabWidget(splitter);
    //    tab->addTab(dockWidget, dockWidget->windowTitle());
    //    splitter->addWidget(tab);
    //}
    //else
    //{
    //    // 简化处理：直接在第一个位置的 QTabWidget 上追加  
    //    QWidget *w = splitter->widget(0);
    //    QTabWidget *tab = qobject_cast<QTabWidget *>(w);
    //    if (tab) {
    //        tab->addTab(dockWidget, dockWidget->windowTitle());
    //    }
    //    else {
    //        // 如果不是 tab，就忽略或自行处理  
    //        qWarning() << "No QTabWidget found in splitter. Creating a new one.";
    //        QTabWidget *newTab = new QTabWidget(splitter);
    //        newTab->addTab(dockWidget, dockWidget->windowTitle());
    //        splitter->addWidget(newTab);
    //    }
    //}
}

MyDockContainer::MyDockContainer(QWidget *parent)
    : QFrame(parent)
{
    // 整体布局：最外层垂直分割器  
    mainSplitter_ = new QSplitter(Qt::Vertical, this);
    mainSplitter_->setObjectName("MainVerticalSplitter");
    //mainSplitter_->setChildrenCollapsible(false);

    QTimer::singleShot(1000, this, [this] {
        qDebug() << "MyDockContainer::SingleShot: " << mainSplitter_->size();
    });

    // 顶部Splitter（用于Dock Top）  
    topSplitter_ = new QSplitter(Qt::Horizontal, mainSplitter_);
    topSplitter_->setObjectName("TopSplitter");

    // 中间区域 + 底部 通过另一个垂直 QSplitter 分割  
    centerAndBottomSplitter_ = new QSplitter(Qt::Vertical, mainSplitter_);
    centerAndBottomSplitter_->setObjectName("CenterAndBottomSplitter");

    // centerHorizontalSplitter_：左右分割器（Left / Center / Right）  
    centerHorizontalSplitter_ = new QSplitter(Qt::Horizontal, centerAndBottomSplitter_);
    centerHorizontalSplitter_->setObjectName("CenterHorizontalSplitter");

    // 左侧Splitter  
    leftSplitter_ = new QSplitter(Qt::Vertical, centerHorizontalSplitter_);
    leftSplitter_->setObjectName("LeftSplitter");

    // centerWidget_ 在 centerHorizontalSplitter_ 中居中放置  
    center_frame = new QFrame(centerHorizontalSplitter_);
    center_frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto center_frame_layout = new QHBoxLayout(center_frame);
    center_frame_layout->setSpacing(0);
    center_frame_layout->setMargin(0);

    // 右侧Splitter  
    rightSplitter_ = new QSplitter(Qt::Vertical, centerHorizontalSplitter_);
    rightSplitter_->setObjectName("RightSplitter");

    // 底部Splitter  
    bottomSplitter_ = new QSplitter(Qt::Horizontal, centerAndBottomSplitter_);
    bottomSplitter_->setObjectName("BottomSplitter");


    //auto top_frame = new QFrame(topSplitter_);
    //top_frame->setStyleSheet("background-color: #ff0000;");

    //auto left_frame = new QFrame(leftSplitter_);
    //left_frame->setStyleSheet("background-color: #00ff00;");

    //center_frame->setStyleSheet("background-color: #0000ff;");

    //auto right_frame = new QFrame(rightSplitter_);
    //right_frame->setStyleSheet("background-color: #ff00ff;");

    //auto bottom_frame = new QFrame(bottomSplitter_);
    //bottom_frame->setStyleSheet("background-color: #00ffff;");

    // 组装好后，把 mainSplitter_ 作为唯一Widget放到本 QFrame 的布局中  
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(mainSplitter_);
    setLayout(layout);
}

void MyDockContainer::setCentralWidget(QWidget *widget)
{
    if (!centerHorizontalSplitter_) return;

    if (center_widget) {
        // 移除旧的中心部件  
        center_widget->setParent(nullptr);
        center_widget = nullptr;
    }

    center_widget = widget;
    if (center_widget)
    {
        center_frame->layout()->addWidget(center_widget);
    }
}

void MyDockContainer::addDockWidget(Qt::DockWidgetArea area, MyDockWidget *dockWidget)
{
    if (!dockWidget) return;

    // 将 dockWidget 初始 parent 设置为本容器  
    dockWidget->setParent(this);

    // 根据 area 选择放到对应 splitter  
    switch (area) {
    case Qt::LeftDockWidgetArea:
        addDockToSplitter(leftSplitter_, dockWidget);
        break;
    case Qt::RightDockWidgetArea:
        addDockToSplitter(rightSplitter_, dockWidget);
        break;
    case Qt::TopDockWidgetArea:
        addDockToSplitter(topSplitter_, dockWidget);
        break;
    case Qt::BottomDockWidgetArea:
        addDockToSplitter(bottomSplitter_, dockWidget);
        break;
    case Qt::AllDockWidgetAreas:
    case Qt::NoDockWidgetArea:
    default:
        // 默认放中间（可自定义逻辑）  
        //if (centerWidget_) {
        //    // 也可以把它当做一个额外Tab或独立分割处理  
        //    addDockToSplitter(leftSplitter_, dockWidget);
        //}
        //else {
        //    addDockToSplitter(leftSplitter_, dockWidget);
        //}
        break;
    }

    // 设置 dockWidget 的初始 container (在此示例中可不做额外记录)  
    dockWidget->setDockContainer(this);
}

