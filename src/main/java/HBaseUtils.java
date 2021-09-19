import org.apache.hadoop.hbase.Cell;
import org.apache.hadoop.hbase.CellUtil;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.client.*;
import org.apache.hadoop.hbase.util.Bytes;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class HBaseUtils {
    private static final Logger logger = LoggerFactory.getLogger(HBaseUtils.class);

    public static boolean isExists(String tableName) {
        boolean tableExists = false;
        try(Admin admin = HBaseConfig.getConnection().getAdmin()) {
            tableExists = admin.tableExists(TableName.valueOf(tableName));
        } catch (IOException e) {
            e.printStackTrace();
        }
        return tableExists;
    }

    /**
     * 创建表
     * @param connection
     * @param tableName
     * @param columnFamilies
     * @throws IOException
     */
    public static void createTable(Connection connection, TableName tableName, String... columnFamilies) {
        try(Admin admin = connection.getAdmin()) {
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

//        Admin admin = null;
//        try {
//            admin = connection.getAdmin();
//            if (admin.tableExists(tableName)) {
//                logger.warn("table:{} exists!", tableName.getName());
//            } else {
//                TableDescriptorBuilder builder = TableDescriptorBuilder.newBuilder(tableName);
//                for (String columnFamily : columnFamilies) {
//                    builder.setColumnFamily(ColumnFamilyDescriptorBuilder.of(columnFamily));
//                }
//                admin.createTable(builder.build());
//                logger.info("create table:{} success!", tableName.getName());
//            }
//        } finally {
//            if (admin != null) {
//                admin.close();
//            }
//        }
    }

    public static void put(Connection connection, String tableName, String rowKey, String family, String column,
                           long ts, String data) {
        try(Table table = connection.getTable(TableName.valueOf(tableName))) {
//            Table table = connection.getTable(TableName.valueOf(tableName));
            Put put = new Put(Bytes.toBytes(tableName));
            put.addColumn(Bytes.toBytes(family), Bytes.toBytes(column), ts, Bytes.toBytes(data));
            table.put(put);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static List<String> get(Connection connection, String tableName, String rowKey, String family, String column,
                                   long ts, String data) {
        List<String> list = new ArrayList<>();
        try(Table table = connection.getTable(TableName.valueOf(tableName))) {
            Get get = new Get(Bytes.toBytes(rowKey));
            get.addColumn(Bytes.toBytes(family), Bytes.toBytes(column));
            get.setTimestamp(ts);

            Result result = table.get(get);
            List<Cell> cells = result.listCells();
            for (Cell c: cells) {
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
        try(Table table = connection.getTable(TableName.valueOf(tableName))) {
            Get get = new Get(Bytes.toBytes(rowKey));
            get.addColumn(Bytes.toBytes(family), Bytes.toBytes(column));
            get.setTimeRange(minTs, maxTs);

            Result result = table.get(get);
            List<Cell> cells = result.listCells();
            for (Cell c: cells) {
                String sValue = Bytes.toString(CellUtil.cloneValue(c));
                list.add(sValue);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return list;
    }

    public static void deleteTable(Connection connection, String tableName) {
        try(Admin admin = connection.getAdmin()) {
            if (admin.tableExists(TableName.valueOf(tableName))) {
                admin.disableTable(TableName.valueOf(tableName));
                admin.deleteTable(TableName.valueOf(tableName));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    public static void main(String[] args) {
//        System.out.println(new Path(ClassLoader.getSystemResource("hbase-site.xml").toURI()));
        HBaseConfig.getConnection();
    }
}
