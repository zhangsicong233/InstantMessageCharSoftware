#pragma once

#include <boost/property_tree/ptree_fwd.hpp>

#include "const.h"

struct SectionInfo {
  SectionInfo() {}

  SectionInfo(const SectionInfo& other) {
    _section_datas = other._section_datas;
  }

  SectionInfo& operator=(const SectionInfo& other) {
    if (this != &other) {
      _section_datas = other._section_datas;
    }

    return *this;
  }

  std::string operator[](const std::string& key) {
    if (_section_datas.find(key) == _section_datas.end()) {
      return "";
    }

    return _section_datas[key];
  }

  ~SectionInfo() { _section_datas.clear(); }

  std::map<std::string, std::string> _section_datas;
};

class ConfigMgr {
 public:
  ConfigMgr();

  ConfigMgr(const ConfigMgr& other) { _config_map = other._config_map; }

  ConfigMgr& operator=(const ConfigMgr& other) {
    if (this != &other) {
      _config_map = other._config_map;
    }

    return *this;
  }

  SectionInfo operator[](const std::string& section) {
    if (_config_map.find(section) == _config_map.end()) {
      return SectionInfo();
    }

    return _config_map[section];
  }

  ~ConfigMgr();

 private:
  std::map<std::string, SectionInfo> _config_map;
};