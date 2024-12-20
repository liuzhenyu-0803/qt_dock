#include "MyDockContainer.h"
#include "MyDockContainer.h"  
#include <QHBoxLayout>  
#include <QVBoxLayout>  
#include <QTabWidget>
#include <QSplitter>
#include <QTimer>
#include <QDebug>  

// �������������µ� DockWidget ��ӵ�ָ�� splitter ��  
// �����Ϊ��ÿ�� splitter ����һ�� QTabWidget ����Ŷ�� DockWidget  
// ����Ը���ɶ�� splitter Ƕ�ף�������Ĳ���  
static void addDockToSplitter(QSplitter *splitter, MyDockWidget *dockWidget)
{
    if (!splitter || !dockWidget) return;

    splitter->addWidget(dockWidget);

    // ���һ��ߴ���һ��QTabWidget�Ž�ȥ  
    // ����������һ��Widgetһ���� QTabWidget��Ҳ����һ��splitter��Ŷ��TabWidget��  
    // ���߿��Ը�splitter��̬�����µ�QTabWidget  
    //if (splitter->count() == 0)
    //{
    //    QTabWidget *tab = new QTabWidget(splitter);
    //    tab->addTab(dockWidget, dockWidget->windowTitle());
    //    splitter->addWidget(tab);
    //}
    //else
    //{
    //    // �򻯴���ֱ���ڵ�һ��λ�õ� QTabWidget ��׷��  
    //    QWidget *w = splitter->widget(0);
    //    QTabWidget *tab = qobject_cast<QTabWidget *>(w);
    //    if (tab) {
    //        tab->addTab(dockWidget, dockWidget->windowTitle());
    //    }
    //    else {
    //        // ������� tab���ͺ��Ի����д���  
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
    // ���岼�֣�����㴹ֱ�ָ���  
    mainSplitter_ = new QSplitter(Qt::Vertical, this);
    mainSplitter_->setObjectName("MainVerticalSplitter");
    //mainSplitter_->setChildrenCollapsible(false);

    QTimer::singleShot(1000, this, [this] {
        qDebug() << "MyDockContainer::SingleShot: " << mainSplitter_->size();
    });

    // ����Splitter������Dock Top��  
    topSplitter_ = new QSplitter(Qt::Horizontal, mainSplitter_);
    topSplitter_->setObjectName("TopSplitter");

    // �м����� + �ײ� ͨ����һ����ֱ QSplitter �ָ�  
    centerAndBottomSplitter_ = new QSplitter(Qt::Vertical, mainSplitter_);
    centerAndBottomSplitter_->setObjectName("CenterAndBottomSplitter");

    // centerHorizontalSplitter_�����ҷָ�����Left / Center / Right��  
    centerHorizontalSplitter_ = new QSplitter(Qt::Horizontal, centerAndBottomSplitter_);
    centerHorizontalSplitter_->setObjectName("CenterHorizontalSplitter");

    // ���Splitter  
    leftSplitter_ = new QSplitter(Qt::Vertical, centerHorizontalSplitter_);
    leftSplitter_->setObjectName("LeftSplitter");

    // centerWidget_ �� centerHorizontalSplitter_ �о��з���  
    center_frame = new QFrame(centerHorizontalSplitter_);
    center_frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto center_frame_layout = new QHBoxLayout(center_frame);
    center_frame_layout->setSpacing(0);
    center_frame_layout->setMargin(0);

    // �Ҳ�Splitter  
    rightSplitter_ = new QSplitter(Qt::Vertical, centerHorizontalSplitter_);
    rightSplitter_->setObjectName("RightSplitter");

    // �ײ�Splitter  
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

    // ��װ�ú󣬰� mainSplitter_ ��ΪΨһWidget�ŵ��� QFrame �Ĳ�����  
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
        // �Ƴ��ɵ����Ĳ���  
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

    // �� dockWidget ��ʼ parent ����Ϊ������  
    dockWidget->setParent(this);

    // ���� area ѡ��ŵ���Ӧ splitter  
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
        // Ĭ�Ϸ��м䣨���Զ����߼���  
        //if (centerWidget_) {
        //    // Ҳ���԰�������һ������Tab������ָ��  
        //    addDockToSplitter(leftSplitter_, dockWidget);
        //}
        //else {
        //    addDockToSplitter(leftSplitter_, dockWidget);
        //}
        break;
    }

    // ���� dockWidget �ĳ�ʼ container (�ڴ�ʾ���пɲ��������¼)  
    dockWidget->setDockContainer(this);
}

