/*
 * Copyright 2016 The CyanogenMod Project
 * Copyright 2017 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _BDROID_BUILDCFG_H
#define _BDROID_BUILDCFG_H

#pragma push_macro("PROPERTY_VALUE_MAX")

#include <cutils/properties.h>
#include <string.h>

inline const char* BtmGetDefaultName()
{
	char product_name[PROPERTY_VALUE_MAX];
	property_get("ro.product.name", product_name, "");

	if (strstr(product_name, "h1"))
		return "LG G5";
	if (strstr(product_name, "elsa"))
		return "LG V20";
	if (strstr(product_name, "lucye"))
		return "LG G6";

	return "";
}

#define BTM_DEF_LOCAL_NAME BtmGetDefaultName()
#define BTA_DISABLE_DELAY 1000 /* in milliseconds */

#pragma pop_macro("PROPERTY_VALUE_MAX")

#endif
