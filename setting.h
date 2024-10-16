#pragma once
#ifndef SETTING_H
#define SETTING_H

/*  Файл настроек
 *  читаем и пишем настройкиб создаем при необходимости
 *  CountOperators - количество операции
 *  Operator - сами операции
 */

#include <string>
#include <map>

class Setting
{
public:

    enum Param
    {
        CountOperators, // количество операции
        Operator, //сами операции
    };

    explicit Setting(std::string fileSetting = "");

    static std::string value(const Param& key);
    static void setValue(const Param& key, const std::string val);

private:

    static std::string fileSetting_;
    static const std::string seperator_;
    static const std::string invalid_setting_;
    static const std::map<Param, std::string> paramToKeyString;//ключи настроек Enum то string
    static const std::map<std::string, Param> stringToKeyParam;//string в ключи настроек
    static const std::map<Param, std::string> defaultConfigValues;//дефолтные
    static std::map<Param, std::string> CurrentConfigValues;//текущие настройки
    bool isOpen();
    void read_setting();
    static void write_setting();
};


#endif // SETTING_H
