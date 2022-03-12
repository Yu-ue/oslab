#include <elf.h>
#include <stdio.h>
#include <string.h>
char strtable[999];
int main(int argc, char **argv)
{
    char *f1 = argv[1];
    FILE *fp = fopen(f1, "r");
    if (fp == NULL)
    {
        printf("File %s can't open", f1);
        return -1;
    }
    Elf32_Ehdr ehdr; //
    fseek(fp, 0, SEEK_SET);
    fread(&ehdr, sizeof(Elf32_Ehdr), 1, fp);
    Elf32_Shdr shdr[99];
    int count = ehdr.e_shnum; //节头表数量
    fseek(fp, ehdr.e_shoff, SEEK_SET);
    fread(shdr, sizeof(Elf32_Shdr), count, fp);
    fseek(fp, shdr[ehdr.e_shstrndx].sh_offset, SEEK_SET);
    fread(strtable, 1, shdr[ehdr.e_shstrndx].sh_size, fp);
    char s[6] = ".text";
    for (int i = 0; i < count; ++i)
    {
        if (!strcmp(&strtable[shdr[i].sh_name], s))
        {
            char text[512] = {'\0'};
            fseek(fp, shdr[i].sh_offset, SEEK_SET);
            fread(&text, shdr[i].sh_size, 1, fp);
            text[510] = 0x55;
            text[511] = 0xAA;
	    //printf("%s",text);
            fclose(fp);
            char *s = strchr(f1, '.');
            char filename[4] = "bin";
	    char* b = filename;
            while (*s != '\0')
                *(++s) = *(b++);
            //printf("bin file:%s\n", f1);
            FILE *bin = fopen(f1, "wb");
            if (bin == NULL)
            {
                printf("File %s can't open", f1);
                return -1;
            }
            fwrite(&text,sizeof(char),512,bin);
            fclose(bin);
            return 0;
        }
    }
    printf("error, there is no .text in %s", argv[1]);
    fclose(fp);
    return 0;
}

