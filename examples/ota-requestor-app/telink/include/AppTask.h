/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#pragma once

#include "AppEvent.h"
#include "LEDWidget.h"
#include "PWMDevice.h"

#include <zephyr/drivers/gpio.h>

#include <platform/CHIPDeviceLayer.h>

#include <cstdint>

struct k_timer;
struct Identify;

class AppTask
{
public:
    CHIP_ERROR StartApp(void);

    enum Action_t : uint8_t
    {
        ON_ACTION = 0,
        OFF_ACTION,
        LEVEL_ACTION,

        INVALID_ACTION
    };

    void PostLightingActionRequest(Action_t aAction);
    void PostEvent(AppEvent * event);
    static void IdentifyEffectHandler(EmberAfIdentifyEffectIdentifier aEffect);

private:
    friend AppTask & GetAppTask(void);
    CHIP_ERROR Init(void);

    static void ActionInitiated(AppTask::Action_t aAction, int32_t aActor);
    static void ActionCompleted(AppTask::Action_t aAction, int32_t aActor);
    static void ActionIdentifyStateUpdateHandler(k_timer * timer);

    void DispatchEvent(AppEvent * event);

    static void UpdateStatusLED(void);
    static void LEDStateUpdateHandler(LEDWidget * ledWidget);
    static void FactoryResetButtonEventHandler(void);
    static void StartThreadButtonEventHandler(void);
    static void StartBleAdvButtonEventHandler(void);

    static void ChipEventHandler(const chip::DeviceLayer::ChipDeviceEvent * event, intptr_t arg);

    static void FactoryResetTimerTimeoutCallback(k_timer * timer);

    static void FactoryResetTimerEventHandler(AppEvent * aEvent);
    static void FactoryResetHandler(AppEvent * aEvent);
    static void StartThreadHandler(AppEvent * aEvent);
    static void StartBleAdvHandler(AppEvent * aEvent);
    static void UpdateLedStateEventHandler(AppEvent * aEvent);
    static void UpdateIdentifyStateEventHandler(AppEvent * aEvent);

    static void InitButtons(void);

    static void ThreadProvisioningHandler(const chip::DeviceLayer::ChipDeviceEvent * event, intptr_t arg);

    static AppTask sAppTask;
    PWMDevice mPwmIdentifyLed;
};

inline AppTask & GetAppTask(void)
{
    return AppTask::sAppTask;
}
