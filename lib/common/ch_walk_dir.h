/*shajf*/

#ifndef CH_WALK_DIR_H
#define CH_WALK_DIR_H

#include <dirent.h>
#include <sys/stat.h>

/* Some useful definitions */
#undef CH_FALSE
#define CH_FALSE   ((int) 0)
#undef CH_TRUE
#define CH_TRUE    ((int) 1)
#undef CH_SKIP
#define CH_SKIP	((int) 2)

enum {
	CH_ACTION_RECURSE        = (1 << 0),
	CH_ACTION_FOLLOWLINKS    = (1 << 1),
	CH_ACTION_FOLLOWLINKS_L0 = (1 << 2),
	CH_ACTION_DEPTHFIRST     = (1 << 3),
	CH_ACTION_QUIET          = (1 << 5),
};

/* fileAction return value of 0 on any file in directory will make
 * ch_walk_dir() return 0, but it doesn't stop directory traversal
 * (fileAction/dirAction will be called on each file).
 *
 * If !CH_ACTION_RECURSE, dirAction is called on the directory and its
 * return value is returned from ch_walk_dir(). No recursion.
 *
 * If CH_ACTION_RECURSE, ch_walk_dir() is called on each directory.
 * If any one of these calls returns 0, current ch_walk_dir() returns 0.
 *
 * If CH_ACTION_DEPTHFIRST, dirAction is called after recurse.
 * If it returns 0, the warning is printed and ch_walk_dir() returns 0.
 *
 * If !CH_ACTION_DEPTHFIRST, dirAction is called before we recurse.
 * Return value of 0 (FALSE) or 2 (SKIP) prevents recursion
 * into that directory, instead ch_walk_dir() returns 0 (if FALSE)
 * or 1 (if SKIP)
 *
 * followLinks=0/1 differs mainly in handling of links to dirs.
 * 0: lstat(statbuf). Calls fileAction on link name even if points to dir.
 * 1: stat(statbuf). Calls dirAction and optionally recurse on link to dir.
 */

int ch_walk_dir(const char *fileName,
		unsigned flags,
		int  (*fileAction)(const char *fileName, struct stat *statbuf, void* userData, int depth),
		int  (*dirAction)(const char *fileName, struct stat *statbuf, void* userData, int depth),
		int  (*acceptAction)(const char *fileName),
        void* userData,
		unsigned depth);

#endif /*CH_WALK_DIR_H*/
