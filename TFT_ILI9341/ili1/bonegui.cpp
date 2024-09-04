#include "bonegui.h"
#include "sysinfo.h"

#include <QFont>
#include <QLabel>
#include <QGroupBox>
#include <QBoxLayout>
#include <QLayout>
#include <QFont>
#include <QDebug>
#include <QSize>
#include <QTimer>

boneGUI::boneGUI(QWidget *parent)
    : QWidget{parent}
{

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *r1 = new QHBoxLayout;

    QFont font;
    font.setPointSize(8);

    std::vector<QString> osinfo = getOsInfo();

    QGroupBox *os = new QGroupBox("OS");
    // os->setFixedSize(100, 60);
    os->setFixedHeight(65);
    os->setAlignment(Qt::AlignCenter);
    QLabel *osLabel = new QLabel(osinfo[2]);
    osLabel->setWordWrap(true);
    osLabel->setAlignment(Qt::AlignCenter);
    osLabel->setFont(font);
    QVBoxLayout *osLayout = new QVBoxLayout;
    osLayout->addWidget(osLabel);
    os->setLayout(osLayout);


    QGroupBox *arch = new QGroupBox("Arch");
    arch->setFixedHeight(65);
    arch->setAlignment(Qt::AlignCenter);
    QLabel *archLabel = new QLabel(osinfo[0]);
    archLabel->setAlignment(Qt::AlignCenter);
    archLabel->setFont(font);
    QVBoxLayout *archLayout = new QVBoxLayout;
    archLayout->addWidget(archLabel);
    arch->setLayout(archLayout);



    QGroupBox *kern = new QGroupBox("Kernel");
    kern->setFixedHeight(65);
    kern->setAlignment(Qt::AlignCenter);
    QLabel *kernLabel = new QLabel(osinfo[1]);
    kernLabel->setAlignment(Qt::AlignCenter);
    kernLabel->setFont(font);
    QVBoxLayout *kernLayout = new QVBoxLayout;
    kernLayout->addWidget(kernLabel);
    kern->setLayout(kernLayout);




    r1->addWidget(os);
    r1->addWidget(arch);
    r1->addWidget(kern);

    QHBoxLayout *r2 = new QHBoxLayout;

    QGroupBox *cpu = new QGroupBox("CPU");
    cpu->setAlignment(Qt::AlignCenter);
    QLabel *cpu_name = new QLabel(getCpuName());
    cpu_name->setFont(font);
    cpu_name->setWordWrap(true);
    QLabel *cpu_serial = new QLabel(getSeiral());
    cpu_serial->setFont(font);
    cpu_serial->setWordWrap(true);
    // QLabel *cpu_mhz = new QLabel(getCpuMHz());
    // cpu_mhz->setFont(font);

    QVBoxLayout *cpu_box = new QVBoxLayout;

    cpu_box->addWidget(cpu_name);
    cpu_box->addWidget(cpu_serial);
    // cpu_box->addWidget(cpu_mhz);

    cpu->setLayout(cpu_box);

    std::vector<QString> mem = getMemory();

    QGroupBox *ram = new QGroupBox("RAM");
    ram->setAlignment(Qt::AlignCenter);
    QLabel *ram_total = new QLabel(mem[0]);
    ram_total->setFont(font);

    ram_free = new QLabel(mem[1]);
    ram_free->setFont(font);

    ram_usage = new QLabel(mem[2]);
    ram_usage->setFont(font);

    QVBoxLayout *ram_box = new QVBoxLayout;

    ram_box->addWidget(ram_total);
    ram_box->addWidget(ram_free);
    ram_box->addWidget(ram_usage);

    ram->setLayout(ram_box);



    QVBoxLayout *time_col = new QVBoxLayout;

    QGroupBox *ts = new QGroupBox("Time");
    ts->setAlignment(Qt::AlignCenter);

    tsLabel = new QLabel(getTimestamp());
    tsLabel->setAlignment(Qt::AlignCenter);
    tsLabel->setFont(font);
    QVBoxLayout *tsLayout = new QVBoxLayout;
    tsLayout->addWidget(tsLabel);
    ts->setLayout(tsLayout);

    QGroupBox *ut = new QGroupBox("UpTime");
    ut->setAlignment(Qt::AlignCenter);

    utLabel = new QLabel(getUptime());
    utLabel->setAlignment(Qt::AlignCenter);
    utLabel->setFont(font);
    QVBoxLayout *utLayout = new QVBoxLayout;
    utLayout->addWidget(utLabel);
    ut->setLayout(utLayout);



    time_col->addWidget(ts);
    time_col->addWidget(ut);

    r2->addWidget(cpu);
    r2->addWidget(ram);
    r2->addLayout(time_col);

    mainLayout->addLayout(r1);
    mainLayout->addLayout(r2);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateRam()));
    timer->start(1000);

}

void boneGUI::updateTime()
{
    QString ts = getTimestamp();
    QString ut = getUptime();
    tsLabel->setText(ts);
    utLabel->setText(ut);
}

void boneGUI::updateRam()
{
    std::vector<QString> mem = getMemory();
    ram_free->setText(mem[1]);
    ram_usage->setText(mem[2]);
}

QSize boneGUI::sizeHint() const
{
    return QSize(320, 240);
}
