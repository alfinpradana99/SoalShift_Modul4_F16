#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/hisam/shift4";
char chiper[]="qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
//char ssdh[]="zi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0qE1~ YMUR2\"`hNIdP";

void dekripsi(char* answer)
{
    	int i;
	int key=17;
    	for(i=0; i<strlen(answer); i++){
        	char *pch=strchr(chiper, answer[i]);
        	if(pch==NULL) continue;
        	else
        	{   
            	//dekripsi
            		if((pch-chiper-key)<0) pch=chiper+((pch-chiper-key+strlen(chiper))%strlen(chiper));
            		else pch=chiper+((pch-chiper-key)%strlen(chiper));

            		answer[i]=*pch;
        	}
    	}
    	answer[strlen(answer)] = '\0';
}

void enkripsi(char* answer)
{
    	int i;
	int key=17;
    	for(i=0; i<strlen(answer); i++){
        	char *pch=strchr(chiper, answer[i]);
        	if(pch==NULL) continue;
        	else
        	{    
           		//enkripsi
            		pch=chiper+(pch-chiper+key)%strlen(chiper);

            		answer[i]=*pch;
        	}
    	}
    	answer[strlen(answer)] = '\0';
}

static int xmp_mkdir(const char *path, mode_t mode)
{

	int res;
    	char fpath[1000];
    	char name[1000];
	sprintf(name,"%s",path);
	if(strlen(name)>9 && strncmp(name,"/YOUTUBER",9)==0)
	{
		enkripsi(name);
		sprintf(fpath, "%s%s",dirpath,name);
		res = mkdir(fpath, 0750);	
	}
	else{
    		enkripsi(name);
		sprintf(fpath, "%s%s",dirpath,name);
		res = mkdir(fpath, mode);
		if (res == -1)
			return -errno;
	}
	return 0;

}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) {
    
    	if (strstr(path,"/YOUTUBER")) 
    	{
		mode = 0640;
        	strcat(path, ".iz1");
    	}

    	char fpath[1000];
    	enkripsi(path);
    	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
    	else sprintf(fpath, "%s%s",dirpath,path);

    	(void) fi;

    	int res;
    	res = creat(fpath, mode);
    	if(res == -1)
		return -errno;

    	close(res);

    	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	int len = strlen(path);
    	const char *ext = &path[len-4];

    	if (len >= 4 && strcmp(ext,".iz1") == 0) {
		pid_t child_id;
		child_id = fork();
		if (child_id == 0) {
			char *param_alert[] = {"zenity","--error","--title=\"Pesan error\"","--text=\"File ekstensi iz1 tidak boleh diubah permissionnya.\n\"",NULL};
			execv("/usr/bin/zenity",param_alert);
			return 0;
		}
		return 0;
    	}

    	char fpath[1000];
    	enkripsi(path);
    	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
    	else sprintf(fpath, "%s%s",dirpath,path);

	int res;

	res = chmod(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
			 off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
    	enkripsi(path);
    	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
    	else sprintf(fpath, "%s%s",dirpath,path);
    
    	int fd;
	int res;

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
    	close(fd);
	if (res == -1)
		res = -errno;

    	return res; 
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
    	char fpath[1000];
    	enkripsi(path);
    	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
    	else sprintf(fpath, "%s%s",dirpath,path);

	int res;
	struct timeval tv[2];

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fpath, tv);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_access(const char *path, int mask)
{
        int res;

        res = access(path, mask);
        if (res == -1)
                return -errno;

        return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
        int res;

        res = open(path, fi->flags);
        if (res == -1)
                return -errno;

        close(res);
        return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;
	char fpath[1000], waktu[100];
	enkripsi(path);
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	res = lstat(fpath, stbuf);
	
	int flagread;
        char user[100], group[100];
        sprintf(fpath,"%s%s",dirpath,path);
        struct passwd *pw = getpwuid(stbuf->st_uid);
        struct group  *gr = getgrgid(stbuf->st_gid);
        if(stbuf->st_mode & S_IRUSR && stbuf->st_mode & S_IRGRP)
                flagread=1;
        else
                flagread=0;
        strcpy(user,pw->pw_name);
        strcpy(group,gr->gr_name);
        strftime(waktu, 50, "%Y-%m-%d %H:%M:%S", localtime(&stbuf->st_mtime));
        //printf("ini file, nama sama grup : %s %s %s\n",path,user,group);
        //printf("ini waktu %s\n",waktu);

        if(strcmp(user,"chipset")==0 || strcmp(user,"ic_controller")==0 /*&& flagread==0*/)
	{
	if(strcmp(group,"rusak")==0)
  		{
                	FILE *file;
                	char isian[1000], tempat[100];
                	memset(isian, 0, sizeof(isian));
                	sprintf(isian, "%s %s %s %s\n", path, user, group, waktu);
			sprintf(tempat, "%s/filemiris.txt",dirpath);
                	file = fopen(tempat, "a");
                	fprintf(file, "%s", isian);
			fclose(file);

                	//printf("jancok %s\n",path);
			remove(path);

        	}
	}
	
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	
	char fpath[1000];
	enkripsi(path);
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		dekripsi(de->d_name);
		res = (filler(buf, de->d_name, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	char fpath[1000];
	enkripsi(path);
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;
  	int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.mkdir		= xmp_mkdir,
	.create		= xmp_create,
	.chmod		= xmp_chmod,
	.write		= xmp_write,
	.utimens	= xmp_utimens,
	.access		= xmp_access,
	.open		= xmp_open,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
