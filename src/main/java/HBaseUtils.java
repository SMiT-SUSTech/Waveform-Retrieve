import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.Cell;
import org.apache.hadoop.hbase.CellUtil;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.client.*;
import org.apache.hadoop.hbase.util.Bytes;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.awt.*;
import java.io.IOException;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.List;

public class HBaseUtils {
    private static final Logger logger = LoggerFactory.getLogger(HBaseUtils.class);
    private static Configuration config;
    private static Connection conn;
    public native void callbackhello();
    public native void callbackHBaseUtils();
    public native void callbackCreateTable(String tName, String fName);
    public native void callbackPut(String tableName, String rowKey, String family, String column,
                                   long startLocation, String data);
    public native void callbackGet(String tableName, String rowKey, String family, String column,
                                   long ts, String data);

//    public HBaseUtils(){};

    public HBaseUtils() {
        System.out.println("init HBaseUtis");
        config = HBaseConfiguration.create();
        /*
        默认端口为2181,但需要与hbase-site.xml文件中对应的端口号一致.
        单机版配置中此参数为2182,请注意修改
        * */
        config.set("hbase.zookeeper.property.clientPort", "2181");
        /*
        此处需配置好hosts
        * */
        config.set("hbase.zookeeper.quorum", "master");
        try {
            conn = ConnectionFactory.createConnection(config);
            logger.info("Build Connection");
            System.out.println("new Build Connection");
            System.out.println(conn);
        } catch (IOException e) {
            e.printStackTrace();
            if (conn != null) {
                try {
                    conn.close();
                } catch (IOException er) {
                    er.printStackTrace();
                }
            }
        }
//        try {
//            config.addResource(new Path(ClassLoader.getSystemResource("hbase-site.xml").toURI()));
//        } catch (URISyntaxException e) {
//            e.printStackTrace();
//        }
//        logger.info("Add config: hbase-site.xml");
//        try {
//            config.addResource(new Path(ClassLoader.getSystemResource("core-site.xml").toURI()));
//        } catch (URISyntaxException e) {
//            e.printStackTrace();
//        }
//        logger.info("Add config: core-site.xml");
//
//        try {
//            conn = ConnectionFactory.createConnection(config);
//            logger.info("Build Connection");
//            System.out.println("new Build Connection");
//            System.out.println(conn);
//        } catch (IOException e) {
//            e.printStackTrace();
//            if (conn != null) {
//                try {
//                    conn.close();
//                } catch (IOException er) {
//                    er.printStackTrace();
//                }
//            }
//        }
//        finally {
//            if (connection != null) {
//                try {
//                    connection.close();
//                } catch (IOException e) {
//                    e.printStackTrace();
//                }
//            }
//        }
    }

    /**
     * 获取配置
     *
     * @return Configuration
     */
    private static Configuration getConfiguration() {
        Configuration config = HBaseConfiguration.create();
        try {
            config.addResource(new Path(ClassLoader.getSystemResource("hbase-site.xml").toURI()));
        } catch (URISyntaxException e) {
            e.printStackTrace();
        }
        logger.info("Add config: hbase-site.xml");
        try {
            config.addResource(new Path(ClassLoader.getSystemResource("core-site.xml").toURI()));
        } catch (URISyntaxException e) {
            e.printStackTrace();
        }
        logger.info("Add config: core-site.xml");
        return config;
    }

    public static void hello() {
        System.out.println("hello, world!!");
    }

    public static void createTable(String tName, String fName) {
        System.out.println("java:: create table");
        System.out.println("java: createTable con:"+conn);
        if (conn.isClosed()) {
            System.out.println("connect is closed");
        }
        try (Admin admin = conn.getAdmin()) {
            System.out.println("in try");
            TableName tableName = TableName.valueOf(tName);
            System.out.println("Table name: " + tableName.toString());
            if (admin.tableExists(tableName)) {
                System.out.println("in if");
//                logger.warn("table:{} exists!", tableName.getName());
                System.out.println("table has existed");
            } else {
                System.out.println("in else");
                TableDescriptorBuilder builder = TableDescriptorBuilder.newBuilder(tableName);
                ColumnFamilyDescriptorBuilder colFamilyDesc = ColumnFamilyDescriptorBuilder.newBuilder(Bytes.toBytes(fName));
                ColumnFamilyDescriptor familyDescriptor = colFamilyDesc.build();
                builder.setColumnFamily(familyDescriptor);
                admin.createTable(builder.build());
                System.out.println("create table success");
            }
        } catch (IOException e) {
            System.out.println("error: "+e);
            e.printStackTrace();
        }
//        Configuration conf = HBaseConfiguration.create();
//        conf.set("hbase.zookeeper.property.clientPort", "2181");
//        conf.set("hbase.zookeeper.quorum", "master");
//        try {
//            conf.addResource(new Path(ClassLoader.getSystemResource("hbase-site.xml").toURI()));
//        } catch (URISyntaxException e) {
//            System.out.println(e);
//            e.printStackTrace();
//        }
////        logger.info("Add config: hbase-site.xml");
//        System.out.println("Add config: hbase-site.xml");
//        try {
//            conf.addResource(new Path(ClassLoader.getSystemResource("core-site.xml").toURI()));
//        } catch (URISyntaxException e) {
//            System.out.println(e);
//            e.printStackTrace();
//        }
////        logger.info("Add config: core-site.xml");
//        System.out.println("Add config: core-site.xml");
//        System.out.println("config: "+conf);
//
//        Connection con = null;
//        System.out.println("conn null");
//        try {
//            System.out.println("in try conn null");
//            con = ConnectionFactory.createConnection(conf);
////            logger.info("Build Connection");
//            System.out.println("new Build Connection");
//            System.out.println(con);
//        } catch (IOException e) {
//            System.out.println("conn catch");
//            e.printStackTrace();
//            System.out.println(e);
//            if (con != null) {
//                try {
//                    con.close();
//                } catch (IOException er) {
//                    er.printStackTrace();
//                }
//            }
//        }
    }

    /**
     * 创建表
     *
     * @param tableName
     * @param columnFamilies
     * @throws IOException
     */
    public static void createTable(TableName tableName, String... columnFamilies) {
        try (HBaseAdmin admin = (HBaseAdmin) conn.getAdmin()) {
            if (admin.tableExists(tableName)) {
                logger.warn("table:{} exists!", tableName.getName());
            } else {
                TableDescriptorBuilder builder = TableDescriptorBuilder.newBuilder(tableName);
                for (String columnFamily : columnFamilies) {
                    builder.setColumnFamily(ColumnFamilyDescriptorBuilder.of(columnFamily));
                }
                admin.createTable(builder.build());
                logger.info("create table:{} success!", tableName.getName());
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * 创建表
     *
     * @param connection
     * @param tableName
     * @param columnFamilies
     * @throws IOException
     */
    public static void createTable(Connection connection, TableName tableName, String... columnFamilies) {
        try (Admin admin = connection.getAdmin()) {
            if (admin.tableExists(tableName)) {
                logger.warn("table:{} exists!", tableName.getName());
            } else {
                TableDescriptorBuilder builder = TableDescriptorBuilder.newBuilder(tableName);
                for (String columnFamily : columnFamilies) {
                    builder.setColumnFamily(ColumnFamilyDescriptorBuilder.of(columnFamily));
                }
                admin.createTable(builder.build());
                logger.info("create table:{} success!", tableName.getName());
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void put(String tableName, String rowKey, String family, String column,
                           long startLocation, String data) {
//        System.out.println("in java hbase put");
        try (Table table = conn.getTable(TableName.valueOf(tableName))) {
//            System.out.println("in java hbase put 555");
            Put put = new Put(Bytes.toBytes(rowKey));
            put.addColumn(Bytes.toBytes(family), Bytes.toBytes(column), startLocation, Bytes.toBytes(data));
            table.put(put);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    public static void put(Connection connection, String tableName, String rowKey, String family, String column,
                           long startLocation, String data) {
        try (Table table = connection.getTable(TableName.valueOf(tableName))) {
            Put put = new Put(Bytes.toBytes(rowKey));
            put.addColumn(Bytes.toBytes(family), Bytes.toBytes(column), startLocation, Bytes.toBytes(data));
            table.put(put);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static String get(String tableName, String rowKey, String family, String column, long ts) {
        String data = "";
//        System.out.println("java in get func 555");
        try (Table table = conn.getTable(TableName.valueOf(tableName))) {
//            System.out.println("java in get func 666");
            Get get = new Get(Bytes.toBytes(rowKey));
            get.addColumn(Bytes.toBytes(family), Bytes.toBytes(column));
            get.setTimestamp(ts);

            Result result = table.get(get);
            List<Cell> cells = result.listCells();
            for (Cell c : cells) {
                String sValue = Bytes.toString(CellUtil.cloneValue(c));
                data += sValue;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
//        System.out.println("java res: " + data);
        return data;
    }


//    public static List<String> get(String tableName, String rowKey, String family, String column,
//                                   long ts, String data) {
//        List<String> list = new ArrayList<>();
//        try (Table table = conn.getTable(TableName.valueOf(tableName))) {
//            Get get = new Get(Bytes.toBytes(rowKey));
//            get.addColumn(Bytes.toBytes(family), Bytes.toBytes(column));
//            get.setTimestamp(ts);
//
//            Result result = table.get(get);
//            List<Cell> cells = result.listCells();
//            for (Cell c : cells) {
//                String sValue = Bytes.toString(CellUtil.cloneValue(c));
//                list.add(sValue);
//            }
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
//        return list;
//    }

    public static List<String> get(Connection connection, String tableName, String rowKey, String family, String column,
                                   long ts, String data) {
        List<String> list = new ArrayList<>();
        try (Table table = connection.getTable(TableName.valueOf(tableName))) {
            Get get = new Get(Bytes.toBytes(rowKey));
            get.addColumn(Bytes.toBytes(family), Bytes.toBytes(column));
            get.setTimestamp(ts);

            Result result = table.get(get);
            List<Cell> cells = result.listCells();
            for (Cell c : cells) {
                String sValue = Bytes.toString(CellUtil.cloneValue(c));
                list.add(sValue);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return list;
    }

    public static List<String> get(String tableName, String rowKey, String family, String column,
                                   long minTs, long maxTs, String data) {
        List<String> list = new ArrayList<>();
        try (Table table = conn.getTable(TableName.valueOf(tableName))) {
            Get get = new Get(Bytes.toBytes(rowKey));
            get.addColumn(Bytes.toBytes(family), Bytes.toBytes(column));
            get.setTimeRange(minTs, maxTs);

            Result result = table.get(get);
            List<Cell> cells = result.listCells();
            for (Cell c : cells) {
                String sValue = Bytes.toString(CellUtil.cloneValue(c));
                list.add(sValue);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return list;
    }


    public static List<String> get(Connection connection, String tableName, String rowKey, String family, String column,
                                   long minTs, long maxTs, String data) {
        List<String> list = new ArrayList<>();
        try (Table table = connection.getTable(TableName.valueOf(tableName))) {
            Get get = new Get(Bytes.toBytes(rowKey));
            get.addColumn(Bytes.toBytes(family), Bytes.toBytes(column));
            get.setTimeRange(minTs, maxTs);

            Result result = table.get(get);
            List<Cell> cells = result.listCells();
            for (Cell c : cells) {
                String sValue = Bytes.toString(CellUtil.cloneValue(c));
                list.add(sValue);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return list;
    }

    public static void deleteTable(String tableName) {
        try (Admin admin = conn.getAdmin()) {
            if (admin.tableExists(TableName.valueOf(tableName))) {
                admin.disableTable(TableName.valueOf(tableName));
                admin.deleteTable(TableName.valueOf(tableName));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void deleteTable(Connection connection, String tableName) {
        try (Admin admin = connection.getAdmin()) {
            if (admin.tableExists(TableName.valueOf(tableName))) {
                admin.disableTable(TableName.valueOf(tableName));
                admin.deleteTable(TableName.valueOf(tableName));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void testAdd(int a, int b) {
        String output = String.format("a: %d, b: %d, a+b=%d\n", a, b, a+b);
        System.out.println(output);
    }

    public static void main(String[] args) {
        HBaseUtils hbaseUtils = new HBaseUtils();
        hbaseUtils.hello();
        hbaseUtils.createTable("test_table_b", "col");

    }
}
