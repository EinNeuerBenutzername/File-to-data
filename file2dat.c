#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static const char *strprbrk(const char *s,const char *charset){
    const char *latestMatch=NULL;
    for(;s=strpbrk(s, charset),s!=NULL;latestMatch=s++){}
    return latestMatch;
}

int main(int argc,char* argv[]){
    char filename[4096]={0},filename_noextension[4096]={0};
    FILE *fp=NULL,*fout=NULL;
    memset(filename,0,4096);
    memset(filename_noextension,0,4096);
    if(argc<=1){
        printf("File path: ");
        fgets(filename,4096,stdin);
        while(filename[0]&&(filename[strlen(filename)-1]=='\n'||filename[strlen(filename)-1]=='\r'))
            filename[strlen(filename)-1]=0;
    }else{
        strcpy(filename,argv[1]);
    }
    int i=2;
    while(1){
        if(strlen(filename)){
            fp=fopen(filename,"rb");
            if(!fp){
                if(argc<=1)return 0;
                i++;
                if(i>=argc)return 0;
                memset(filename,0,4096);
                strcpy(filename,argv[i]);
                continue;
            }
            {
                if(filename!=NULL)strcpy(filename_noextension,strprbrk(filename,"\\/")+1);
                int size=(int)strlen(filename_noextension);
                for(int i=0;(i<size)&&(i<4096);i++){
                    if(filename_noextension[i]=='.'){
                        filename_noextension[i]='\0';
                        break;
                    }
                }
            }
            {
                char fileout[4096]={0};
                memset(fileout,0,4096);
//                strcpy(fileout,filename_noextension);
                strcpy(fileout,filename);
                strcat(fileout,".txt");
                fout=fopen(fileout,"w");
                if(fout==NULL){
                    fclose(fp);
                    return 1;
                }
            }
            fseek(fp,0L,SEEK_END);
            long filesize=ftell(fp);
            rewind(fp);
            fprintf(fout,"const unsigned char %s[%ld]={",filename_noextension,filesize);
            for(long i=1;i<filesize;i++){
                unsigned char a=fgetc(fp);
                fprintf(fout,"%d,",a);
            }
            char a=fgetc(fp);
            fprintf(fout,"%d};",a);
            fclose(fp);
        }
        if(i<argc){
            memset(filename,0,4096);
            strcpy(filename,argv[i]);
            memset(filename_noextension,0,4096);
        }else return 0;
        i++;
    }
    return 1;
}

