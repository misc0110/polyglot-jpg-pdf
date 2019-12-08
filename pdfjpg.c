#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if(argc != 4) {
        printf("Usage: %s <PDF file> <JPG file> <output>\n", argv[0]);
        return 1;
    }

    char buffer[4096];
    FILE *pdf = NULL, *jpg = NULL, *out = NULL;
    unsigned char *jpg_buf = NULL, *pdf_buf = NULL;
    size_t size = 0;

    pdf = fopen(argv[1], "rb");
    if(!pdf) {
        fprintf(stderr, "Could not open PDF '%s'\n", argv[1]);
        goto error;
    }

    jpg = fopen(argv[2], "rb");
    if(!jpg) {
        fprintf(stderr, "Could not open JPG '%s'\n", argv[2]);
        goto error;
    }

    out = fopen(argv[3], "wb");
    if(!out) {
        fprintf(stderr, "Could not open output file '%s'\n", argv[3]);
        goto error;
    }

    // header
    if(fread(buffer, 1, 0x14, jpg) != 0x14) {
        fprintf(stderr, "Could not read JPG header\n");
        goto error;
    }
    if(fwrite(buffer, 1, 0x14, out) != 0x14) {
        fprintf(stderr, "Could not write JPG header\n");
        goto error;
    }
    
    // get jpg data
    fseek(jpg, 0, SEEK_END);
    size = ftell(jpg);
    fseek(jpg, 0, SEEK_SET);
    
    if(!size) {
        fprintf(stderr, "Invalid JPG\n");
        goto error;
    }

    jpg_buf = (unsigned char*)malloc(size);
    if(!jpg_buf) {
        fprintf(stderr, "Could not allocate memory\n");
        goto error;
    }
    if(fread(jpg_buf, 1, size, jpg) != size) {
        fprintf(stderr, "Could not read JPG file\n");
        goto error;
    }
    
    // write magic
    if(fwrite("\xff\xfe\x00\x22\x0a%PDF-1.5\x0a""999 0 obj\x0a<<>>\x0astream\x0a", 1, 36, out) != 36) {
        fprintf(stderr, "Could not write PDF comment\n");
        goto error;
    }
    
    // write jpeg data
    for(size_t i = 0; i < size - 1; i++) {
        if(jpg_buf[i] == 0xff && jpg_buf[i + 1] == 0xdb) {
            if(fwrite(jpg_buf + i, 1, size - i, out) != size - i) {
                fprintf(stderr, "Could not write JPG data\n");
                goto error;
            }
            break;
        }
    }
    
    free(jpg_buf);
    jpg_buf = NULL;
    
    // close magic pdf object
    if(fwrite("endstream\x0aendobj\x0a", 1, 17, out) != 17) {
        fprintf(stderr, "Could not close PDF comment\n");
        goto error;
    }
    
    // get pdf data
    fseek(pdf, 0, SEEK_END);
    size = ftell(pdf);
    fseek(pdf, 0, SEEK_SET);
    pdf_buf = (unsigned char*)malloc(size);
    if(!pdf_buf) {
        fprintf(stderr, "Could not allocate memory for PDF\n");
        goto error;
    }
    if(fread(pdf_buf, 1, size, pdf) != size) {
        fprintf(stderr, "Could not read PDF data\n");
        goto error;
    }
    
    // write pdf data
    if(fwrite(pdf_buf, 1, size, out) != size) {
        fprintf(stderr, "Could not write PDF data\n");
        goto error;
    }
    free(pdf_buf);
    pdf_buf = NULL;
    
    fclose(pdf);
    pdf = NULL;
    fclose(jpg);
    jpg = NULL;
    fclose(out);
    out = NULL;
    return 0;

error:
    if(pdf) fclose(pdf);
    if(jpg) fclose(jpg);
    if(out) fclose(out);
    if(jpg_buf) free(jpg_buf);
    return 1;
}
