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