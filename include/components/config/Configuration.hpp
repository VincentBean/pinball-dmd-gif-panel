#pragma once

#include "components/gif/GifLoader.hpp"

struct Config
{
  bool clockEnabled;
  int brightness;
  load_strategy_t loadStrategy;
  String timeZone;
};

void loadSettings();
void saveSettings();