#include <string>  
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <tcutil.h>
#include <tchdb.h>

#include "dbutil.h"

#define K_LEN 100   // key的长度
#define V_LEN 20    // value的长度

void test_db(int TEST_FREQUENCY);

//测试TokyoCabinet数据库
//测试次数从：20000, 40000, 60000, 80000, 100000, 120000, 140000, 160000, 200000, 400000, 600000, 800000, 1000000

int main(int argc, char** argv)
{
    printf("测试TokyoCabinet数据库\n");
    
    // int TEST_FREQUENCY = 10000; // 基本操作的次数，默认为10000次

    // if (argc >= 2)
    // {
    //     TEST_FREQUENCY = atoi(argv[1]); // 如果传入了基本操作的次数，则使用传入的基本操作次数
    //     test_db(TEST_FREQUENCY);
    // }

    int arr_length = 13;
    int TEST_FREQUENCY_ARR[] = {20000, 40000, 60000, 80000, 100000, 120000, 140000, 160000, 200000, 400000, 600000, 800000, 1000000};
    for (int i=0; i<arr_length; i++) {
    	printf("基本操作的次数：%d\n", TEST_FREQUENCY_ARR[i]);
    	test_db(TEST_FREQUENCY_ARR[i]);
    }

    return 0;
}

void test_db(int TEST_FREQUENCY)
{
	// 打开数据库
    TCHDB *db;
    db = tchdbnew();
    //tchdbopen(db, "*", HDBOWRITER | HDBOCREAT);
    tchdbopen(db, "/dev/shm/tc_test_1000000.hdb", HDBOWRITER | HDBOCREAT);

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
        dbtime_start();
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            int r = tchdbput(db, k[i], K_LEN, v, V_LEN);
	       //printf("r = %d\n", r);
        }
        dbtime_end();
        printf("测试添加%d次：", TEST_FREQUENCY);
        write_result("tc_test/tc_write.txt");
    }

    // 测试获取
    {
        dbtime_start();
        char v1[1024] = {0};
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
    	    int r = tchdbget3(db, k[i], K_LEN, v1, 1024);
    	    //printf("r = %d\n", r);
    	    //printf("%s\n", v1);
        }
        dbtime_end();
        printf("测试获取%d次：", TEST_FREQUENCY);
        write_result("tc_test/tc_read.txt");
    }

    // 测试修改
    {
    	for (int i=0; i<V_LEN; i++)
    	{
    		v[i] = 'A' + i;
    	}

        dbtime_start();
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            tchdbput(db, k[i], K_LEN, v, V_LEN);
        }
        dbtime_end();
        printf("测试修改%d次：", TEST_FREQUENCY);
        write_result("tc_test/tc_update.txt");
    }

    // 测试删除
    {
        dbtime_start();
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            tchdbout(db, k[i], K_LEN);
        }
        dbtime_end();
        printf("测试删除%d次：", TEST_FREQUENCY);
        write_result("tc_test/tc_delete.txt");
    }

    // 获取内存使用的信息，并将信息写入到文件中
    memory_message("tc_");

    tchdbclose(db);
    tchdbdel(db);

    // 释放内存
    for (int i=0; i<TEST_FREQUENCY; i++)
    {
    	free(k[i]);
    }
    free(v);
}
