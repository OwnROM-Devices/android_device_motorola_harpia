/*
   Copyright (c) 2016, The CyanogenMod Project. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"
#include <sys/sysinfo.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

bool is2GB()
{
    struct sysinfo sys;
    sysinfo(&sys);
    return sys.totalram > 1024ull * 1024 * 1024;
}

void vendor_load_properties()
{
    const char *customerid = NULL;
    bool msim = false;

    std::string platform = property_get("ro.board.platform");
    if (platform != ANDROID_TARGET)
        return;

    // Warning-less way of sprintf(var, ""); 
    std::string sku = property_get("ro.boot.hardware.sku"); 

    std::string dualsim = property_get("ro.boot.dualsim");
    if (dualsim, "true") { 
        property_set("persist.radio.force_get_pref", "1");
        property_set("persist.radio.multisim.config", "dsds");
        property_set("ro.hw.dualsim", "true");
        msim = true;
    } 

    std::string bootdevice = property_get("ro.boot.device"); 
        property_set("ro.hw.device", "bootdevice");

    std::string radio = property_get("ro.boot.radio"); 
        property_set("ro.hw.radio", "radio");

    if (is2GB()) {
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "192m");
        property_set("dalvik.vm.heapsize", "512m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapminfree", "512k");
        property_set("dalvik.vm.heapmaxfree", "8m");
    } else {
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "96m");
        property_set("dalvik.vm.heapsize", "256m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapminfree", "2m");
        property_set("dalvik.vm.heapmaxfree", "8m");
    }

    property_set("ro.telephony.default_network", "10");

    if (sku== "XT1600") {
        /* XT1600 */
        customerid = "retail";
    } else if (sku == "XT1601") { 
        /* XT1601 */
        customerid = "retail";
        property_set("persist.radio.process_sups_ind", "1");
        if (msim) {
            property_set("persist.radio.pb.max.match", "8");
            property_set("persist.radio.pb.min.match", "8");
        }
    } else if (sku == "XT1602") { 
        /* XT1602 */
    } else if (sku == "XT1603") { 
        /* XT1603 */
        customerid = "retail";
        property_set("persist.radio.pb.max.match", "10");
        property_set("persist.radio.pb.min.match", "7");
    } else if (sku == "XT1604") { 
        /* XT1604 - HAS NFC! */
    } else if (sku == "XT1607") { 
        /* XT1607 */
    } else if (sku == "XT1609") { 
        /* XT1609 */
    }

    property_override("ro.product.device", "harpia");
    property_override("ro.build.product", "harpia");
    property_override("ro.build.description",
            "harpia-user 6.0.1 MPI24.241-15.3 3 release-keys");
    property_override("ro.build.fingerprint",
            "motorola/harpia/harpia:6.0.1/MPI24.241-15.3/3:user/release-keys");

    if (customerid) {
        property_set("ro.mot.build.customerid", customerid);
    }
    }