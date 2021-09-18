import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.client.Connection;
import org.apache.hadoop.hbase.client.ConnectionFactory;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.net.URISyntaxException;

public class HBaseConfig {
    private static final Logger logger = LoggerFactory.getLogger(HBaseConfig.class);
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
//       Properties props = PropertiesLoaderUtils.loadAllProperties("hbase.properties");
//       String clientPort = props.getProperty("hbase.zookeeper.property.clientPort");
//       String quorum = props.getProperty("hbase.zookeeper.quorum");
//
//       logger.info("connect to zookeeper {}:{}", quorum, clientPort);

//       Configuration config = HBaseConfiguration.create();
//       String path = this.getClass().getClassLoader().getResource("hbase-site.xml").getPath();
//       config.addResource(path);
//       config.set("hbase.zookeeper.property.clientPort", clientPort);
//       config.set("hbase.zookeeper.quorum", quorum);
//       return config;
    }

    /**
     * 建立连接
     *
     * @return Connection
     */
    public static Connection getConnection() {
        //获取配置
        Configuration config = getConfiguration();

        try(Connection connection = ConnectionFactory.createConnection(config)) {
            logger.info("Build Connection");
            System.out.println("Build Connection");
            System.out.println(connection);
            return connection;
        } catch (IOException e) {
            e.printStackTrace();
        }
//        try {
//            connection = ConnectionFactory.createConnection(config);
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
//        return connection;
        return null;
    }
}
