#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

// reference: http://www.cnblogs.com/lsjwq/archive/2011/05/15/2046716.html

#define VIDEO_TYPE_NUM (7)
#define FILENAME_MAX_LEN (255)
#define SKIP_HEAD_LEN (0x200000)
#define REPORT_DATA_LEN (0x100000)
#define BUFFER_LEN (0x40000)

int XV_Convert(char* src_file) {
    char *fb = NULL, peek[5];
    int mn = 0, type = 0, l = 0, c = 0, fail = 0;
	size_t rl = 0;
    char *ts = NULL;
    char *video_type[VIDEO_TYPE_NUM] = {"rmvb", "wmv", "flv", "avi", "mp4", "mpg", "mkv"};
    char val[VIDEO_TYPE_NUM] = {46, 48, 70, 82, 0, 0, 26};
    int j = 0;
    FILE *fin = NULL, *fout = NULL;
    char dst_file[FILENAME_MAX_LEN] = {0};
    int translate_percentage = 0;

    fin = fopen(src_file, "rb");
    if (fin == NULL) {
        return -1;
    }

    fb = (char*)malloc(SKIP_HEAD_LEN);
    c = 0;

    fseek(fin, 0, SEEK_END);
    l = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    fread(fb, 1, SKIP_HEAD_LEN, fin);

    for (size_t byte_count = 0; byte_count < SKIP_HEAD_LEN; byte_count += sizeof(int)) {
        int* int_ptr = (int*)(fb + byte_count);
        if (*int_ptr != 0) {
            fseek(fin, 0, SEEK_SET);
            break;
        }
    }

    fread(peek, 1, 4, fin);

    mn = ('R' - peek[1] + 0x100) & 0xFF;
    if (((peek[2] + mn) & 0xFF) == 'M' && ((peek[3] + mn) & 0xFF ) == 'F') {
        type = 1;
        goto ok;
    }
    mn = (0x26 - peek[1] + 0x100) & 0xFF;
    if (((peek[2] + mn) & 0xFF) == 0xB2 && ((peek[3] + mn) & 0xFF) == 0x75) {
        type = 2;
        goto ok;
    }
    mn = ('L'- peek[1] + 0x100) & 0xFF;
    if (((peek[2] + mn) & 0xFF) == 'V') {
        type = 3;
        goto ok;
    }
    mn = ('I' - peek[1] + 0x100) & 0xFF;
    if (((peek[2] + mn) & 0xFF) == 'F' && ((peek[3] + mn) & 0xFF) == 'F') {
        type = 4;
        goto ok;
    }
    mn = (-peek[1] + 0x100) & 0xFF;
    if (((peek[2] + mn) & 0xFF) == 0) {
        type = 5;
        goto ok;
    }
    mn = (-peek[1] + 0x100) & 0xFF;
    if (((peek[2] + mn) & 0xFF) == 1 && ((peek[3] + mn) & 0xFF) == 0xBA) {
        type = 6;
        goto ok;
    }
    mn = (0x45 - peek[1] + 0x100) & 0xFF;
    if (((peek[2] + mn) & 0xFF) == 0xDF && ((peek[3] + mn) & 0xFF) == 0xA3) {
        type = 7;
        goto ok;
    }

//SEND_REPORT:
    strcpy(dst_file, src_file);
    strcat(dst_file, ".report");
    fout = fopen(dst_file, "wb");
    if (fout == NULL) {
        fclose(fin);
        return -2;
    }
    fwrite(fb, 0, SKIP_HEAD_LEN, fout);
    fwrite(peek, 0, 4, fout);

    fread(fb, 0, REPORT_DATA_LEN, fin);
    fwrite(fb, 0, REPORT_DATA_LEN, fout);
    fail += 1;
    fclose(fin);
    fclose(fout);
    fprintf(stderr, "convert failed, please send %s to bbbiao@gmail.com\n", dst_file);
    free(fb);
    return -1;

ok:
    ts = video_type[type - 1];
    strcpy(dst_file, src_file);
    strcat(dst_file, ".");
    strcat(dst_file, ts);
    fout = fopen(dst_file, "wb");
    peek[0] = val[type - 1];
    for (j = 1; j <= 3; j++) {
        peek[j] = (peek[j] + mn) & 0xFF;
    }
    fwrite(peek, 1, 4, fout);
    fread(fb, 1, 0x3FC, fin);
    for (j = 1; j <= 0x3FC; j++) {
        fb[j - 1] = (fb[j - 1]+mn) & 0xFF;
    }
    fwrite(fb, 1, 0x3FC, fout);
    while (ftell(fin) <= l ) {
        rl = fread(fb, 1, 0x40000, fin);
        if (rl == 0) break;;
        fwrite(fb, 1, rl, fout);
        c = c + 1;
        if (c == 10) {
            double pos = ftell(fout);
            pos = pos / (l - 0x200000);
            pos = pos * 100;
            if ((int)pos > translate_percentage) {
                printf("Converting %3d%%\n", (int)pos);
                translate_percentage = (int)pos;
            }
            c = 0;
        }
    }
    fclose(fin);
    fclose(fout);
    free(fb);

    return 0;
}

void usage(char *filename) {
	printf("usage: convert xunlei xv video format to other format.\n");
    printf("    %s xv_filename\n", filename);
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		usage(argv[0]);
		return 0;
	}

	return XV_Convert(argv[1]);

}

