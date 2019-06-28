#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <db.h>

#include "dbutil.h"

#define K_LEN 100   // key的长度
#define V_LEN 20    // value的长度

char* randomstr()
{
    static char buf[K_LEN+1];
    int len = K_LEN+1;
    for (int i = 0; i < len; ++i) 
    {
        buf[i] = 'A' + rand() % 26;
    }
    buf[len] = '\0';
    return buf;
}

void test_db(int TEST_FREQUENCY);

/* 数据结构DBT在使用前，应首先初始化，否则编译可通过但运行时报参数错误  */
void init_DBT(DBT * key, DBT * data)
{
    memset(key, 0, sizeof(DBT));
    memset(data, 0, sizeof(DBT));
}

int main(int argc, char** argv)
{
    printf("测试BerkeleyDB数据库\n");

    /*
    int TEST_FREQUENCY = 10000; // 基本操作的次数，默认为10000次

    if (argc >= 2)
    {
        TEST_FREQUENCY = atoi(argv[1]); // 如果传入了基本操作的次数，则使用传入的基本操作次数
    }
    */

    int arr_length = 6;
    int TEST_FREQUENCY_ARR[] = {20000, 40000, 60000, 80000, 100000, 120000};
    for (int i=0; i<arr_length; i++) {
    	printf("基本操作的次数：%d\n", TEST_FREQUENCY_ARR[i]);
    	test_db(TEST_FREQUENCY_ARR[i]);
    }

    return 0;
}

void test_db(int TEST_FREQUENCY)
{
	// 打开数据库
    DB_ENV *dbenv;
    DB *dbp;
    DBT key, data;

    int ret = 0;

    /* initialize env handler */
    if (ret = db_env_create(&dbenv, 0)) { 
        printf("db_env_create ERROR: %s\n", db_strerror(ret));
    }

    u_int32_t flags = DB_CREATE | DB_INIT_MPOOL | DB_INIT_CDB | DB_THREAD;

    if (ret = dbenv->open(dbenv, "/dev/shm", flags, 0)) {
        printf("dbenv->open ERROR: %s\n", db_strerror(ret));
    }   

    /* initialize db handler */
    if (ret = db_create(&dbp, dbenv, 0)) {
        printf("db_create ERROR: %s\n", db_strerror(ret));
    }


    flags = DB_CREATE | DB_THREAD;

    if (ret = dbp->open(dbp, NULL, "single.db", NULL, DB_BTREE, flags, 0664)) {
        printf("dbp->open ERROR: %s\n", db_strerror(ret));
    }

    // -----------------------生成key数组-------------------------------------
    // 根据基本操作次数生成key数组，为了保证key数组中的每个key都不同，需要随机生成每个key
    srand(2017);
    char *k[TEST_FREQUENCY]; // key数组
    for (int i = 0; i < TEST_FREQUENCY; ++i) {
        k[i] = (char *)malloc(K_LEN + 1);
        for (int j = 0; j < K_LEN; ++j) {
            k[i][j] = 'A' + rand() % 26;
        }
        k[i][K_LEN] = '\0';
    }
    // -----------------------生成key数组-------------------------------------


    // -----------------------生成value-------------------------------------
    char *v = (char *)malloc(V_LEN+1);
    for (int i=0; i<V_LEN; i++)
    {
        v[i] = 'a' + i;
    }
    v[V_LEN] = '\0';
    // -----------------------生成value-------------------------------------


    // 测试添加
    {
        init_DBT(&key, &data);
        dbtime_start();
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            key.data = k[i];
            key.size = K_LEN;
            data.data = v;
            data.size = V_LEN;
            dbp->put(dbp, NULL, &key, &data, DB_NOOVERWRITE);
        }
        dbtime_end();
        printf("测试添加%d次：", TEST_FREQUENCY);
        write_result("bdb_write.txt");
    }

    // 测试获取 ？
    {
        DBT key1, data1;
        init_DBT(&key1, &data1);
        data1.flags = DB_DBT_MALLOC;
        dbtime_start();
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            key1.data = k[i];
            key1.size = K_LEN;
            dbp->get(dbp, NULL, &key1, &data1, 0);
        }
        dbtime_end();
        printf("测试获取%d次：", TEST_FREQUENCY);
        write_result("bdb_read.txt");
    }


    // 测试修改
    {
        dbtime_start();
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            key.data = k[i];
            key.size = K_LEN;
            data.data = v;
            data.size = V_LEN;
            dbp->put(dbp, NULL, &key, &data, 0);
        }
        dbtime_end();
        printf("测试修改%d次：", TEST_FREQUENCY);
        write_result("bdb_update.txt");
    }

    // 测试删除
    {
        dbtime_start();
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            key.data = k[i];
            key.size = K_LEN;
            dbp->del(dbp, NULL, &key, 0);
        }
        dbtime_end();
        printf("测试删除%d次：", TEST_FREQUENCY);
        write_result("bdb_delete.txt");
    }



    // 获取内存使用的信息，并将信息写入到文件中
    memory_message("bdb_");

    if(dbp) {
        dbp->close(dbp, 0);
    }

    if (dbenv) {
        dbenv->close(dbenv, 0);
    }

    // 释放内存
    for (int i=0; i<TEST_FREQUENCY; i++)
    {
        free(k[i]);
    }
    free(v);
}
