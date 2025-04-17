#ifndef _STORAGEMGR_H_
#define _STORAGEMGR_H_

#include "config.h"
#include "sbuffer.h"
#include <sqlite3.h>

/**
 * Hàm chính của storage manager thread
 * @param arg pointer to thread_args_t structure
 * @return NULL on completion
 */
void *storage_manager(void *arg);

/**
 * Thiết lập kết nối đến SQLite database
 * @param db_name name of the database file
 * @param db pointer to SQLite database handle
 * @return 0 on success, -1 on failure
 */
int setup_database(const char *db_name, sqlite3 **db);

/**
 * Tạo bảng trong database nếu chưa tồn tại
 * @param db SQLite database handle
 * @return 0 on success, -1 on failure
 */
int create_tables(sqlite3 *db);

/**
 * Lưu dữ liệu cảm biến vào database
 * @param db SQLite database handle
 * @param data sensor data to be stored
 * @return 0 on success, -1 on failure
 */
int store_sensor_data(sqlite3 *db, sensor_data_t *data);

#endif