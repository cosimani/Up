#include "instagram.h"
#include "ui_instagram.h"

Instagram::Instagram(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Instagram)
{
    ui->setupUi(this);
}

Instagram::~Instagram()
{
    delete ui;
}
