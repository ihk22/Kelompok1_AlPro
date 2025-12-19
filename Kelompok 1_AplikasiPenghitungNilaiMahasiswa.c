#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 

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
    menuUtama();
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
