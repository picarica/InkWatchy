#include "wifiTask.h"

std::mutex wifiTaskMutex;
bool isWifiTaskRunning;

WiFiMulti wifiMulti;
TaskHandle_t wifiTask;

uint8_t wifiConnectionTries = 0;
void (*wifiFunction)() = NULL;

void createWifiTask(uint8_t tries, void (*functionToRunAfterConnection)(), uint8_t wifiPriority)
{
    debugLog("Creating wifi task");
    wifiConnectionTries = tries;
    wifiFunction = functionToRunAfterConnection;
    if (isWifiTaskCheck() == false)
    {
        debugLog("xTaskCreate wifi");
        setBoolMutex(&wifiTaskMutex, &isWifiTaskRunning, true); // To make sure it's fast enough
        xTaskCreate(
            turnOnWifiTask,
            "wifiTask",
            TASK_STACK_WIFI,
            NULL,
            wifiPriority,
            &wifiTask);
    } else {
        debugLog("The task is already running? why?");
    }
}

void turnOnWifiTask(void *parameter)
{
    debugLog("Turning wifi on");
    setBoolMutex(&wifiTaskMutex, &isWifiTaskRunning, true);
    for (int i = 0; i < wifiConnectionTries; i++)
    {
        debugLog("Running wifi loop: " + String(i));
        // debugLog("isWifiTaskRunning: " + BOOL_STR(isWifiTaskCheck()));
        if (HARDWARE_POWER_SAVINGS == 1)
        {
            WiFi.setSleep(WIFI_PS_MAX_MODEM);
            debugLog("Setting sleep mode for wifi");
        }
        // We don't have NVS anymore
        // esp_wifi_set_storage(WIFI_STORAGE_RAM);
        // WiFi.persistent(false);
        // Won't work, fuck IDF for forcing that. We need a NVS partition
        // https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/partition-tables.html
        // 0x3000 bytes we need
        WiFi.mode(WIFI_STA);

        debugLog("Wifi sleep mode: " + String(WiFi.getSleep()));

        WiFi.setAutoConnect(true);
        WiFi.setAutoReconnect(true);
        initWifi();
        wifiMulti.run(WIFI_MULTI_SYNC_TIME);

        if (WiFi.status() == WL_CONNECTED)
        {
            break;
        }
        else
        {
            debugLog("Wifi failed to connect, retrying...");
            // turnOffWifi();
            turnOffWifiMinimal();
            delayTask(WIFI_MULTI_ERROR_TIME);
        }
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        delayTask(300);
        wifiFunction();
    }
    turnOffWifiMinimal();
    debugLog("Setting isWifiTaskRunning to false NOW");
    setBoolMutex(&wifiTaskMutex, &isWifiTaskRunning, false);
    vTaskDelete(NULL);
}

void turnOffWifiMinimal()
{
    if (WiFi.getMode() != WIFI_OFF)
    {
        if (WiFi.disconnect(true) == false)
        {
            debugLog("Failed to disconnect from wifi? turning it off anyway");
            if (WiFi.mode(WIFI_OFF) == false)
            {
                debugLog("Failed to force set mode of wifi, doing manual esp idf way");
                debugLog("esp_wifi_deinit: " + String(esp_err_to_name(esp_wifi_deinit())));
                debugLog("esp_wifi_stop: " + String(esp_err_to_name(esp_wifi_stop())));
            }
        }
    }
}

void turnOffWifi()
{
    debugLog("Turning wifi off");
    if (isWifiTaskCheck() == true)
    {
        // This is because task `arduino_events` has a queue and communicated with wifi task. Idk about this fix
        vTaskSuspend(wifiTask);
        delayTask(500);
        vTaskDelete(wifiTask);
        setBoolMutex(&wifiTaskMutex, &isWifiTaskRunning, false);
    }
    turnOffWifiMinimal();
}

bool initWifiMultiDone = false;
void initWifi()
{
    if (initWifiMultiDone == true)
    {
        return void();
    }
    if (WIFI_SSID1 != "" && WIFI_PASS1 != "")
    {
        debugLog("Adding wifi " + String(WIFI_SSID1));
        wifiMulti.addAP(WIFI_SSID1, WIFI_PASS1);
    }
    if (WIFI_SSID2 != "" && WIFI_PASS2 != "")
    {
        debugLog("Adding wifi " + String(WIFI_SSID2));
        wifiMulti.addAP(WIFI_SSID2, WIFI_PASS2);
    }
    if (WIFI_SSID3 != "" && WIFI_PASS3 != "")
    {
        debugLog("Adding wifi " + String(WIFI_SSID3));
        wifiMulti.addAP(WIFI_SSID3, WIFI_PASS3);
    }
    if (WIFI_SSID4 != "" && WIFI_PASS4 != "")
    {
        debugLog("Adding wifi " + String(WIFI_SSID4));
        wifiMulti.addAP(WIFI_SSID4, WIFI_PASS4);
    }
    if (WIFI_SSID5 != "" && WIFI_PASS5 != "")
    {
        debugLog("Adding wifi " + String(WIFI_SSID5));
        wifiMulti.addAP(WIFI_SSID5, WIFI_PASS5);
    }
    if (WIFI_SSID6 != "" && WIFI_PASS6 != "")
    {
        debugLog("Adding wifi " + String(WIFI_SSID6));
        wifiMulti.addAP(WIFI_SSID7, WIFI_PASS7);
    }
    if (WIFI_SSID7 != "" && WIFI_PASS7 != "")
    {
        debugLog("Adding wifi " + String(WIFI_SSID7));
        wifiMulti.addAP(WIFI_SSID7, WIFI_PASS7);
    }
    if (WIFI_SSID8 != "" && WIFI_PASS8 != "")
    {
        debugLog("Adding wifi " + String(WIFI_SSID8));
        wifiMulti.addAP(WIFI_SSID8, WIFI_PASS8);
    }
    if (WIFI_SSID9 != "" && WIFI_PASS9 != "")
    {
        debugLog("Adding wifi " + String(WIFI_SSID9));
        wifiMulti.addAP(WIFI_SSID9, WIFI_PASS9);
    }
    if (WIFI_SSID10 != "" && WIFI_PASS10 != "")
    {
        debugLog("Adding wifi " + String(WIFI_SSID10));
        wifiMulti.addAP(WIFI_SSID10, WIFI_PASS10);
    }
    initWifiMultiDone = true;
}

bool isWifiTaskCheck()
{
    wifiTaskMutex.lock();
    bool tmp = isWifiTaskRunning;
    wifiTaskMutex.unlock();
    return tmp;
}

#if DEBUG == 1 || DEBUG_MENUS == 1
String wifiStatus()
{
    switch (WiFi.status())
    {
    case WL_NO_SHIELD:
        return "NO SHIELD";
    case WL_IDLE_STATUS:
        return "IDLE STATUS";
    case WL_NO_SSID_AVAIL:
        return "NO SSID AVAIL";
    case WL_SCAN_COMPLETED:
        return "SCAN COMPLETED";
    case WL_CONNECTED:
        return "CONNECTED";
    case WL_CONNECT_FAILED:
        return "CONNECT FAILED";
    case WL_CONNECTION_LOST:
        return "CONNECTION LOST";
    case WL_DISCONNECTED:
        return "DISCONNECTED";
    default:
        return "UNKNOWN STATUS";
    }
}
#endif

#if DEBUG
void loopwifiDebug()
{
}
#endif