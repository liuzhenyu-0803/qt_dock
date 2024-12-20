#include "MyDockWidget.h"  
#include "MyDockContainer.h"  
#include <QLayout>  
#include <QDebug>  
#include <QStyle>  

MyDockWidget::MyDockWidget(const QString &title, QWidget *parent)
    : QFrame(parent)
{
    setObjectName("MyDockWidget");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    // 标题栏  
    QFrame *titleBar = new QFrame(this);
    titleBar->setObjectName("DockWidgetTitleBar");
    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setMargin(2);
    titleLayout->setSpacing(5);

    titleBar->setStyleSheet("QFrame#DockWidgetTitleBar { background-color: rgb(200, 200, 200); border: none; }");

    m_titleLabel = new QLabel(title, titleBar);
    titleLayout->addWidget(m_titleLabel);

    titleLayout->addStretch();

    // 示例：添加一个关闭按钮  
    //QPushButton *closeBtn = new QPushButton("×", titleBar);
    //closeBtn->setFixedSize(20, 20);
    //titleLayout->addWidget(closeBtn);
    //connect(closeBtn, &QPushButton::clicked, this, &MyDockWidget::onCloseButtonClicked);

    mainLayout->addWidget(titleBar);

    // 内容区  
    m_contentWidget = new QFrame(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(m_contentWidget);
    contentLayout->setMargin(4);
    contentLayout->setSpacing(4);
    m_contentWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    m_contentWidget->setObjectName("MyDockWidget");
    m_contentWidget->setStyleSheet("QFrame#MyDockWidget { background-color: cyan; border: none; }");

    mainLayout->addWidget(m_contentWidget);
}

void MyDockWidget::setDockContainer(MyDockContainer *container)
{
    m_container = container;
}

void MyDockWidget::setContentWidget(QWidget *widget)
{
    if (!m_contentWidget) return;
    QLayout *layout = m_contentWidget->layout();
    if (!layout) return;

    // 移除原有子Widget  
    while (QLayoutItem *item = layout->takeAt(0)) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
    layout->addWidget(widget);
}

void MyDockWidget::onCloseButtonClicked()
{
    // 模拟关闭  
    hide();
    // 也可在这里 emit 某个信号让外部管理真正的删除  
}

void MyDockWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 简化：若点击在标题栏范围内才允许拖拽  
        if (event->pos().y() < 30) {
            m_dragStartPos = event->pos();
            m_isDragging = true;
            emit beginDrag(this);
        }
    }
    QFrame::mousePressEvent(event);
}

void MyDockWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging && (event->buttons() & Qt::LeftButton)) {
        QPoint diff = event->pos() - m_dragStartPos;
        if (diff.manhattanLength() > 5) {
            // 如果还在 dock 状态下，切换到 floating  
            if (m_state == DockState::Docked) {
                switchToFloatingMode();
            }
            // 更新浮动窗体位置  
            if (m_floatingWindow) {
                QPoint globalPos = mapToGlobal(event->pos());
                m_floatingWindow->move(globalPos - m_dragStartPos);
            }
        }
    }
    QFrame::mouseMoveEvent(event);
}

void MyDockWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isDragging && event->button() == Qt::LeftButton) {
        m_isDragging = false;
        emit endDrag(this);

        // 在这里你可以判断鼠标释放的位置，决定是否需要让这个DockWidget重新回到某个区域  
        // 示例：若想回到container则转为Docked，否则保持Floating  
        // 实际项目中一般需要全局鼠标坐标来判断是否在容器范围内  
        if (m_floatingWindow) {
            QPoint globalPos = QCursor::pos(); // 当前鼠标位置  
            // 判断逻辑(简化)  
            // 如果此位置在 m_container 范围内，则 switchToDockedMode()  
            if (m_container) {
                // map to container coordinate  
                QPoint containerPos = m_container->mapFromGlobal(globalPos);
                if (m_container->rect().contains(containerPos)) {
                    // 回到Docked  
                    switchToDockedMode();
                }
            }
        }
    }
    QFrame::mouseReleaseEvent(event);
}

void MyDockWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    // 双击标题栏可以切换 docked/floating  
    if (event->pos().y() < 30) {
        if (m_state == DockState::Docked) {
            switchToFloatingMode();
        }
        else {
            switchToDockedMode();
        }
    }
    QFrame::mouseDoubleClickEvent(event);
}

void MyDockWidget::setFloating(bool floating)
{
    if (floating && m_state == DockState::Docked) {
        switchToFloatingMode();
    }
    else if (!floating && m_state == DockState::Floating) {
        switchToDockedMode();
    }
}

void MyDockWidget::switchToFloatingMode()
{
    if (m_state == DockState::Floating) return;

    m_state = DockState::Floating;
    // 从原父对象里移除  
    if (parentWidget()) {
        setParent(nullptr);
    }

    // 创建浮动窗口，将自己塞进去  
    m_floatingWindow = new QDialog(nullptr, Qt::FramelessWindowHint);
    m_floatingWindow->setAttribute(Qt::WA_DeleteOnClose);
    // 用一个布局包裹自己  
    QVBoxLayout *floatLayout = new QVBoxLayout(m_floatingWindow);
    floatLayout->setMargin(0);
    floatLayout->setSpacing(0);
    floatLayout->addWidget(this);

    m_floatingWindow->resize(size());
    m_floatingWindow->show();
    show();
}

void MyDockWidget::switchToDockedMode()
{
    if (m_state == DockState::Docked) return;

    m_state = DockState::Docked;
    // 关闭并销毁浮动窗口  
    if (m_floatingWindow) {
        m_floatingWindow->close();
        m_floatingWindow = nullptr;
    }
    // 回到 container 父对象  
    if (m_container) {
        setParent(m_container);
        show();
        // 需要由外部决定具体插入哪里，这里仅简单放回  
        // 也可 emit 一个信号让 MyDockContainer 重新 addDockWidget  
        // 这里为了示例，默认加回 LeftDockWidgetArea  
        m_container->addDockWidget(Qt::LeftDockWidgetArea, this);
    }
}