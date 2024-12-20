#pragma once  

#include <QFrame>  
#include <QLabel>  
#include <QMouseEvent>  
#include <QVBoxLayout>  
#include <QHBoxLayout>  
#include <QPushButton>  
#include <QPoint>  
#include <QDialog>  

class MyDockContainer;  // ǰ������  

// ���ڱ�ʾͣ��/����״̬  
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

    // ���������������൱�� QDockWidget::setWidget��  
    void setContentWidget(QWidget *widget);

    // ���ÿɷ񸡶���ģ��QDockWidget::setFeatures��  
    void setFloating(bool floating);

    DockState dockState() const { return m_state; }

signals:
    void beginDrag(MyDockWidget *widget);
    void endDrag(MyDockWidget *widget);

protected:
    // ��ק�¼�  
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    // ˫������������/��ԭ  
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

    // ����ʱʹ�õĶԻ��򣨻��ޱ߿��壩��ʢ���Լ�  
    QDialog *m_floatingWindow = nullptr;
};