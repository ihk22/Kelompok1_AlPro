
int cekLogin(char *user, char *pass, char *roleWajib) {
    FILE *fp = fopen(FILE_LOGIN, "r");
    char buffer[100];
    char fileUser[20], filePass[20], fileRole[10];
    int loginSukses = 0;

    if (fp == NULL) {
        printf("[!] ERROR: File '%s' tidak ditemukan!\n", FILE_LOGIN);
        return 0;
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        sscanf(buffer, "%[^|]|%[^|]|%s", fileUser, filePass, fileRole);
        if (strcmp(user, fileUser) == 0 &&
            strcmp(pass, filePass) == 0 &&
            strcmp(roleWajib, fileRole) == 0) {
            loginSukses = 1;
            break;
        }
    }
    fclose(fp);
    return loginSukses;
}


void menuAdmin() {
    char id[20], pass[20];
    printf("\n--- LOGIN ADMIN ---\n");
    printf("ID Admin : "); scanf("%s", id);
    printf("Password : "); scanf("%s", pass);
    bersihkanBuffer();

    if (cekLogin(id, pass, "ADMIN")) {
        int pilihan;
        do {
            printf("\n1. Input/Update Nilai\n");
            printf("2. Kembali\n");
            printf("Pilihan: ");
            scanf("%d", &pilihan);
            bersihkanBuffer();
            if (pilihan == 1) inputDataNilai();
        } while (pilihan != 2);
    } else {
        printf("[!] Login gagal\n");
        getchar();
    }
}


void menuMahasiswa() {
    char nim[20], pass[20];
    printf("\n--- LOGIN MAHASISWA ---\n");
    printf("NIM : "); scanf("%s", nim);
    printf("Password : "); scanf("%s", pass);
    bersihkanBuffer();

    if (cekLogin(nim, pass, "MHS")) {
        lihatNilaiSesuaiNIM(nim);
    } else {
        printf("[!] Login gagal\n");
        getchar();
    }
}
