package wj.data_chart.controller;


import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import wj.data_chart.Util;

@Controller
public class DataController {

    @GetMapping("/")
    public String index() {
        return "index";
    }


    @GetMapping("/compare_write")
    public String compareWrite() {
        return "compare_write";
    }

    @GetMapping("/compare_read")
    public String compareRead() {
        return "compare_read";
    }

    @GetMapping("/compare_update")
    public String compareUpdate() {
        return "compare_update";
    }

    @GetMapping("/compare_delete")
    public String compareDelete() {
        return "compare_delete";
    }


    @GetMapping("/table_write")
    public String tableWrite() {
        return "table_write";
    }

    @GetMapping("/table_read")
    public String tableRead() {
        return "table_read";
    }

    @GetMapping("/table_update")
    public String tableUpdate() {
        return "table_update";
    }

    @GetMapping("/table_delete")
    public String tableDelete() {
        return "table_delete";
    }


    @GetMapping("/compare_vm_size")
    public String compareVmSize() {
        return "compare_vm_size";
    }

    @GetMapping("/compare_vm_rss")
    public String compareVmRSS() {
        return "compare_vm_rss";
    }

    @GetMapping("/compare_vm_data")
    public String compareVmData() {
        return "compare_vm_data";
    }

    @GetMapping("/compare_vm_stk")
    public String compareVmStk() {
        return "compare_vm_stk";
    }

    @GetMapping("/table_vm_size")
    public String tableVmSize() {
        return "table_vm_size";
    }

    @GetMapping("/table_vm_rss")
    public String tableVmRSS() {
        return "table_vm_rss";
    }

    @GetMapping("/table_vm_data")
    public String tableVmData() {
        return "table_vm_data";
    }

    @GetMapping("/table_vm_stk")
    public String tableVmStk() {
        return "table_vm_stk";
    }

    @GetMapping("/tc_test")
    public String tcTest() {
        return "tc_test";
    }

    @GetMapping("/tc_table_test")
    public String tcTableTest() {
        return "tc_table_test";
    }

    @ResponseBody
    @RequestMapping("tcTestData")
    public String tcTestData() {
        return Util.getTCDBTestData(true);
    }

    @ResponseBody
    @RequestMapping("tcTableTestData")
    public String tcTableTestData() {
        return Util.getTCDBTestData(false);
    }

    @ResponseBody
    @RequestMapping("writeData")
    public String getWriteData() {
        return Util.getCompareData(Util.WRITE_MODE, true);
    }

    @ResponseBody
    @RequestMapping("readData")
    public String getReadData() {
        return Util.getCompareData(Util.READ_MODE, true);
    }

    @ResponseBody
    @RequestMapping("updateData")
    public String getUpdateData() {
        return Util.getCompareData(Util.UPDATE_MODE, true);
    }

    @ResponseBody
    @RequestMapping("deleteData")
    public String getDeleteData() {
        return Util.getCompareData(Util.DELETE_MODE, true);
    }

    @ResponseBody
    @RequestMapping("tableWriteData")
    public String getTableWriteData() {
        return Util.getCompareData(Util.WRITE_MODE, false);
    }

    @ResponseBody
    @RequestMapping("tableReadData")
    public String getTableReadData() {
        return Util.getCompareData(Util.READ_MODE, false);
    }

    @ResponseBody
    @RequestMapping("tableUpdateData")
    public String getTableUpdateData() {
        return Util.getCompareData(Util.UPDATE_MODE, false);
    }

    @ResponseBody
    @RequestMapping("tableDeleteData")
    public String getTableDeleteData() {
        return Util.getCompareData(Util.DELETE_MODE, false);
    }

    @ResponseBody
    @RequestMapping("vmSizeData")
    public String getVmSizeData() {
        return Util.getCompareData(Util.VM_SIZE_MODE, true);
    }

    @ResponseBody
    @RequestMapping("vmRSSData")
    public String getVmRSSData() {
        return Util.getCompareData(Util.VM_RSS_MODE, true);
    }

    @ResponseBody
    @RequestMapping("vmDataData")
    public String getVmDataData() {
        return Util.getCompareData(Util.VM_DATA_MODE, true);
    }

    @ResponseBody
    @RequestMapping("vmStkData")
    public String getVmStkData() {
        return Util.getCompareData(Util.VM_STK_MODE, true);
    }


    @ResponseBody
    @RequestMapping("tableVmSizeData")
    public String getTableVmSizeData() {
        return Util.getCompareData(Util.VM_SIZE_MODE, false);
    }

    @ResponseBody
    @RequestMapping("tableVmRSSData")
    public String getTableVmRSSData() {
        return Util.getCompareData(Util.VM_RSS_MODE, false);
    }

    @ResponseBody
    @RequestMapping("tableVmDataData")
    public String getTableVmDataData() {
        return Util.getCompareData(Util.VM_DATA_MODE, false);
    }

    @ResponseBody
    @RequestMapping("tableVmStkData")
    public String getTableVmStkData() {
        return Util.getCompareData(Util.VM_STK_MODE, false);
    }

    @ResponseBody
    @RequestMapping("/hello")
    public String hello() {
        return "hello";
    }


}
