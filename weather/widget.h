#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonObject>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QUrl>
#include <QDebug>
#include <global.h>
#include <QTimer>
#include <QEvent>
#include <QSystemTrayIcon>
#include <QMenu>

#include <class_net_thread/net_thread.h>
#include <dialog_change_city/dialog_change_city.h>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QStringList showMessageList;

    QTimer * timShowMessage = nullptr;

    QSystemTrayIcon *mSysTrayIcon;

    QMenu *mMenu;
    QAction *mShowMainAction;
    QAction *mExitAppAction;

    /* 插入weak label */
    void pushListWeak();

    void pushListDate();

    void pushListWeatherType();

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void createActions();

    void createMenu();

private slots:

    /* 网络获取的信息槽函数 */
    void slot_read_network_data(QNetworkReply * reply);

    void slot_get_weather_info(WEATHER_INFO *);

    void slot_change_local_city();

    void slot_show_message();

    //void on_pushButton_clicked();

    //void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);

    void on_showMainAction();
    void on_exitAppAction();


signals:


private:
    Ui::Widget *ui;

    NET_THREAD * netThread = nullptr;

    Dialog_Change_City * dialog_change_city = nullptr;

    QPoint last_mouse_position_;


    //Systray *m_pTray;


protected:
    //void changeEvent(QEvent *event);
};
#endif // WIDGET_H
