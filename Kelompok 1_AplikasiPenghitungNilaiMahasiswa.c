#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 
#include "auth_menu.c"   // Bagian 2 - Putri (Autentikasi & Menu)

#define MAX_PERTEMUAN 18.0
#define FILE_NILAI "nilai.txt"
#define FILE_LOGIN "login.txt"
#define FILE_TEMP "temp_nilai.txt"

struct Mahasiswa {
    char nim[20];
    char nama[50];
    int jumlahKehadiran;  
    float nilaiPresensi;  
    float nilaiKeaktifan;
    float nilaiUTS;
    float nilaiUAS;
    float nilaiAkhir;
    char grade;
    char status[20];
};

struct Akun {
    char username[20];
    char password[20];
    char role[10];
};

void menuUtama();
int cekLogin(char *user, char *pass, char *roleWajib);
int cekNIMTerdaftar(char *nimCari); // FUNGSI BARU
void menuAdmin();
void menuMahasiswa();
void inputDataNilai();
void hitungNilai(struct Mahasiswa *mhs);
char tentukanGrade(float na);
void tentukanStatus(float na, char *status);
void updateNilai(struct Mahasiswa mhs); // FUNGSI UPDATE BARU
void lihatNilaiSesuaiNIM(char *cariNIM);
void bersihkanBuffer();
int cekValidasiAngka(char *str);

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    menuAuth();     // Bagian 2 - Putri
    return 0;
}

void bersihkanBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int cekValidasiAngka(char *str) {
    if (strlen(str) != 7) return 0;
    for (int i = 0; i < 7; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int cekNIMTerdaftar(char *nimCari) {
    FILE *fp = fopen(FILE_LOGIN, "r");
    char buffer[100];
    char fileNIM[20], dump1[20], dump2[10];
    int ditemukan = 0;

    if (fp == NULL) return 0; // File tidak ada, dianggap tidak terdaftar

    while (fgets(buffer, sizeof(buffer), fp)) {
        sscanf(buffer, "%[^|]|%[^|]|%s", fileNIM, dump1, dump2);
        
        if (strcmp(fileNIM, nimCari) == 0) {
            ditemukan = 1;
            break;
        }
    }
    fclose(fp);
    return ditemukan;
}

void menuUtama() {
    int pilihan;
    do {
        printf("\n__________________________________________\n");
        printf(" DATABASE NILAI MAHASISWA TEKNIK ELEKTRO 01 \n");
        printf("___________________________________________\n");
        printf("1. Login Admin\n");
        printf("2. Login Mahasiswa\n");
        printf("3. Keluar\n");
        printf("==========================================\n");
        printf("Pilihan: ");
        
        if (scanf("%d", &pilihan) != 1) {
            bersihkanBuffer();
            pilihan = 0;
        } else {
            bersihkanBuffer();
        }

        switch (pilihan) {
            case 1: menuAdmin(); break;
            case 2: menuMahasiswa(); break;
            case 3: printf("Terima kasih banyak.\n"); exit(0);
            default: printf("Maaf, Pilihannya tidak valid.\n");
        }
    } while (pilihan != 3);
}


// Bagian 2: Putri Ayu Dahlia Nainggolan

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


//bagian 3_Evandra Akmal Syahputra

void inputDataNilai() {
    struct Mahasiswa mhs;
    int nimValid = 0;

    do {
        printf("Masukan NIM: ");
        scanf("%s", mhs.nim);
        bersihkanBuffer();

        if (!cekValidasiAngka(mhs.nim)) {
            printf("[!] NIM harus angka\n");
            continue;
        }

        if (!cekNIMTerdaftar(mhs.nim)) {
            printf("[!] NIM tidak terdaftar\n");
            if (strcmp(mhs.nim, "0") == 0) return;
        } else nimValid = 1;
    } while (!nimValid);

    printf("Nama: ");
    scanf("%[^\n]", mhs.nama);
    bersihkanBuffer();

    printf("Kehadiran: ");
    scanf("%d", &mhs.jumlahKehadiran);
    printf("Nilai Keaktifan: ");
    scanf("%f", &mhs.nilaiKeaktifan);
    printf("Nilai UTS: ");
    scanf("%f", &mhs.nilaiUTS);
    printf("Nilai UAS: ");
    scanf("%f", &mhs.nilaiUAS);

    hitungNilai(&mhs);
    updateNilai(mhs);
}

void hitungNilai(struct Mahasiswa *mhs) {
    mhs->nilaiPresensi = ((float)mhs->jumlahKehadiran / MAX_PERTEMUAN) * 100;
    mhs->nilaiAkhir =
        (mhs->nilaiPresensi * 0.25) +
        (mhs->nilaiKeaktifan * 0.05) +
        (mhs->nilaiUTS * 0.30) +
        (mhs->nilaiUAS * 0.40);

    mhs->grade = tentukanGrade(mhs->nilaiAkhir);
    tentukanStatus(mhs->nilaiAkhir, mhs->status);
}

char tentukanGrade(float na) {
    if (na >= 85) return 'A';
    else if (na >= 75) return 'B';
    else if (na >= 65) return 'C';
    else if (na >= 50) return 'D';
    else return 'E';
}

void tentukanStatus(float na, char *status) {
    if (na >= 65) strcpy(status, "Lulus");
    else strcpy(status, "Tidak Lulus");
}
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

