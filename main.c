#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>

void list_trash(void);
void send_to_trash(char *);
void create_info(char *);

int main(int argc, char **argv)
{
	if (argc <= 1) {
		list_trash();

		return 0;
	}

	for (int i = 1; i < argc; i++) {
		send_to_trash(argv[i]);
		create_info(argv[i]);
	}
}

void list_trash(void)
{
	DIR *d;
	struct dirent *dir;

	char trash_path[64];
	snprintf(trash_path, sizeof(trash_path),
		 "/home/%s/.local/share/Trash/files/", getlogin());

	d = opendir(trash_path);

	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_type == DT_REG) {
				printf("%s\n", dir->d_name);
			}
		}
		closedir(d);
	}
}

void send_to_trash(char *filename)
{
	char new_path[64];
	snprintf(new_path, sizeof(new_path),
		 "/home/%s/.local/share/Trash/files/%s", getlogin(), filename);

	rename(filename, new_path);
}

void create_info(char *filename)
{
	char info_path[64];
	snprintf(info_path, sizeof(info_path),
		 "/home/%s/.local/share/Trash/info/%s.trashinfo", getlogin(),
		 filename);

	FILE *fp = fopen(info_path, "w+");

	char absolute_path[64];
	realpath(filename, absolute_path);

	char date[64];
	time_t raw_time;
	struct tm *timeinfo;

	time(&raw_time);
	timeinfo = localtime(&raw_time);

	snprintf(date, sizeof(date), "%04d-%02d-%02dT%02d:%02d:%02d",
		 timeinfo->tm_year + 1900, timeinfo->tm_mon + 1,
		 timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min,
		 timeinfo->tm_sec);

	fprintf(fp, "[Trash Info]\nPath=%s\nDeletionDate=%s\n", absolute_path,
		date);

	fclose(fp);
}
