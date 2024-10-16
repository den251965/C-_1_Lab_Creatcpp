#include "setting.h"

#include <fstream>

std::string Setting::fileSetting_{};
const std::string Setting::seperator_ = " = ";
const std::string Setting::invalid_setting_ = "-1";

const std::map<Setting::Param, std::string> Setting::paramToKeyString {
    {   CountOperators   ,    "CountOperators"    },
    {     Operator       ,       "Operator"       },
};

const std::map<std::string, Setting::Param> Setting::stringToKeyParam {
    {    "CountOperators"    ,   Setting::CountOperators   },
    {       "Operator"       ,     Setting::Operator       },
};

const std::map<Setting::Param, std::string> Setting::defaultConfigValues {
    {   CountOperators   ,    "1"    },
    {     Operator       ,    "1"    },
};

std::map<Setting::Param, std::string> Setting::CurrentConfigValues {
    {   CountOperators   ,    invalid_setting_    },
    {     Operator       ,    invalid_setting_    },
};

Setting::Setting(std::string fileSetting) {
    fileSetting_ = !fileSetting.empty()? fileSetting : "conf.cfg";
    bool correct = true;
    if (isOpen())
    {
        read_setting();
        for (auto& t : CurrentConfigValues) {
            correct = t.second == invalid_setting_ ? false : true;
            if (!correct) {
                break;
            }
        }
    } else {
        correct = false;
    }
    if (!correct) {
        write_setting();
        read_setting();
    }
}

bool Setting::isOpen() {
    if (FILE *file = fopen(fileSetting_.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

void Setting::read_setting() {
    std::ifstream read(fileSetting_);
    std::string item, key;
    while (std::getline(read, item)) {
        auto pos = item.find(seperator_);
        if (pos != item.npos) {
            key = item.substr(0, pos);
            if (stringToKeyParam.count(key) != 0) {
                auto k = stringToKeyParam.at(key);
                CurrentConfigValues[k] = item.substr(pos + seperator_.length(), item.length());
            }
        }
    }
}

void Setting::write_setting() {
    std::ofstream write(fileSetting_);
    std::string val;
    for (auto& item : CurrentConfigValues) {
        val = item.second != invalid_setting_ ? item.second : defaultConfigValues.at(item.first);
        write << paramToKeyString.at(item.first) << seperator_ << val << "\n";
    }
}

std::string Setting::value(const Setting::Param &key) {
    if (CurrentConfigValues.count(key) != 0) {
        return CurrentConfigValues.at(key);
    }
    return "";
}

void Setting::setValue(const Setting::Param &key, const std::string val) {
    if (paramToKeyString.count(key) == 0 || val == invalid_setting_) {
        return;
    }
    CurrentConfigValues[key] = val;
    write_setting();
}


