#include <cassert>  
#include <string>  
#include <iostream>
#include <chrono>
#include <stdio.h>

#include <rocksdb/db.h>

#include "dbutil.h"

#define K_LEN 100
#define V_LEN 20

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

void test_db(int TEST_FREQUENCY)
{
    rocksdb::DB* db;
    rocksdb::Options options;
    options.create_if_missing = true;

    // 打开数据库
    rocksdb::Status status = rocksdb::DB::Open(options, "/dev/shm", &db);
    assert(status.ok());

    srand(2017);
    std::string k[TEST_FREQUENCY];
    for (int i = 0; i < TEST_FREQUENCY; ++i) {
        k[i] = (randomstr());
    }
    
    std::string v("abcdefghijklmnopqrstuvwxyz");

    // 测试添加
    {
        dbtime_start();
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            status = db->Put(rocksdb::WriteOptions(), k[i], v);
            //assert(status.ok());
        }
        dbtime_end();
        printf("测试添加%d次：", TEST_FREQUENCY);
        write_result("rocksDB_write.txt");
    }

    // 测试获取
    {
        dbtime_start();
        std::string v2[TEST_FREQUENCY];
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            status = db->Get(rocksdb::ReadOptions(), k[i], &v2[i]);
            //assert(status.ok());
        }
        dbtime_end();
        printf("测试获取%d次：", TEST_FREQUENCY);
        write_result("rocksDB_read.txt");
        
        // 验证获取结果是否正确
        std::string ss;
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            if (v2[i] != v) {
                std::cout << "第 " << i << " 个结果不正确" << std::endl;
                std::cout << v2[i] << std::endl;
            }
        }
    }

    // 测试修改
    {
        std::string v3("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

        dbtime_start();
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            status = db->Put(rocksdb::WriteOptions(), k[i], v3);
            //assert(status.ok());
        }
        dbtime_end();
        printf("测试修改%d次：", TEST_FREQUENCY);
        write_result("rocksDB_update.txt");
    }

    // 测试删除
    {
        dbtime_start();
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            status = db->Delete(rocksdb::WriteOptions(), k[i]);
            //assert(status.ok());
        }
        dbtime_end();
        printf("测试删除%d次：", TEST_FREQUENCY);
        write_result("rocksDB_delete.txt");
    }


    // 获取内存使用的信息，并将信息写入到文件中
    memory_message("rocksDB_");

    delete db;
}

int main(int argc, char** argv) 
{
    printf("测试RocksDB数据库\n");

    /*
    int TEST_FREQUENCY = 10000; // 基本操作的次数，默认为10000次
    if (argc >= 2)
    {
        TEST_FREQUENCY = atoi(argv[1]);    
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
