package com.antell.cloudhands.api.utils;

import redis.clients.jedis.JedisPool;
import redis.clients.jedis.JedisPoolConfig;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.exceptions.JedisConnectionException;
import redis.clients.jedis.exceptions.JedisException;


public final class JedisPoolUtil {

    private static JedisPool JEDIS_POOL = null;

    private JedisPoolUtil() {}

    public static JedisPool getJedisPool() {
        if (JEDIS_POOL == null) {
            //synchronized(JedisPoolUtil.class)
            {
                if (JEDIS_POOL == null) {
                    JedisPoolConfig poolConfig = new JedisPoolConfig();
                    poolConfig.setMaxTotal(1000);
                    poolConfig.setMaxIdle(1000);
                    poolConfig.setMaxWaitMillis(1 * 1000);
                    poolConfig.setTestOnBorrow(false);

                    JEDIS_POOL = new JedisPool(poolConfig, "127.0.0.1", 6379, 100*1000, "AntellSec#2017");
                }
            }
        }

        return JEDIS_POOL;
    }

}


