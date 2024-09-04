#ifndef BONEGUI_H
#define BONEGUI_H

#include <QWidget>
#include <QLabel>

class boneGUI : public QWidget
{
    Q_OBJECT
public:
    explicit boneGUI(QWidget *parent = nullptr);

public slots:
    void updateTime();
    void updateRam();

private:
    QSize sizeHint() const;
    QLabel *tsLabel;
    QLabel *utLabel;
    QLabel *ram_free;
    QLabel *ram_usage;

signals:
};

#endif // BONEGUI_H
