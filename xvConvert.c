#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>


// reference: http://www.cnblogs.com/lsjwq/archive/2011/05/15/2046716.html

int XV_Convert(char* src_file) {
    char *fb, peek[5];
    int mn, type, l, rl, c, fail;
    char *ts=NULL;
    char *video_type[7] = {"rmvb", "wmv", "flv", "avi", "mp4", "mpg", "mkv"};
    char val[7]={46, 48, 70, 82, 0, 0, 26};
    int j;
    FILE *fin, *fout;
    char dst_file[255]={0};

    fb = (char*)malloc(sizeof(char) * 0x200011);
    c = 0;

    fin = fopen(src_file, "rb");
    if (fin==NULL) return -1;
    fseek(fin, 0, SEEK_END);
    l = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    fread(fb, 1, 0x200000, fin);
    fread(peek, 1, 4, fin);

    mn = ('R'-peek[1] + 0x100) & 0xFF;
    if (((peek[2]+mn) & 0xFF) == 'M' && ((peek[3]+mn) & 0xFF ) == 'F') {
        type = 1;
        goto ok;
    }
    mn = (0x26-peek[1] + 0x100) & 0xFF;
    if (((peek[2]+mn) & 0xFF) == 0xB2 && ((peek[3]+mn) & 0xFF) == 0x75) {
        type = 2;
        goto ok;
    }
    mn = ('L'-peek[1] + 0x100) & 0xFF;
    if (((peek[2]+mn) & 0xFF) == 'V') {
        type = 3;
        goto ok;
    }
    mn = ('I'-peek[1] + 0x100) & 0xFF;
    if (((peek[2]+mn) & 0xFF) == 'F' && ((peek[3]+mn) & 0xFF) == 'F') {
        type = 4;
        goto ok;
    }
    mn = (-peek[1] + 0x100) & 0xFF;
    if (((peek[2]+mn) & 0xFF) == 0) {
        type = 5;
        goto ok;
    }
    mn = (-peek[1] + 0x100) & 0xFF;
    if (((peek[2]+mn) & 0xFF) == 1 && ((peek[3]+mn) & 0xFF) == 0xBA) {
        type = 6;
        goto ok;
    }
    mn = (0x45-peek[1] + 0x100) & 0xFF;
    if (((peek[2]+mn) & 0xFF) == 0xDF && ((peek[3]+mn) & 0xFF) == 0xA3) {
        type = 7;
        goto ok;
    }

    strcpy(dst_file, src_file);
    strcat(dst_file, ".new");
    fout = fopen(dst_file, "wb");
    if (fout==NULL) { fclose(fin); return -2; }
    fwrite(fb, 0, 0x200000, fout);
    fwrite(fb, 0, 4, fout);

    fread(fb, 0, 0x100000, fin);
    fwrite(fb, 0, 0x100000, fout);
    fail += 1;
    fclose(fin);
    fclose(fout);
    printf("convert failed\n");
    free(fb);
    return -1;

ok:
    ts = video_type[type-1];
    strcpy(dst_file, src_file);
    strcat(dst_file, ".");
    strcat(dst_file, ts);
    fout = fopen(dst_file, "wb");
    peek[0] = val[type-1];
    for (j=1; j<=3; j++) {
        peek[j] = (peek[j]+mn) & 0xFF;
    }
    fwrite(peek, 1, 4, fout);
    fread(fb, 1, 0x3FC, fin);
    for (j=1; j<=0x3FC; j++) {
        fb[j-1] = (fb[j-1]+mn) & 0xFF;
    }
    fwrite(fb, 1, 0x3FC, fout);
    printf("go to ");
    while (ftell(fin) <= l ) {

        rl = fread(fb, 1, 0x40000, fin);
        if (rl == 0) break;;
        fwrite(fb, 1, rl, fout);
        c = c + 1;
        if (c == 10) {
            double pos = ftell(fout);
            pos = pos / (l-0x200000);
            pos = pos * 100;
            printf("%d ", (int)pos);
            c = 0;
        }
    }
    printf("100\n");
    fclose(fin);
    fclose(fout);
    free(fb);
    
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("usage: convert xunlei xv video format to other format.\n");
        printf("    %s xv_filename\n", argv[0]);
        return 0;
    }

    return XV_Convert(argv[1]);
}
