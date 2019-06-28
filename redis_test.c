#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>

#include "dbutil.h"

#define K_LEN 100   // key的长度
#define V_LEN 20    // value的长度

char* randomstr();
void test_db(int TEST_FREQUENCY);

int main(int argc, char** argv)
{
    printf("测试Redis数据库\n");

    
    // int TEST_FREQUENCY = 10000; // 基本操作的次数，默认为10000次

    // if (argc >= 2)
    // {
    //     TEST_FREQUENCY = atoi(argv[1]); // 如果传入了基本操作的次数，则使用传入的基本操作次数
    // }
    // test_db(TEST_FREQUENCY);
    

    int arr_length = 6;
    int TEST_FREQUENCY_ARR[] = {20000, 40000, 60000, 80000, 100000, 120000};
    for (int i=0; i<arr_length; i++) {
        printf("基本操作的次数：%d\n", TEST_FREQUENCY_ARR[i]);
        test_db(TEST_FREQUENCY_ARR[i]);
    }

    return 0;
}

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
    // 打开数据库
    redisContext *conn = redisConnect("127.0.0.1", 6379);
    if (conn != NULL && conn->err) {
        printf("connection error: %s\n", conn->errstr);
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


    redisReply *reply;

    // 测试添加
    {
        
        dbtime_start();
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            reply = redisCommand(conn, "SET %s %s", k[i], v);
        }
        dbtime_end();
        //freeReplyObject(reply);
        printf("测试添加%d次：", TEST_FREQUENCY);
        write_result("redis_write.txt");
    }

    // 测试获取
    {
        dbtime_start();
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            reply = redisCommand(conn, "GET %s", k[i]);
        }
        dbtime_end();
        //freeReplyObject(reply);
        printf("测试获取%d次：", TEST_FREQUENCY);
        write_result("redis_read.txt");
    }


    // 测试修改
    {
        char *v_new = (char *)malloc(V_LEN+1);
        for (int i=0; i<V_LEN; i++)
        {
            v_new[i] = 'A' + i;
        }
        v_new[V_LEN] = '\0';
        dbtime_start();
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            reply = redisCommand(conn, "SET %s %s", k[i], v_new);
        }
        dbtime_end();
        printf("测试修改%d次：", TEST_FREQUENCY);
        write_result("redis_update.txt");
    }

    // 测试删除
    {
        dbtime_start();
        for (int i = 0; i < TEST_FREQUENCY; ++i) {
            reply = redisCommand(conn, "DEL %s", k[i]);
        }
        dbtime_end();
        printf("测试删除%d次：", TEST_FREQUENCY);
        write_result("redis_delete.txt");
    }



    // 获取内存使用的信息，并将信息写入到文件中
    memory_message("redis_");

    freeReplyObject(reply);

    // 关闭数据库
    redisFree(conn);

    // 释放内存
    for (int i=0; i<TEST_FREQUENCY; i++)
    {
        free(k[i]);
    }
    free(v);
}



// int main1()
// {
//     redisContext *conn = redisConnect("127.0.0.1", 6379);
//     if (conn != NULL && conn->err) {
//         printf("connection error: %s\n", conn->errstr);
//         return 0;
//     }

//     redisReply *reply;
//     reply = redisCommand(conn, "SET %s %s", "foo", "bar");
//     freeReplyObject(reply);

//     reply = redisCommand(conn, "GET %s", "foo");
//     printf("%s\n", reply->str);
//     freeReplyObject(reply);

//     redisFree(conn);

//     return 0;
// }