void updateNilai(struct Mahasiswa mhsBaru) {
    FILE *fpRead = fopen(FILE_NILAI, "r");
    FILE *fpTemp = fopen(FILE_TEMP, "w");
    char buffer[256];
    int ditemukan = 0;

    if (fpRead == NULL) {
        fprintf(fpTemp, "%s|%s|%d|%.2f|%.2f|%.2f|%.2f|%.2f|%c|%s\n",
            mhsBaru.nim, mhsBaru.nama, mhsBaru.jumlahKehadiran,
            mhsBaru.nilaiPresensi, mhsBaru.nilaiKeaktifan,
            mhsBaru.nilaiUTS, mhsBaru.nilaiUAS,
            mhsBaru.nilaiAkhir,mhsBaru.grade, mhsBaru.status);
            fclose(fpTemp);
            rename(FILE_TEMP, FILE_NILAI);
            return;
    }

    while (fgets(buffer, sizeof(buffer), fpRead)) {
        char temp[256];
        strcpy(temp, buffer);
        char *token = strtok(temp, "|");

        if (token && strcmp(token, mhsBaru.nim) == 0) {
            fprintf(fpTemp, "%s|%s|%d|%.2f|%.2f|%.2f|%.2f|%.2f|%c|%s\n",
                mhsBaru.nim, mhsBaru.nama, mhsBaru.jumlahKehadiran,
                mhsBaru.nilaiPresensi, mhsBaru.nilaiKeaktifan,
                mhsBaru.nilaiUTS, mhsBaru.nilaiUAS,
                mhsBaru.nilaiAkhir,mhsBaru.grade, mhsBaru.status);
            ditemukan = 1;
        } else {
            fputs(buffer, fpTemp);
        }
    
    }

    if (!ditemukan) {
        fprintf(fpTemp, "%s|%s|%d|%.2f|%.2f|%.2f|%.2f|%.2f|%c|%s\n",
            mhsBaru.nim, mhsBaru.nama, mhsBaru.jumlahKehadiran,
            mhsBaru.nilaiPresensi, mhsBaru.nilaiKeaktifan,
            mhsBaru.nilaiUTS, mhsBaru.nilaiUAS,
            mhsBaru.nilaiAkhir,mhsBaru.grade, mhsBaru.status);
    }
       
    fclose(fpRead);
    fclose(fpTemp);
    remove(FILE_NILAI);
    rename(FILE_TEMP, FILE_NILAI);
}

void lihatNilaiSesuaiNIM(char *cariNIM) {
    FILE *fp = fopen(FILE_NILAI, "r");
    char buffer[256];
    struct Mahasiswa b;

    if (!fp) {
        printf("[!] Belum ada data nilai.\n");
        getchar();
        return;
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        sscanf(buffer, "%[^|]|%[^|]|%d|%f|%f|%f|%f|%f|%c|%[^\n]",
            b.nim, b.nama, &b.jumlahKehadiran,
            &b.nilaiPresensi, &b.nilaiKeaktifan,
            &b.nilaiUTS, &b.nilaiUAS,
            &b.nilaiAkhir, &b.grade, b.status);

        if (strcmp(b.nim, cariNIM) == 0) {
            printf("NIM: %s\nNAMA: %s\nNilai Akhir: %.2f\n",
                b.nim, b.nama, b.nilaiAkhir);
                break;
            }
        
    }
    fclose(fp);
}