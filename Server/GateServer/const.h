#pragma once

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>

#include "ConfigMgr.h"
#include "Singleton.hpp"

enum ErrorCodes {
  Success = 0,
  Error_Json = 1001,
  RPCFailed = 1002,
};

class ConfigMgr;
extern ConfigMgr gCfgMgr;