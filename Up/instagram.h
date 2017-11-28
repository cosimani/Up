#ifndef INSTAGRAM_H
#define INSTAGRAM_H

#include <QWidget>

namespace Ui {
class Instagram;
}

class Instagram : public QWidget
{
    Q_OBJECT

public:
    explicit Instagram(QWidget *parent = 0);
    ~Instagram();

private:
    Ui::Instagram *ui;
};

#endif // INSTAGRAM_H
