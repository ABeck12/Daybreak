#pragma once

// Fix for ImGui deprecation warnings
#define _CRT_SECURE_NO_WARNINGS

#ifndef NOMINMAX
// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#define NOMINMAX
#endif

#include <memory>
#include <iostream>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

#include <glm/glm.hpp>
#include <Daybreak/Math/GlmToString.h>

#ifdef DB_PLATFORM_WINDOWS
#include <Windows.h>
#endif


#include "Daybreak/Core/Base.h"
#include "Daybreak/Core/Log.h"