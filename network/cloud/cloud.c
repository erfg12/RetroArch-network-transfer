#include "cloud.h"

void cloud_upload(const char *file, char *data, size_t size) {
	size_t n;
    unsigned char *base;
    char url[1024] = "transfer.php";
    mbedtls_base64_encode(NULL, 0, &n, data, size);
    char *file_name = NULL;
    net_http_urlencode(&file_name, file);
    base = (unsigned char*)mbedtls_calloc(n, 1);
    size_t base_len = n + strlen(file_name) + strlen("data=&upload=") + 1;
    mbedtls_base64_encode(base, n, &n, data, size);
    char* post_data = mbedtls_calloc(base_len, 1);

    /* we need to clean base64 string for url */
    for (int i = 0; i < n; i++)
    {
       if (base[i] == '+') base[i] = '-';
       if (base[i] == '/') base[i] = '_';
    }

    snprintf(post_data, base_len, "upload=%s&data=%s", file_name, base);
    task_push_http_post_transfer(url, post_data, true, NULL, NULL, NULL);
    mbedtls_free(base);
    mbedtls_free(post_data);
	free(file);
}

char filename[1024] = "";

void cloud_download() {
	char url[1024] = "transfer.php";
	char *file_name = NULL;
	net_http_urlencode(&file_name, filename);
	size_t base_len = strlen(file_name) + strlen("download=") + 1;
	char* post_data = mbedtls_calloc(base_len, 1);

	snprintf(post_data, base_len, "download=%s", file_name);
	task_push_http_post_transfer(url, post_data, true, NULL, store_download_cb, NULL);
	mbedtls_free(post_data);
}

void store_download_cb(void *task_data, void *user_data, const char *error)
{
	RARCH_LOG("receiving download data... \n");

	if (task_data)
	{
		unsigned char *buf;
		int ret;
		size_t len;
		http_transfer_data_t *data = (http_transfer_data_t*)task_data;
		global_t *global = global_get_ptr();
		size_t state_path_size = 16384 * sizeof(char);
		char *state_path = (char*)malloc(state_path_size);

		if (data->len == 0)
		{
			free(task_data);
			return;
		}

		ret = mbedtls_base64_decode(NULL, 0, &len, data->data, data->len);

		if ((buf = (unsigned char*)mbedtls_calloc(1, len)) == NULL)
			return(MBEDTLS_ERR_PEM_ALLOC_FAILED);

		if ((ret = mbedtls_base64_decode(buf, len, &len, data->data, data->len)) != 0)
		{
			mbedtls_free(buf);
			return(MBEDTLS_ERR_PEM_INVALID_DATA + ret);
		}

		/* we need to un-clean base64 string from url */
		for (int i = 0; i < len; i++)
		{
			if (buf[i] == '-') buf[i] = '+';
			if (buf[i] == '_') buf[i] = '/';
		}

		FILE *fp = fopen(filename, "wb");
		if (fp)
			write_scores(fp);

		free(task_data);
		mbedtls_free(buf);
	}
}