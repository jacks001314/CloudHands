/*
 * shajf
 */
#include "ch_constants.h"
#include "ch_walk_dir.h"

#define DOT_OR_DOTDOT(s) ((s)[0] == '.' && (!(s)[1] || ((s)[1] == '.' && !(s)[2])))

static inline char* last_char_is(const char *s, int c)
{
     if (s && *s) {
         size_t sz = strlen(s) - 1;
         s += sz;
         if ( (unsigned char)*s == c)
             return (char*)s;
     }
     return NULL;
}


static int true_action(const char *fileName ch_unused ,
		struct stat *statbuf ch_unused ,
		void* userData ch_unused,
		int depth ch_unused)
{
	return CH_TRUE;
}

static char *concat_subpath_file(const char *path,const char *f){
     char *result;    
     char *lc;
     size_t len = 0;

     if (f && DOT_OR_DOTDOT(f))
         return NULL;
    
    if (!path)
        path = "";
    lc = last_char_is(path, '/');
    while (*f == '/')
        f++;
    len = strlen(path)+strlen(f)+(lc==NULL?1:0);
    result = (char*)calloc(1,len+1);
    
    if(!result)
        return NULL;

    snprintf(result,len+1,"%s%s%s", path, (lc==NULL ? "/" : ""), f);

    return result;
}

int  ch_walk_dir(const char *fileName,
		unsigned flags,
		int (*fileAction)(const char *fileName, struct stat *statbuf, void* userData, int depth),
		int (*dirAction)(const char *fileName, struct stat *statbuf, void* userData, int depth),
        int (*acceptAction)(const char *fileName),
		void* userData,
		unsigned depth)
{
	struct stat statbuf;
	int status;
	DIR *dir;
	struct dirent *next;

	if (!fileAction) fileAction = true_action;
	if (!dirAction) dirAction = true_action;

	status = CH_ACTION_FOLLOWLINKS; /* hijack a variable for bitmask... */
	if (!depth)
		status = CH_ACTION_FOLLOWLINKS | CH_ACTION_FOLLOWLINKS_L0;
	status = ((flags & status) ? stat : lstat)(fileName, &statbuf);
	if (status < 0) {
		printf("status=%d flags=%x\n", status, flags);
		goto done_nak_warn;
	}

	/* If S_ISLNK(m), then we know that !S_ISDIR(m).
	 * Then we can skip checking first part: if it is true, then
	 * (!dir) is also true! */
	if ( /* (!(flags & CH_ACTION_FOLLOWLINKS) && S_ISLNK(statbuf.st_mode)) || */
	 !S_ISDIR(statbuf.st_mode)
	) {
		return fileAction(fileName, &statbuf, userData, depth);
	}

	/* It's a directory (or a link to one, and followLinks is set) */

	if (!(flags & CH_ACTION_RECURSE)) {
		return dirAction(fileName, &statbuf, userData, depth);
	}

	if (!(flags & CH_ACTION_DEPTHFIRST)) {
		status = dirAction(fileName, &statbuf, userData, depth);
		if (!status)
			goto done_nak_warn;
		if (status == CH_SKIP)
			return CH_TRUE;
	}

	dir = opendir(fileName);
	if (!dir) {
		goto done_nak_warn;
	}
	status = CH_TRUE;
	while ((next = readdir(dir)) != NULL) {
		char *nextFile;

        if(acceptAction){
        
            if(!acceptAction(next->d_name)){
                continue;
            }
        }

		nextFile = concat_subpath_file(fileName, next->d_name);
		if (nextFile == NULL)
			continue;
		/* process every file (NB: CH_ACTION_RECURSE is set in flags) */
		if (!ch_walk_dir(nextFile, flags, fileAction, dirAction,acceptAction,
						userData, depth + 1))
			status = CH_FALSE;
//		s = recursive_action(nextFile, flags, fileAction, dirAction,
//						userData, depth + 1);
		free(nextFile);
	}
	closedir(dir);

	if (flags & CH_ACTION_DEPTHFIRST) {
		if (!dirAction(fileName, &statbuf, userData, depth))
			goto done_nak_warn;
	}

	return status;

 done_nak_warn:
	if (!(flags & CH_ACTION_QUIET))
		printf("%s\n",fileName);
	return CH_FALSE;
}
