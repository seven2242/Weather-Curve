#include "widget.h"
#include "ui_widget.h"

#include <QIcon>
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle(tr("天气"));
    setWindowFlags(Qt::WindowStaysOnBottomHint| Qt::FramelessWindowHint);//去掉标题栏

    //setAttribute(Qt::WA_TranslucentBackground, true);//设置窗口背景透明
    setWindowOpacity(0.65);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);

    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),20,20);
    setMask(bmp);

    netThread = new NET_THREAD(this);

    timShowMessage = new QTimer(this);

    timShowMessage->start(10000);

    mSysTrayIcon = new QSystemTrayIcon(this);
    //新建托盘要显示的icon
    QIcon icon = QIcon("://weather_icon.ico");
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip(QObject::tr("Weather"));
    //给QSystemTrayIcon添加槽函数
    //connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

    //建立托盘操作的菜单

    //在系统托盘显示此对象


    /* 关联槽函数 */
    connect(ui->label_refresh, SIGNAL(mouse_clicked()), netThread->weather, SLOT(slot_refresh_manually()));
    connect(netThread->weather, SIGNAL(signal_emit_weather_info(WEATHER_INFO *)), this, SLOT(slot_get_weather_info(WEATHER_INFO *)));
    connect(ui->label_local_city, SIGNAL(mouse_clicked()), this, SLOT(slot_change_local_city()));
    connect(timShowMessage, SIGNAL(timeout()), this, SLOT(slot_show_message()));

    createActions();
    createMenu();

    netThread->startThread();
    mSysTrayIcon->show();
}

Widget::~Widget()
{

    delete ui;
}


/**
 * @brief Widget::slot_read_network_data
 * @param reply
 */
/*
void Widget::on_pushButton_clicked()
{
    //隐藏主窗口
    this->hide();

    //新建QSystemTrayIcon对象
    mSysTrayIcon = new QSystemTrayIcon(this);
    //新建托盘要显示的icon
    QIcon icon = QIcon("weather_icon.ico");
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip(QObject::tr("测试系统托盘图标"));
    //给QSystemTrayIcon添加槽函数
    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

    //建立托盘操作的菜单
    //createActions();
    //createMenu();
    //在系统托盘显示此对象
    mSysTrayIcon->show();
}

void Widget::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
        mSysTrayIcon->showMessage(QObject::tr("Message Title"),
                                  QObject::tr("欢迎使用此程序"),
                                  QSystemTrayIcon::Information,
                                  1000);
        break;
    case QSystemTrayIcon::DoubleClick:
        this->show();
        break;
    default:
        break;
    }
}*/

void Widget::createActions()
{
    mShowMainAction = new QAction(QObject::tr("显示主界面"),this);
    connect(mShowMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));

    mExitAppAction = new QAction(QObject::tr("退出"),this);
    connect(mExitAppAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));

}

void Widget::createMenu()
{
    mMenu = new QMenu(this);
    mMenu->addAction(mShowMainAction);

    mMenu->addSeparator();

    mMenu->addAction(mExitAppAction);

    mSysTrayIcon->setContextMenu(mMenu);
}

void Widget::on_showMainAction()
{
    this->show();
}

void Widget::on_exitAppAction()
{
    exit(0);
}


void Widget::slot_read_network_data(QNetworkReply * reply)
{
    QByteArray get_network = reply->readAll();
    dbg("reply finished", 0);

}

/**
 * @brief 获取温度信息-槽函数
 * @param info
 */
void Widget::slot_get_weather_info(WEATHER_INFO * info)
{
    dbg("get weather info", 0);

    QString pixmap;

    showMessageList.clear();

    ui->label_city_name->setText(info->cityName);
    ui->label_real_time_temp->setText(info->realTime.temp);    
    ui->label_temp_1->setText(info->forecast[0].temp);
    ui->label_weather_type_1->setText(info->forecast[0].weatherType);

    showMessageList.append(info->forecast[0].dataAndWeek);
    showMessageList.append(tr("能见度：") + info->realTime.airQuality+tr("KM"));
    showMessageList.append(info->forecast[0].windDirAndPower);
    showMessageList.append(info->forecast[0].notice);

    if(info->forecast[0].weatherType.contains("多云")||info->forecast[0].weatherType.contains("阴"))
    {
        pixmap = ":/icons/weather_icons/yun.png";
    }
    else if(info->forecast[0].weatherType.contains("晴"))
    {
        pixmap = ":/icons/weather_icons/qing.png";
    }
    else if(info->forecast[0].weatherType.contains("雨"))
    {
        pixmap = ":/icons/weather_icons/yu.png";
    }



    ui->label_weather_icon->setPixmap(QPixmap(pixmap));
}

void Widget::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton)
        last_mouse_position_=event->globalPos();
}

void Widget::mouseMoveEvent(QMouseEvent *event){
    if(!event->buttons().testFlag(Qt::LeftButton))return;
    const QPoint position=pos()+event->globalPos()-last_mouse_position_;
    move(position.x(),position.y());
    last_mouse_position_=event->globalPos();
}

void Widget::slot_change_local_city()
{
    dbg("clicked", 0);

    dialog_change_city = new Dialog_Change_City;
    dialog_change_city->show();

    connect(dialog_change_city, SIGNAL(signal_emit_city_name(QString)), netThread->weather, SLOT(slot_change_local_city(QString)));
}

void Widget::slot_show_message()
{
    static quint8 index = 0;

    if(showMessageList.isEmpty() == true)
    {
        return;
    }

    ui->label_show_message->setText(showMessageList.at(index));

    index ++;

    if(index >= showMessageList.size())
    {
        index = 0;
    }
}
