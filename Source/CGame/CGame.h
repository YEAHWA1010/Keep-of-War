// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CGame.h"

#if PLATFORM_WINDOWS
  #ifdef UpdateResource
	#undef UpdateResource
  #endif
  #ifdef min
	#undef min
  #endif
  #ifdef max
	#undef max
  #endif
  #ifdef GetObject
	#undef GetObject
  #endif
  #ifdef DrawText
	#undef DrawText
  #endif
#endif