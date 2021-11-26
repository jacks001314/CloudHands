#ifndef CH_PACKET_API_READER_H
#define CH_PACKET_API_READER_H

#include <stdint.h>

typedef struct packet_entry_t packet_entry_t;


struct packet_entry_t {

    uint8_t ptype;
    uint16_t metaDataSize;
    uint64_t time;
    uint64_t dataSize;
    void * data;
};

/*open mmap file to read
 *
 * @fid indentify a mmap file
 * @fpath the file full path to be read
 * return:
 * if open failed will return -1 ,otherwise return 0
 * */
extern int  packet_mmap_open(int fid,const char *fpath);


/*open share memory to read
 *
 * @sid indentify a share memory
 * @key the share memory key  to be read
 * @proj_id the share memory proj_id  to be read
 * return:
 * if open failed will return -1 ,otherwise return 0
 * */
extern int  packet_shm_open(int sid,const char *key,int proj_id);


/*read packet entry from mmap/shm 
 *
 * @id the mmap or shm id
 *
 * return:
 * if ok return the pointer of packet entry instance,otherwise return NULL
 * */

extern packet_entry_t* packet_entry_read(int id);

/*close */

extern void packet_close(int id);


#endif /*CH_PACKET_API_READER_H*/
