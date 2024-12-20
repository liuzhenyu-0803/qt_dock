#pragma once

#include <QtWidgets/QWidget>
#include "ui_Widget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::WidgetClass ui;
};
