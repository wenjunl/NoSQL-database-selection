package wj.data_chart;

import org.apache.commons.io.FileUtils;

import java.io.File;

public class Util {

    public static final String ROOT_DIR = "/Users/wenjun/Desktop/database/result_data/";
    //public static final String ROOT_DIR = "/home/wenjun/db_test_data/";

    public static final String WRITE_MODE = "write";
    public static final String READ_MODE = "read";
    public static final String UPDATE_MODE = "update";
    public static final String DELETE_MODE = "delete";

    public static final String VM_SIZE_MODE = "vmsize";
    public static final String VM_RSS_MODE = "vmrss";
    public static final String VM_DATA_MODE = "vmdata";
    public static final String VM_STK_MODE = "vmstk";


    /**
     *
     * @param mode 写，读，更新，删除
     * @param flag true:表示数据用于折线图的显示，false:表示数据用于表格里显示
     * @return
     */
    public static String getCompareData(String mode, boolean flag) {
        String filePath1 = ROOT_DIR + "tc_"+ mode +".txt";
        String filePath2 = ROOT_DIR + "levelDB_"+ mode +".txt";
        String filePath3 = ROOT_DIR + "rocksDB_"+ mode +".txt";
        String filePath4 = ROOT_DIR + "bdb_"+ mode +".txt";
        String filePath5 = ROOT_DIR + "redis_"+ mode +".txt";

        File tcFile = new File(filePath1);
        File levelDBFile = new File(filePath2);
        File rocksDBFile = new File(filePath3);
        File bdbFile = new File(filePath4);
        File redisFile = new File(filePath5);

        String tcDataStr = null;
        String rocksDBDataStr = null;
        String levelDBDataStr = null;

        String bdbDataStr = null;
        String redisDataStr = null;

        try {
            tcDataStr = FileUtils.readFileToString(tcFile, "UTF-8");
            levelDBDataStr = FileUtils.readFileToString(levelDBFile, "UTF-8");
            rocksDBDataStr = FileUtils.readFileToString(rocksDBFile, "UTF-8");

            bdbDataStr = FileUtils.readFileToString(bdbFile, "UTF-8");
            redisDataStr = FileUtils.readFileToString(redisFile, "UTF-8");

        } catch (Exception e) {
            //TODO
        }

        tcDataStr = Util.handleStr(tcDataStr);
        rocksDBDataStr = Util.handleStr(rocksDBDataStr);
        levelDBDataStr = Util.handleStr(levelDBDataStr);

        bdbDataStr = Util.handleStr(bdbDataStr);
        redisDataStr = Util.handleStr(redisDataStr);

        String returnJson = null;

        if (flag) {
            returnJson = "{\"tc\": "+tcDataStr+"," +
                    "             \"levelDB\": "+levelDBDataStr+"," +
                    "             \"rocksDB\": "+rocksDBDataStr+"," +
                    "             \"bdb\": "+bdbDataStr+"," +
                    "             \"redis\": "+redisDataStr+"}";
        } else {
            returnJson = "{\"data\":[" +
                    "[\"Tokyo Cabinet\","+tcDataStr.substring(1, tcDataStr.length()-1)+"]," +
                    "[\"levelDB\","+levelDBDataStr.substring(1, levelDBDataStr.length()-1)+"]," +
                    "[\"rocksDB\","+rocksDBDataStr.substring(1, rocksDBDataStr.length()-1)+"]," +
                    "[\"BerkeleyDB\","+bdbDataStr.substring(1, bdbDataStr.length()-1)+"]," +
                    "[\"Redis\","+redisDataStr.substring(1, redisDataStr.length()-1)+"]" +
                    "]}";
        }

        return returnJson;
    }


    public static String getTCDBTestData(boolean flag) {
        String filePathPrefix = ROOT_DIR + "tc_test/";

        String writeFilePath = filePathPrefix + "tc_write.txt";
        String readFilePath = filePathPrefix + "tc_read.txt";
        String updateFilePath = filePathPrefix + "tc_update.txt";
        String deleteFilePath = filePathPrefix + "tc_delete.txt";

        String writeDataStr = null;
        String readDataStr = null;
        String updateDataStr = null;
        String deleteDataStr = null;

        try {
            writeDataStr = FileUtils.readFileToString(new File(writeFilePath), "UTF-8");
            readDataStr = FileUtils.readFileToString(new File(readFilePath), "UTF-8");
            updateDataStr = FileUtils.readFileToString(new File(updateFilePath), "UTF-8");
            deleteDataStr = FileUtils.readFileToString(new File(deleteFilePath), "UTF-8");
        } catch (Exception e) {
            // TODO
        }

        writeDataStr = Util.handleStr(writeDataStr);
        readDataStr = Util.handleStr(readDataStr);
        updateDataStr = Util.handleStr(updateDataStr);
        deleteDataStr = Util.handleStr(deleteDataStr);

        String returnJson = null;

        if (flag) {
            returnJson = "{\"tc_write\": "+writeDataStr+"," +
                    "             \"tc_read\": "+readDataStr+"," +
                    "             \"tc_update\": "+updateDataStr+","+
                    "             \"tc_delete\": "+deleteDataStr+"}";
        } else {
            returnJson = "{\"data\": [" +
                    "[\"写\", "+writeDataStr.substring(1,writeDataStr.length()-1)+"]," +
                    "[\"读\", "+readDataStr.substring(1,readDataStr.length()-1)+"]," +
                    "[\"更新\", "+updateDataStr.substring(1,updateDataStr.length()-1)+"]," +
                    "[\"删除\", "+deleteDataStr.substring(1,deleteDataStr.length()-1)+"]" +
                    "]}";
        }

        return returnJson;
    }

    public static String handleStr(String str) {
        if (str.endsWith(",")) {
            str = str.substring(0, str.length()-1);
        }

        str = "[" + str + "]";

        return str;
    }
}
