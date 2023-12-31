#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef QT_NO_DEBUG
    #define dbg(str, param)
#else
#define dbg(str, param)  qDebug()<<str<<param<<"\n"
#endif

/* 城市ID */
#define CITY_ID_BEIJING      "101220101"       //北京
#define CITY_ID_BAOAN        "101280605"       //深圳宝安区
#define CITY_ID_GUANGZHOU    "101280101"       //广东省广州市

#define weather_api          "https://devapi.qweather.com/v7/weather/now?location="

#endif // GLOBAL_H
