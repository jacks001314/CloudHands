package com.antell.cloudhands.api.geoip;


import com.antell.cloudhands.api.utils.JedisPoolUtil;
import com.maxmind.geoip2.DatabaseReader;
import com.maxmind.geoip2.model.CityResponse;
import com.maxmind.geoip2.record.City;
import com.maxmind.geoip2.record.Country;
import com.maxmind.geoip2.record.Location;
import org.apache.kafka.common.protocol.types.Field;

import java.io.File;
import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Map;

import redis.clients.jedis.Jedis;


/**
 * Created by dell on 2018/6/26.
 */
public class MaxmindGEOIPDataBase implements GEOIPDataBase{

    private DatabaseReader databaseReader;

    @Override
    public void open(String dbPath) throws GEOIPException {


        try {
            File database = new File(dbPath);
            databaseReader = new DatabaseReader.Builder(database).build();
        }catch (Exception e){

            throw new GEOIPException(e.getMessage());
        }

    }

    private boolean internalIp(String ip) {
        InetAddress address = null;
        try {
            address = InetAddress.getByName(ip);
        } catch (Exception e) {
            return false;
        }
        if (address.isSiteLocalAddress()) {
            return true;
        }

        return false;

    }


    @Override
    public GEOIPItem query(String ip) {

        GEOIPItem item = new GEOIPItem();

        try {

            if (internalIp(ip)) {
                Jedis jedisClient = null;
                jedisClient = (JedisPoolUtil.getJedisPool()).getResource();

                String redisKeyAssetsCoordinateHash = "load_config_to_redis_assets_coordinate.hash";
                Map map = jedisClient.hgetAll(redisKeyAssetsCoordinateHash);
                if (map.containsKey("country")){
                    item.setCountry((String)map.get("country"));
                }

                if (map.containsKey("location")){
                    item.setLocation((String)map.get("location"));
                }

                if (map.containsKey("city")){
                    item.setCity((String)map.get("city"));
                }

                if (map.containsKey("latitude")){
                    item.setLatitude(Double.valueOf((String) map.get("latitude")) );
                }

                if (map.containsKey("longitude")){
                    item.setLongitude(Double.valueOf((String) map.get("longitude")) );
                }

                jedisClient.close();

                return item;

            }
        }catch (Exception e){
            return item;

        }


        if (databaseReader == null) {

            return item;
        }

        InetAddress ipAddress = null;
        try {
            ipAddress = InetAddress.getByName(ip);
        } catch (UnknownHostException e) {

            e.printStackTrace();
            return item;
        }



        CityResponse response = null;
        try {
            response = databaseReader.city(ipAddress);
        } catch(Exception e){

            //e.printStackTrace();
            //logger.error(e.getMessage());
            return item;
        }


        Country country = response.getCountry();
        Location location = response.getLocation();
        City city = response.getCity();

        String isoCode = country.getIsoCode();
        if (isoCode == null) {
            return item;
        }

        if (isoCode.equalsIgnoreCase("TW")) {

            item.setLocation("CN");
            item.setCountry("中国");
        } if (isoCode.equalsIgnoreCase("KOR")) {

            item.setLocation("KOR");
            item.setCountry("韩国");
        } else {

            item.setLocation(isoCode);
            item.setCountry(country.getNames().get("zh-CN"));
        }

        item.setCity(city.getNames().get("zh-CN"));

        item.setLatitude(location.getLatitude());
        item.setLongitude(location.getLongitude());

        return item;

    }

    @Override
    public void close() {

        try {
            databaseReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
