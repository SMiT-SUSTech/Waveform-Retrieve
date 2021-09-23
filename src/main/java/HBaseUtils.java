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

import java.io.IOException;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.List;

public class HBaseUtils {
    private static final Logger logger = LoggerFactory.getLogger(HBaseUtils.class);
    private static Configuration config;
    private static Connection connection;

    public HBaseUtils() {
        config = HBaseConfiguration.create();
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

        try (Connection connection = ConnectionFactory.createConnection(config)) {
            logger.info("Build Connection");
            System.out.println("Build Connection");
            System.out.println(connection);
        } catch (IOException e) {
            e.printStackTrace();
        }
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

    /**
     * 建立连接
     *
     * @return Connection
     */
    public static Connection getConnection() {
        //获取配置
        Configuration config = getConfiguration();

        try (Connection connection = ConnectionFactory.createConnection(config)) {
            logger.info("Build Connection");
            System.out.println("Build Connection");
            System.out.println(connection);
            return connection;
        } catch (IOException e) {
            e.printStackTrace();
        }

        return null;
    }

    public static boolean isExists(String tableName) {
        boolean tableExists = false;
        try (Admin admin = HBaseConfig.getConnection().getAdmin()) {
            tableExists = admin.tableExists(TableName.valueOf(tableName));
        } catch (IOException e) {
            e.printStackTrace();
        }
        return tableExists;
    }

    /**
     * 创建表
     *
     * @param connection
     * @param tableName
     * @param columnFamilies
     * @throws IOException
     */
    public static void createTable(TableName tableName, String... columnFamilies) {
        try (Admin admin = getConnection().getAdmin()) {
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
                           long ts, String data) {
        try (Table table = getConnection().getTable(TableName.valueOf(tableName))) {
            Put put = new Put(Bytes.toBytes(tableName));
            put.addColumn(Bytes.toBytes(family), Bytes.toBytes(column), ts, Bytes.toBytes(data));
            table.put(put);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    public static void put(Connection connection, String tableName, String rowKey, String family, String column,
                           long ts, String data) {
        try (Table table = connection.getTable(TableName.valueOf(tableName))) {
            Put put = new Put(Bytes.toBytes(tableName));
            put.addColumn(Bytes.toBytes(family), Bytes.toBytes(column), ts, Bytes.toBytes(data));
            table.put(put);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static List<String> get(String tableName, String rowKey, String family, String column,
                                   long ts, String data) {
        List<String> list = new ArrayList<>();
        try (Table table = getConnection().getTable(TableName.valueOf(tableName))) {
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
        try (Table table = getConnection().getTable(TableName.valueOf(tableName))) {
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
        try (Admin admin = getConnection().getAdmin()) {
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

//    public static void main(String[] args) {
//        HBaseConfig.getConnection();
//    }
}
