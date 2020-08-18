
#include <pcap.h>
#include <time.h>

#define MAX_FILE_NAME 1024

static int is_endsWith(const char *target,const char *match){
 
     size_t match_length;
     size_t target_length;
 
     if(target == NULL||match == NULL)
         return 0;
 
     match_length = strlen(match);
     target_length = strlen(target);
 
     if(match_length == 0||target_length == 0)
         return 0;
 
     /* This is impossible to match */
     if (match_length > target_length) {
         /* No match. */
         return 0;
     }
 
     if (memcmp(match, (target + (target_length - match_length)), match_length) == 0) {
         return 1;
     }
 
     /* No match. */
     return 0;
 
 }


static int is_valid_pcap_fname(const char *root_dir,const char *name){
    
    return (is_endsWith(name,".pcap")||is_endsWith(name,".pcapng"))&&((strlen(root_dir)+strlen(name)+1)<PATH_MAX); 
}

static size_t format_pcap_files(const char *root_dir){

    char buff[MAX_FILE_NAME] = {0};

}

int main(int argc,char **argv){


}

