#pragma once  

#include <QFrame>  
#include <QLabel>  
#include <QMouseEvent>  
#include <QVBoxLayout>  
#include <QHBoxLayout>  
#include <QPushButton>  
#include <QPoint>  
#include <QDialog>  

class MyDockContainer;  // 前置声明  

// 用于表示停靠/浮动状态  
enum class DockState {
    Docked,
    Floating
};

class MyDockWidget : public QFrame
{
    Q_OBJECT
public:
    explicit MyDockWidget(const QString &title, QWidget *parent = nullptr);
    void setDockContainer(MyDockContainer *container);

    // 设置主内容区（相当于 QDockWidget::setWidget）  
    void setContentWidget(QWidget *widget);

    // 设置可否浮动（模拟QDockWidget::setFeatures）  
    void setFloating(bool floating);

    DockState dockState() const { return m_state; }

signals:
    void beginDrag(MyDockWidget *widget);
    void endDrag(MyDockWidget *widget);

protected:
    // 拖拽事件  
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    // 双击标题栏浮动/还原  
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private slots:
    void onCloseButtonClicked();

private:
    void switchToFloatingMode();
    void switchToDockedMode();

    MyDockContainer *m_container = nullptr;
    QLabel *m_titleLabel = nullptr;
    QWidget *m_contentWidget = nullptr;
    QPoint m_dragStartPos;
    bool m_isDragging = false;

    DockState m_state = DockState::Docked;

    // 浮动时使用的对话框（或无边框窗体）来盛放自己  
    QDialog *m_floatingWindow = nullptr;
};