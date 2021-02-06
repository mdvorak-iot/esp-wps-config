#include <esp_netif.h>
#include <esp_wifi.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <wps_config.h>

static const char TAG[] = "example";

void setup()
{
	esp_log_level_set("wifi", ESP_LOG_WARN);
	esp_log_level_set("auto_wps", ESP_LOG_DEBUG);

	// Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	// Initalize WiFi
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
	assert(sta_netif);
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_start());

	// Start WPS
	ESP_ERROR_CHECK(wps_config_start());

	// Setup complete
	ESP_LOGI(TAG, "started");
}

void loop()
{
	vTaskDelay(1);
}

extern "C" _Noreturn void app_main()
{
	setup();
	for (;;)
	{
		loop();
	}
}
