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

    // ������  
    QFrame *titleBar = new QFrame(this);
    titleBar->setObjectName("DockWidgetTitleBar");
    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setMargin(2);
    titleLayout->setSpacing(5);

    titleBar->setStyleSheet("QFrame#DockWidgetTitleBar { background-color: rgb(200, 200, 200); border: none; }");

    m_titleLabel = new QLabel(title, titleBar);
    titleLayout->addWidget(m_titleLabel);

    titleLayout->addStretch();

    // ʾ�������һ���رհ�ť  
    //QPushButton *closeBtn = new QPushButton("��", titleBar);
    //closeBtn->setFixedSize(20, 20);
    //titleLayout->addWidget(closeBtn);
    //connect(closeBtn, &QPushButton::clicked, this, &MyDockWidget::onCloseButtonClicked);

    mainLayout->addWidget(titleBar);

    // ������  
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

    // �Ƴ�ԭ����Widget  
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
    // ģ��ر�  
    hide();
    // Ҳ�������� emit ĳ���ź����ⲿ����������ɾ��  
}

void MyDockWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // �򻯣�������ڱ�������Χ�ڲ�������ק  
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
            // ������� dock ״̬�£��л��� floating  
            if (m_state == DockState::Docked) {
                switchToFloatingMode();
            }
            // ���¸�������λ��  
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

        // ������������ж�����ͷŵ�λ�ã������Ƿ���Ҫ�����DockWidget���»ص�ĳ������  
        // ʾ��������ص�container��תΪDocked�����򱣳�Floating  
        // ʵ����Ŀ��һ����Ҫȫ������������ж��Ƿ���������Χ��  
        if (m_floatingWindow) {
            QPoint globalPos = QCursor::pos(); // ��ǰ���λ��  
            // �ж��߼�(��)  
            // �����λ���� m_container ��Χ�ڣ��� switchToDockedMode()  
            if (m_container) {
                // map to container coordinate  
                QPoint containerPos = m_container->mapFromGlobal(globalPos);
                if (m_container->rect().contains(containerPos)) {
                    // �ص�Docked  
                    switchToDockedMode();
                }
            }
        }
    }
    QFrame::mouseReleaseEvent(event);
}

void MyDockWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    // ˫�������������л� docked/floating  
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
    // ��ԭ���������Ƴ�  
    if (parentWidget()) {
        setParent(nullptr);
    }

    // �����������ڣ����Լ�����ȥ  
    m_floatingWindow = new QDialog(nullptr, Qt::FramelessWindowHint);
    m_floatingWindow->setAttribute(Qt::WA_DeleteOnClose);
    // ��һ�����ְ����Լ�  
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
    // �رղ����ٸ�������  
    if (m_floatingWindow) {
        m_floatingWindow->close();
        m_floatingWindow = nullptr;
    }
    // �ص� container ������  
    if (m_container) {
        setParent(m_container);
        show();
        // ��Ҫ���ⲿ��������������������򵥷Ż�  
        // Ҳ�� emit һ���ź��� MyDockContainer ���� addDockWidget  
        // ����Ϊ��ʾ����Ĭ�ϼӻ� LeftDockWidgetArea  
        m_container->addDockWidget(Qt::LeftDockWidgetArea, this);
    }
}